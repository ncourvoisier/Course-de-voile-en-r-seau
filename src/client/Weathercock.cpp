#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "Weathercock.h"
#include "Singletons.h"

namespace sail
{

    Weathercock::Weathercock()
    : m_texture(sail::gResourceManager().getTexture("wind-direction.png"))
    , m_position({ 0.0f, 0.0f })
    {

    }

    void Weathercock::fromWindData(const WindData &wind)
    {
        m_speed = wind.speed;
        m_direction = wind.direction;
    }

    void Weathercock::render(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Sprite cock;
        cock.setTexture(m_texture);
        cock.setPosition(m_position);
        cock.setRotation(m_direction);
        cock.setScale(0.05); // TODO : make it dynamic
        target.draw(cock, states);
    }

}