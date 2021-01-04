//
// Created by nicolas on 04/01/2021.
//

#include <gf/ResourceManager.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <iostream>
#include "Banner.h"
#include "Singletons.h"

namespace sail {

    Banner::Banner(gf::Vector2f target, gf::Color4f color)
    : gf::Entity(1)
    , m_target(target)
    , m_color(color)
    , m_text()
    , m_clock()
    , m_displate(false)
    {
        gf::Font& font = gResourceManager().getFont("arial.ttf");
        m_text.setCharacterSize(70);
        m_text.setColor(m_color);
        m_text.setParagraphWidth(800);
        m_text.setAlignment(gf::Alignment::Center);
        m_text.setFont(font);
        m_text.setAnchor(gf::Anchor::Center);
        m_text.setPosition({0.0f, 50.0f});
    }

    Banner::~Banner() {

    }

    void Banner::update(gf::Time time) {
        if (!m_displate) {
            return;
        }
        if (m_clock.getElapsedTime().asSeconds() > 3) {
            m_displate = false;
        }
    }

    void Banner::displayText(std::string text) {
        m_text.setString(text);
        m_displate = true;
        m_clock.restart();
    }

    void Banner::render(gf::RenderTarget& target, const gf::RenderStates &states) {
        std::cout << "ICI";
        if (m_displate) {
            std::cout << "AFFICHAGE";
            target.draw(m_text, states);
        }
    }



}