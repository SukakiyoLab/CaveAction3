#include "animation_image.h"

#include "image_storage.h"

#include "debug_log.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "xml_loader.h"

#include <string>


namespace component
{

	CAT_AnimationImage::CAT_AnimationImage(CAT_Transform *const transform, ComponentInitializer* cInit, SDL_Renderer* renderer_ptr)
		:CAT_ImageRoot(transform, static_cast<CAT_ImageRoot::ComponentInitializer*>(cInit)) {
		debug::debugLog("Create Image!\n");
		
		CAT_ImageStorage *storage = CAT_ImageStorage::getInstance();

		this->m_renderer = renderer_ptr;
		this->offset = cInit->offset;

		XMLLoader::AnimLoader animLoader;

		animLoader.Load(cInit->xml_file);

		for (int id = 0; id < animLoader.get_size(); id++) {

			std::vector<std::string> paths = animLoader.get_filenames(id);
			std::vector<double> durations = animLoader.get_durations(id);


			ImageState state;

			double sum_time = 0;
			for (int i = 0; i < durations.size(); i++) {
				sum_time += durations[i];
				state.duration_times.push_back(sum_time);
			}

			for (int i = 0; i < paths.size(); i++)
			{
				storage->save_image(paths[i].c_str());
				state.m_images.push_back(storage->get_image(paths[i].c_str()));


				if (!state.m_images[i])
				{
					debug::debugLog("IMG_Load: %s\n", IMG_GetError());
				}

				SDL_SetColorKey(state.m_images[i],
					SDL_TRUE,
					SDL_MapRGB(state.m_images[i]->format, 255, 0, 255));

				state.m_textures.push_back(SDL_CreateTextureFromSurface(m_renderer, state.m_images[i]));


				SDL_QueryTexture(state.m_textures[i], &(this->m_format), NULL, &(state.m_w), &(state.m_h));
				state.m_image_rects.push_back(SDL_Rect{ 0, 0, state.m_w, state.m_h });
			}

			this->states.push_back(state);
		}
	}

	void CAT_AnimationImage::gain(int delta_time) {
		sum_time += delta_time;
	}

	void CAT_AnimationImage::project(CAT_ViewCamera* camera)
	{
		//sum_time += delta_time;
		double time = fmod(sum_time, this->states[state_id].duration_times[states[state_id].duration_times.size() - 1]);
		int num = 0;

		while(true){
			if(states[state_id].duration_times[num] < time) {
				num++;
			}else{
				break;
			}
		}

		float draw_w = this->states[state_id].m_w * this->m_transform->get_scale()[0];
		float draw_h = this->states[state_id].m_h * this->m_transform->get_scale()[1];

		

		Vector3d pos = this->m_transform->get_position() - camera->get_position() + camera->get_view_port_center();

		this->m_draw_rect = SDL_FRect{(float)pos[0] - draw_w / 2 + offset[0], (float)pos[1] - draw_h / 2 + offset[1], draw_w, draw_h};

		SDL_SetTextureAlphaMod(this->states[state_id].m_textures[num], 255);


		SDL_RenderCopyExF(this->m_renderer,
						  this->states[state_id].m_textures[num],
						  &(this->states[state_id].m_image_rects[num]),
						  &(this->m_draw_rect),
						  0,
						  NULL,
						  SDL_FLIP_NONE);
	}

	int CAT_AnimationImage::change_animation(unsigned short new_id) {
		if (new_id != this->state_id) {
			debug::debugLog("Change Animation!");
			this->sum_time = 0;
		}

		CAT_AnimationRoot::change_animation(new_id); /* 親クラスのメンバ関数 */
		return 0;
	}
}