#ifndef SAILINSANE_NAVIGATIONARROW_H
#define SAILINSANE_NAVIGATIONARROW_H

#include <gf/Texture.h>
#include <gf/Entity.h>
#include <gf/VertexArray.h>
#include <gf/Shapes.h>
#include <gf/Window.h>
#include "../Protocol.h"
#include "ClientBoat.h"

namespace sail
{

    class NavigationArrow: public gf::Entity
    {
        public:
            NavigationArrow(gf::Vector2f target, ClientBoat& boat, gf::View& view);

        void update(gf::Time time) override;

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
            gf::ConvexShape m_arrow;

            gf::Vector2f m_target;

            ClientBoat& m_boat;

            gf::View& m_view;
    };

}

#endif //SAILINSANE_NAVIGATIONARROW_H
