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
    , Boat(0, 0)
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
        gf::Vector2f position(getScaledX(), getScaledY());

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


        gf::Font& font = gResourceManager().getFont("arial.ttf");

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

        target.draw(textName, shipStates);
        target.draw(spriteRudder, shipStates);
        target.draw(spriteBoat, states);
        target.draw(spriteSail, shipStates);
    }

    void ClientBoat::fromBoatData(BoatData data)
    {
        m_x = data.xPos;
        m_y = data.yPos;
        angle = data.angle;
        sail_angle = data.sailAngle;
        rudder_angle = data.rudderAngle;
        sheet_length = data.sheetLength;
        rotational_velocity = data.rotationalVelocity;
        v = data.velocity;
    }

    double ClientBoat::getScaledX()
    {
        return m_x * WorldScale;
    }

    double ClientBoat::getScaledY()
    {
        return m_y * WorldScale;
    }

}