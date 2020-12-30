#ifndef SAILINSANE_WEATHERCOCK_H
#define SAILINSANE_WEATHERCOCK_H

#include <gf/Texture.h>
#include <gf/Entity.h>
#include "../Protocol.h"

namespace sail
{

    class Weathercock: public gf::Entity
    {
        public:
            Weathercock();

            void fromWindData(const WindData& wind);

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
            const gf::Texture& m_texture;

            const gf::Vector2f  m_position;

            double m_speed;
            double m_direction;

    };

}

#endif //SAILINSANE_WEATHERCOCK_H
