#include "xml_loader.h"

#include <iostream>

namespace XMLLoader {

	void AnimLoader::Load(const char* xml_file) {

        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_string(xml_file);

        for (auto point : doc.child("data").children("animation")) {
            Animation animation;
            for (auto val : point.children("frame")) {
                
                animation.filenames.push_back(std::string(reinterpret_cast<const char*>(val.child_value("filename"))));
                animation.durations.push_back(strtof(reinterpret_cast<const char*>(val.child_value("duration")), nullptr));

                
            }

            this->animations.push_back(animation);
            
        }

	}


    std::vector<CAT_NMRect> NMRectLoad(const char* rect_data, Eigen::Vector2i offset) {
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_string(rect_data);

        std::vector<CAT_NMRect> rect_list;

        for (auto rect : doc.child("root").children("rect")) {
            CAT_NMRect nmrect;

            nmrect.id = std::stoi(std::string(reinterpret_cast<const char*>(rect.child_value("id"))));
            

            int x = std::stoi(std::string(reinterpret_cast<const char*>(rect.child_value("x")))) + offset[0];
            int y = std::stoi(std::string(reinterpret_cast<const char*>(rect.child_value("y")))) + offset[1];
            int width = std::stoi(std::string(reinterpret_cast<const char*>(rect.child_value("width"))));
            int height = std::stoi(std::string(reinterpret_cast<const char*>(rect.child_value("height"))));

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

