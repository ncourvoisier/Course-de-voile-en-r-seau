#include <gf/ResourceManager.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <iostream>
#include <utility>
#include "Banner.h"
#include "Singletons.h"

namespace sail {

    static constexpr float BannerWidth = 20.0f;
    static constexpr float OutlineThickness = 450.0f;
    static constexpr float TextOutlineThickness = 600.0f;
    static constexpr int TextSize = 40;

    Banner::Banner(gf::View& view)
    : gf::Entity(1)
    , m_view(view)
    , m_textColor(gf::Color::White)
    , m_text()
    , m_rectangle()
    , m_rectangleColor(gf::Color::fromRgba32(130, 130, 130, 150))
    , m_clock()
    , m_display(false)
    {
        gf::Font& font = gResourceManager().getFont("arial.ttf");
        m_text.setCharacterSize(45);
        m_text.setColor(m_textColor);
        m_text.setParagraphWidth(800);

        m_text.setFont(font);
        m_text.setLetterSpacing(5.0f);
        m_text.setOutlineColor(gf::Color::Black);
        m_rectangle.setColor(m_rectangleColor);
        m_rectangle.setOutlineColor(gf::Color::Black);
    }

    void Banner::update(gf::Time time) {
        if (!m_display)
            return;
        if (m_clock.getElapsedTime().asSeconds() > 3)
            m_display = false;
    }

    void Banner::displayText(std::string text) {
        m_text.setString(std::move(text));
        m_display = true;
        m_clock.restart();
    }

    void Banner::render(gf::RenderTarget& target, const gf::RenderStates &states) {
        if (! m_display)
            return;

        gf::Vector2f vSize = m_view.getSize();

        m_rectangle.setPosition({ 0, vSize.y * 0.8f });
        m_rectangle.setSize({ vSize.x, vSize.y / BannerWidth });
        m_rectangle.setOutlineThickness(vSize.y / OutlineThickness);

        m_text.setCharacterSize(vSize.y / TextSize);
        m_text.setPosition({ vSize.x / 2.0f, vSize.y * 0.8f + vSize.y / BannerWidth / 2.0f });
        m_text.setAnchor(gf::Anchor::Center);
        m_text.setOutlineThickness(vSize.y / TextOutlineThickness);

        target.draw(m_rectangle, states);
        target.draw(m_text, states);
    }

}