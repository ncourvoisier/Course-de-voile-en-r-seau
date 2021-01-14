#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Color.h>
#include <string>
#include <gf/Views.h>
#include <gf/Shapes.h>

#ifndef SAILINSANE_BANNER_H
#define SAILINSANE_BANNER_H

namespace sail {

    class Banner: public gf::Entity {

    public:
        Banner(gf::View& view);
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        void update(gf::Time time) override;
        void displayText(std::string text);

    private:
        gf::RectangleShape m_rectangle;
        gf::Color4f m_rectangleColor;
        gf::Text m_text;
        gf::Color4f m_textColor;
        gf::Clock m_clock;
        gf::View& m_view;
        bool m_display;
    };

}

#endif //SAILINSANE_BANNER_H
