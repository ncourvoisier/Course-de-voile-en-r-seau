//
// Created by nicolas on 04/01/2021.
//

#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Color.h>
#include <string>
#include <gf/Views.h>

#ifndef SAILINSANE_BANNER_H
#define SAILINSANE_BANNER_H

namespace sail {

    class Banner: public gf::Entity {

    public:
        Banner(gf::Vector2f target, gf::Color4f color);
        ~Banner();
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        void update(gf::Time time) override;
        void displayText(std::string text);

    private:
        gf::Color4f m_color;
        gf::Text m_text;
        gf::Vector2f m_target;
        gf::Clock m_clock;
        bool m_displate;
    };

}

#endif //SAILINSANE_BANNER_H
