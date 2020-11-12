#ifndef SAILINSANE_BOAT_H
#define SAILINSANE_BOAT_H

#include <cmath>
#include <string>
#include <stdlib.h>

namespace sail {

    class Boat {
    public:
        Boat() = default;
        Boat(char* name);
        ~Boat();
        Boat *sailing_boat_init();
        void sailing_boat_reset();
        void sailing_boat_free(Boat *boat);
        char* getName();
        double getLatitude() const;
        void setLatitude(double latitude);
        double getLongitude() const;
        void setLongitude(double longitude);
        double getSailAngle() const;
        void setSailAngle(double sail_angle);
        double getAngle() const;
        void setAngle(double angle);
        double getRudderAngle() const;
        void setRudderAngle(double rudder_angle);
        double getVelocity() const;
        void setVelocity(double v);
        void setSheetLength(double sheet_length);
        double getSheetLength() const;
        double getRudderLift() const;
        double getSailLift() const;
        double getDriftCoefficient() const;
        int getSailIsFree() const;
        double getRotationalVelocity() const;
        double getEll() const;
        double getMass() const;
        double getRudderDistance() const;
        double getMastDistance() const;
        double getTangentialFriction() const;
        double getAngularFriction() const;
        double getSailCenterOfEffort() const;
        double getInertia() const;
        void setRotationalVelocity(double rotationalVelocity);

    private:
        char* name;

        /* coordinates of boat */
        double x;
        double y;

        /* orientation of boat */
        double angle;
        double sail_angle;
        double rudder_angle;
        double sheet_length;

        /* is the sail free to move? */
        int sail_is_free;

        /* state variables */
        double v;
        double rotational_velocity;
        double ell;

        /* parameters */
        double drift_coefficient;
        double mass;
        double rudder_distance;
        double mast_distance;
        double rudder_lift;
        double sail_lift;
        double tangential_friction;
        double angular_friction;
        double sail_center_of_effort;
        double inertia;
    };
}


#endif //SAILINSANE_BOAT_H
