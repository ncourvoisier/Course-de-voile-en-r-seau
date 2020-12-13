#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <iostream>
#include <gf/Sprite.h>

#include "ClientBoat.h"
#include "Singletons.h"

namespace sail
{

    ClientBoat::ClientBoat(gf::Color4f color)
    : gf::Entity(1)
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
        gf::Vector2f position(x, y);

        gf::Sprite spriteBoat;
        //std::cout << "POSITION : " << position.x << ", " << position.y << "\n"; NO PROBLEM HERE

        /*gf::RectangleShape sailShip({20.0f, 35.0f});
        sailShip.setPosition(position);
        sailShip.setColor(m_color);
        sailShip.setAnchor(gf::Anchor::Center);
        sailShip.setRotation(angle);*/
        spriteBoat.setTexture(m_texture);
        spriteBoat.setPosition(position);
        spriteBoat.setScale(0.2f); // TODO : ADAPT THIS
        spriteBoat.setTextureRect(m_boat);
        spriteBoat.setAnchor(gf::Anchor::Center);
        spriteBoat.setRotation(angle);

        gf::RenderStates shipStates;
        shipStates.transform = spriteBoat.getTransform();

        /*gf::RectangleShape sail({5.0f, 25.0f});
        sail.setPosition({10.0f, 17.5f});
        sail.setColor(gf::Color::Red);
        sail.setAnchor(gf::Anchor::Center);
        sail.setRotation(sail_angle);*/
        gf::Sprite spriteSail;
        spriteSail.setTexture(m_texture);
        spriteSail.setPosition({57.0f, 110.0f});
        spriteSail.setTextureRect(m_sail);
        spriteSail.setAnchor(gf::Anchor::TopRight);
        spriteSail.setRotation(sail_angle);

        /*gf::RectangleShape rudder({5.0f,10.0f});
        rudder.setPosition({10.0f,35.0f});
        rudder.setColor(gf::Color::Blue);
        rudder.setAnchor(gf::Anchor::Center);
        rudder.setRotation(rudder_angle);*/
        gf::Sprite spriteRudder;
        spriteRudder.setTexture(m_texture);
        spriteRudder.setPosition({57.0f,299.0f});
        spriteRudder.setTextureRect(m_rudder);
        spriteRudder.setAnchor(gf::Anchor::Center);
        spriteRudder.setRotation(rudder_angle);

        target.draw(spriteRudder, shipStates);
        target.draw(spriteBoat, states);
        target.draw(spriteSail, shipStates);
    }

    void ClientBoat::fromBoatData(BoatData data)
    {
        x = data.xPos * Scale;
        y = data.yPos * Scale;
        angle = data.angle;
        sail_angle = data.sailAngle;
        rudder_angle = data.rudderAngle;
    }

}