#ifndef SAILINSANE_PHYSICS_H
#define SAILINSANE_PHYSICS_H

#define R_EARTH 6378000

#include <cmath>
#include "Boat.h"
#include "Wind.h"

namespace sail
{

    static void sailing_physics_update(sail::Boat boat, const sail::Wind& wind, double dt);

    static double sign_of(double a);
    static double apparent_wind_x(const sail::Boat& boat, const sail::Wind& wind) ;
    static double apparent_wind_y(const sail::Boat& boat, const sail::Wind& wind) ;
    static double apparent_wind_direction(const sail::Boat& boat, const sail::Wind& wind) ;
    static double apparent_wind_speed(const sail::Boat& boat, const sail::Wind& wind) ;

    static bool mainsheet_is_tight(const sail::Boat& boat, const sail::Wind& wind) ;

    static double force_on_rudder(const sail::Boat& boat, const sail::Wind& wind);
    static double force_on_sail(const sail::Boat& boat, const sail::Wind& wind);

    static bool sail_is_bounds(const sail::Boat& boat);

    static double delta_y(const sail::Boat& boat, const sail::Wind& wind);
    static double delta_x(const sail::Boat& boat, const sail::Wind& wind);
    static double delta_relational_velocity(const sail::Boat& boat, const sail::Wind& wind);
    static double delta_velocity(const sail::Boat& boat, const sail::Wind& wind);

}

#endif //SAILINSANE_PHYSICS_H
