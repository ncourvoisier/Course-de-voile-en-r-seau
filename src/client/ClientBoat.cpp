#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include <string>

#include "../Constants.h"

#include "ClientBoat.h"
#include "Singletons.h"

namespace sail
{

    ClientBoat::ClientBoat(std::string name)
    : gf::Entity(1)
    , m_name(name)
    , m_texture(gTextureAtlas().getTexture())
    , m_boat(gTextureAtlas().getTextureRect("boat"))
    , m_sail(gTextureAtlas().getTextureRect("sail"))
    , m_rudder(gTextureAtlas().getTextureRect("rudder"))
    {

    }

    ClientBoat::~ClientBoat()
    {

    }

    void ClientBoat::update(gf::Time time)
    {
        // TODO : prediction
    }

    void ClientBoat::render(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::Vector2f position(m_x, m_y);

        gf::Sprite spriteBoat;
        spriteBoat.setTexture(m_texture);
        spriteBoat.setPosition(position);
        spriteBoat.setScale(0.2f); // TODO : ADAPT THIS
        spriteBoat.setTextureRect(m_boat);
        spriteBoat.setAnchor(gf::Anchor::Center);
        spriteBoat.setRotation(angle + M_PI);

        gf::RenderStates shipStates;
        shipStates.transform = spriteBoat.getTransform();

        gf::Sprite spriteSail;
        spriteSail.setTexture(m_texture);
        spriteSail.setPosition({57.0f, 110.0f});
        spriteSail.setTextureRect(m_sail);
        spriteSail.setAnchor(gf::Anchor::TopRight);
        spriteSail.setRotation(sail_angle);

        gf::Sprite spriteRudder;
        spriteRudder.setTexture(m_texture);
        spriteRudder.setPosition({57.0f,299.0f});
        spriteRudder.setTextureRect(m_rudder);
        spriteRudder.setAnchor(gf::Anchor::Center);
        spriteRudder.setRotation(rudder_angle);

        gf::ResourceManager resources;
        resources.addSearchDir("../resources");
        gf::Font& font = resources.getFont("arial.ttf");

//        std::string name = "Nicolas";
        std::string name = m_name;
        gf::Text textName;
        textName.setString(name);
        textName.setCharacterSize(65);
        textName.setColor(gf::Color::White);
        textName.setParagraphWidth(800);
        textName.setAlignment(gf::Alignment::Center);
        textName.setFont(font);
        textName.setAnchor(gf::Anchor::Center);
        textName.setRotation(80.0);
        textName.setPosition({-110.0f, 150.0f});

        /*std::string info = "Speed : Sail angle : ";
        std::ostringstream sa;
        sa << sail_angle;
        std::string str = sa.str();
        auto res = info + str;
        gf::Text textInfo;
        textInfo.setString(res);
        textInfo.setCharacterSize(75);
        textInfo.setColor(gf::Color::White);
        textInfo.setParagraphWidth(800);
        textInfo.setAlignment(gf::Alignment::Center);
        textInfo.setFont(font);
        textInfo.setAnchor(gf::Anchor::Center);
        textInfo.setPosition({300.0f, 300.0f});
        target.draw(textInfo, shipStates);*/

        target.draw(textName, shipStates);
        target.draw(spriteRudder, shipStates);
        target.draw(spriteBoat, states);
        target.draw(spriteSail, shipStates);
    }

    void ClientBoat::fromBoatData(BoatData data)
    {
        m_x = data.xPos * WorldScale;
        m_y = data.yPos * WorldScale;
        angle = data.angle;
        sail_angle = data.sailAngle;
        rudder_angle = data.rudderAngle;
    }

}