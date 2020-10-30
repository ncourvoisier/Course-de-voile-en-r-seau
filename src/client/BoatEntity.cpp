#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "BoatEntity.h"

namespace sail
{

    BoatEntity::BoatEntity(gf::Color4f color)
    : m_position({0, 0})
    , m_velocity({0, 0})
    , m_size(50.0f)
    , m_color(color)
    {

    }

    BoatEntity::~BoatEntity()
    {

    }

    void BoatEntity::update(gf::Time time)
    {
        // TODO : INSERT MOVE PREDICTION LOGIC SOMEWHERE HERE (I GUESS)

    }

    void BoatEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
    {
        gf::RectangleShape shape({m_size, m_size});
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape, states);
    }

    const gf::Vector2f &BoatEntity::getPosition() const
    {
        return m_position;
    }

    void BoatEntity::setPosition(const gf::Vector2f &mPosition)
    {
        m_position = mPosition;
    }

    const gf::Vector2f &BoatEntity::getVelocity() const
    {
        return m_velocity;
    }

    void BoatEntity::setVelocity(const gf::Vector2f &mVelocity)
    {
        m_velocity = mVelocity;
    }

}