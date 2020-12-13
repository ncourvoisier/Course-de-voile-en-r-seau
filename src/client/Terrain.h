#ifndef SAILINSANE_TERRAIN_H
#define SAILINSANE_TERRAIN_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/VertexArray.h>
#include "ClientBoat.h"

namespace sail
{

class Terrain: public gf::Entity
{
    public:
        static constexpr unsigned Size = 1000; // TODO : shared constant with server
        static constexpr unsigned DisplayHalfRange = 50;
        static constexpr float TileSize = 10.0f;

        Terrain(ClientBoat& playerBoat);

        void load(gf::Array2D<float> elevations);

        void update(gf::Time time) override;

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        gf::Array2D<gf::Color4f> m_terrain;

        gf::VertexArray m_vertices;

        gf::Vector2u m_oldPosition;

        ClientBoat& m_playerBoat;
    };

}

#endif //SAILINSANE_TERRAIN_H
