#include "xml_loader.h"

#include <iostream>

namespace XMLLoader {

	void AnimLoader::Load(XMLData* data_name) {

        //pugi::xml_document doc;

        //pugi::xml_parse_result result = doc.load_string(xml_file);

        for (auto point : data_name->nexts["animation"]) {
            Animation animation;
            for (auto val : point->nexts["frame"]) {
                
                animation.filenames.push_back(val->nexts["filename"][0]->item);
                animation.durations.push_back(std::stoi(val->nexts["duration"][0]->item));

                
            }

            this->animations.push_back(animation);
            
        }

	}


    std::vector<CAT_NMRect> NMRectLoad(XMLData* rect_data, Eigen::Vector2i offset) {
        //pugi::xml_document doc;

        //pugi::xml_parse_result result = doc.load_string(rect_data);

        std::vector<CAT_NMRect> rect_list;

        for (auto rect : rect_data->nexts["rect"]) {
            CAT_NMRect nmrect;

            nmrect.id = std::stoi(rect->nexts["id"][0]->item);
            

            int x = std::stoi(rect->nexts["x"][0]->item) + offset[0];
            int y = std::stoi(rect->nexts["y"][0]->item) + offset[1];
            int width = std::stoi(rect->nexts["width"][0]->item);
            int height = std::stoi(rect->nexts["height"][0]->item);

            nmrect.top = y - height;
            nmrect.bottom = y + height;
            nmrect.left = x - width;
            nmrect.right = x + width;

            //debug::debugLog("top : %d , bottom : %d\n", nmrect.top, nmrect.bottom);

            nmrect.center = Eigen::Vector2i(x, y);


            rect_list.push_back(nmrect);
        }

        return rect_list;
    }

    


}

