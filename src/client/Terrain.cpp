#include "Terrain.h"

#include <gf/ColorRamp.h>
#include <gf/Unused.h>
#include <gf/RenderTarget.h>

namespace sail
{

    Terrain::Terrain(ClientBoat& playerBoat)
    : m_playerBoat(playerBoat)
    {

    }

    void Terrain::load(gf::Array2D<float> elevations)
    {
        gf::ColorRamp rampTerrain;
        rampTerrain.addColorStop(0.499f, gf::Color::fromRgba32(17, 82, 118)); // Water
        rampTerrain.addColorStop(1.000f, gf::Color::fromRgba32(38, 143, 14)); // Ground

        for (auto row : elevations.getRowRange())
        {
            for (auto col : elevations.getColRange())
            {
                float elevation = elevations({ row, col });
                m_terrain({ row, col }) = rampTerrain.computeColor(elevation);
            }
        }
    }

    void Terrain::update(gf::Time time)
    {
        gf::unused(time);

        unsigned rowMin = (m_playerBoat.getLongitude() > DisplayHalfRange) ?
                (m_playerBoat.getLongitude() - DisplayHalfRange) : 0;
        unsigned rowMax = (m_playerBoat.getLongitude() + DisplayHalfRange < Size) ?
                (m_playerBoat.getLongitude() + DisplayHalfRange) : Size - 1;
        unsigned colMin = (m_playerBoat.getLatitude() > DisplayHalfRange) ?
                (m_playerBoat.getLatitude() - DisplayHalfRange) : 0;
        unsigned colMax = (m_playerBoat.getLatitude() + DisplayHalfRange < Size) ?
                (m_playerBoat.getLatitude() + DisplayHalfRange) : Size - 1;

        m_vertices.clear();

        for (unsigned row = rowMin; row < rowMax; ++row)
        {
            for (unsigned col = colMin; col < colMax; ++col)
            {
                gf::Vertex vertices[4];

                vertices[0].position = {col * TileSize,row * TileSize };
                vertices[1].position = {(col + 1) * TileSize,row * TileSize };
                vertices[2].position = {col * TileSize,(row + 1) * TileSize };
                vertices[3].position = {(col + 1) * TileSize,(row + 1) * TileSize };

                vertices[0].color = m_terrain({ row, col });
                vertices[1].color = m_terrain({ row,col + 1 });
                vertices[2].color = m_terrain({row + 1, col });
                vertices[3].color = m_terrain({row + 1,col + 1 });

                // first triangle
                m_vertices.append(vertices[0]);
                m_vertices.append(vertices[1]);
                m_vertices.append(vertices[2]);

                // second triangle
                m_vertices.append(vertices[2]);
                m_vertices.append(vertices[1]);
                m_vertices.append(vertices[3]);
            }
        }
    }

    void Terrain::render(gf::RenderTarget &target, const gf::RenderStates& states)
    {
        target.draw(m_vertices, states);
    }

}