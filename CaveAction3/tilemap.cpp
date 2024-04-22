#include "tilemap.h"

#include "image_storage.h"

#include "debug_log.h"
#include "csv_loader.h"

namespace component
{

    CAT_Tilemap::CAT_Tilemap(CAT_Transform *const transform, const char *tilemap_path, std::vector<std::vector<unsigned short>> tilemap_init, Uint8 alpha , SDL_Renderer *const renderer)
    :CAT_ImageRoot(transform) {
        debug::debugLog("Create Tilemap!\n");

        CAT_ImageStorage *storage = CAT_ImageStorage::getInstance();
        storage->save_image(tilemap_path);
        this->m_image = storage->get_image(tilemap_path);
        this->m_renderer = renderer;

        if (!this->m_image)
        {
            debug::debugLog("IMG_Load: %s\n", IMG_GetError());
        }

        SDL_SetColorKey(this->m_image,
                        SDL_TRUE,
                        SDL_MapRGB(this->m_image->format, 255, 0, 255));

        this->m_texture = SDL_CreateTextureFromSurface(m_renderer, m_image);

        this->m_alpha = alpha;

        SDL_QueryTexture(this->m_texture, &(this->m_format), NULL, &(this->m_w), &(this->m_h));
        int w_num = m_w / TILE_SIZE;
        int h_num = m_h / TILE_SIZE;
        for (int j = 0; j < h_num; j++)
        {
            for (int i = 0; i < w_num; i++)
            {
                this->m_image_rect.push_back(SDL_Rect{i * 32, j * 32, TILE_SIZE, TILE_SIZE});
            }
        }

        //tile_init = CSV_Load(std::string(tilemap_init_path));
        tile_init = tilemap_init;

        Vector3d pos = this->m_transform->get_position();

        int loop_h = tile_init.size();
        if (loop_h == 0)
            return;
        int loop_w = tile_init.at(0).size();
        for (int i = 0; i < loop_h; i++)
        {
            for (int j = 0; j < loop_w; j++)
            {
                m_draw_rect.push_back(SDL_Rect{(int) pos[0] + TILE_SIZE * j, (int) pos[1] + TILE_SIZE * i, TILE_SIZE, TILE_SIZE});

            }
        }
        
    }

    void CAT_Tilemap::project(CAT_ViewCamera* camera)
    {
        float draw_w = this->m_w * this->m_transform->get_scale()[0];
		float draw_h = this->m_h * this->m_transform->get_scale()[1];

		// float draw_h = 32;
		// float draw_w = 32;

		Vector3d pos = this->m_transform->get_position() - camera->get_position() + camera->get_view_port_center();
                
        
        int loop_h = tile_init.size();
        if (loop_h == 0)
            return;
        int loop_w = tile_init.at(0).size();
        for (int i = 0; i < loop_h; i++)
        {
            for (int j = 0; j < loop_w; j++)
            {
                Vector2i one_pos = { (int)pos[0] + TILE_SIZE * j - camera->get_view_port_center()[0], (int)pos[1] + TILE_SIZE * i - camera->get_view_port_center()[1]};

                if (one_pos[0] <= camera->get_view_port_param(2) && one_pos[0] >= camera->get_view_port_param(1) &&
                    one_pos[1] <= camera->get_view_port_param(3) && one_pos[1] >= camera->get_view_port_param(0)) {

                    int tile_num = j + i * loop_w;

                    m_draw_rect[tile_num] = (SDL_Rect{ (int)pos[0] + TILE_SIZE * j, (int)pos[1] + TILE_SIZE * i, TILE_SIZE, TILE_SIZE });


                    SDL_SetTextureAlphaMod(this->m_texture, this->m_alpha);


                    SDL_RenderCopy(this->m_renderer,
                        this->m_texture,
                        &(this->m_image_rect[tile_init.at(i).at(j)]),
                        &(this->m_draw_rect[tile_num])
                    );
                }
            }
        }
    }
}