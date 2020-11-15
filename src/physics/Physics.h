#ifndef SAILINSANE_PHYSICS_H
#define SAILINSANE_PHYSICS_H

#define R_EARTH 6378000

#include <cmath>
#include "Boat.h"
#include "Wind.h"

namespace sail
{
     void sailing_physics_update(Boat& boat, const Wind& wind, double dt);

     double sign_of(double a);
     double apparent_wind_x(const Boat& boat, const Wind& wind) ;
     double apparent_wind_y(const Boat& boat, const Wind& wind) ;
     double apparent_wind_direction(const Boat& boat, const Wind& wind) ;
     double apparent_wind_speed(const Boat& boat, const Wind& wind) ;

     bool mainsheet_is_tight(const Boat& boat, const Wind& wind) ;

     double force_on_rudder(const Boat& boat, const Wind& wind);
     double force_on_sail(const Boat& boat, const Wind& wind);

     bool sail_is_bounds(const Boat& boat);

     double delta_y(const Boat& boat, const Wind& wind);
     double delta_x(const Boat& boat, const Wind& wind);
     double delta_rotational_velocity(const Boat& boat, const Wind& wind);
     double delta_velocity(const Boat& boat, const Wind& wind);
}

#endif //SAILINSANE_PHYSICS_H
