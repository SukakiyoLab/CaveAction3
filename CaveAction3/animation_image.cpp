#include "animation_image.h"

#include "image_storage.h"

#include "debug_log.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "xml_loader.h"

#include <string>


namespace component
{

	CAT_AnimationImage::CAT_AnimationImage(CAT_Transform *const transform, const char* xml_file, SDL_Renderer *const renderer)
		:CAT_RawImage(transform) {
		debug::debugLog("Create Image!\n");
		
		CAT_ImageStorage *storage = CAT_ImageStorage::getInstance();

		this->m_renderer = renderer;


		XMLLoader::AnimLoader animLoader;

		animLoader.Load(xml_file);

		for (int id = 0; id < animLoader.get_size(); id++) {

			std::vector<std::string> paths = animLoader.get_filenames(id);
			std::vector<double> durations = animLoader.get_durations(id);


			State state;

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

				SDL_QueryTexture(state.m_textures[i], &(this->m_format), NULL, &(this->m_w), &(this->m_h));
				state.m_image_rects.push_back(SDL_Rect{ 0, 0, m_w, m_h });
			}

			this->states.push_back(state);
		}
	}

	void CAT_AnimationImage::gain(double delta_time) {
		sum_time += delta_time;
	}

	void CAT_AnimationImage::project()
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

		float draw_w = this->m_w * this->m_transform->get_scale()[0];
		float draw_h = this->m_h * this->m_transform->get_scale()[1];

		

		Vector3d pos = this->m_transform->get_position();

		this->m_draw_rect = SDL_FRect{(float)pos[0] - draw_w / 2, (float)pos[1] - draw_h / 2, draw_w, draw_h};

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
		this->state_id = new_id;

		return 0;
	}
}