#include "Boat.h"

namespace sail {

    Boat::Boat()
    : x(0.05)
    , y(0.05) // TODO : don't initialize here
    , angle(M_PI_4)
    , sail_angle(0)
    , rudder_angle(0)
    , sail_is_free(0)
    , v(5)
    , sheet_length(1)
    , rotational_velocity(0.0)
    , inertia(10000.0)
    , drift_coefficient(0.05)
    , rudder_distance(4.0)
    , angular_friction(8000)
    , mass(30.0)
    , tangential_friction(0.1)
    , mast_distance(1)
    , rudder_lift(8000.0)
    , sail_center_of_effort(1.0)
    , sail_lift(1000.0)
    {

    }

    double Boat::getAngle() const {
        return angle;
    }

    double Boat::getLatitude() const {
        return y;
    }

    double Boat::getLongitude() const {
        return x;
    }

    double Boat::getRudderAngle() const {
        return rudder_angle;
    }

    double Boat::getSailAngle() const {
        return sail_angle;
    }

    double Boat::getSheetLength() const {
        return sheet_length;
    }

    double Boat::getVelocity() const {
        return v;
    }

    double Boat::getRudderLift() const {
        return rudder_lift;
    }

    double Boat::getSailLift() const {
        return sail_lift;
    }

    double Boat::getDriftCoefficient() const {
        return drift_coefficient;
    }

    int Boat::getSailIsFree() const {
        return sail_is_free;
    }

    double Boat::getRotationalVelocity() const {
        return rotational_velocity;
    }

    double Boat::getEll() const {
        return ell;
    }

    double Boat::getMass() const {
        return mass;
    }

    double Boat::getRudderDistance() const {
        return rudder_distance;
    }

    double Boat::getMastDistance() const {
        return mast_distance;
    }

    double Boat::getTangentialFriction() const {
        return tangential_friction;
    }

    double Boat::getAngularFriction() const {
        return angular_friction;
    }

    double Boat::getSailCenterOfEffort() const {
        return sail_center_of_effort;
    }

    double Boat::getInertia() const {
        return inertia;
    }

    void Boat::setAngle(double angle) {
        Boat::angle = angle;
    }

    void Boat::setLatitude(double latitude) {
        Boat::y = latitude;
    }

    void Boat::setLongitude(double longitude) {
        Boat::x = longitude;
    }

    void Boat::setRudderAngle(double rudder_angle) {
        Boat::rudder_angle = rudder_angle;
    }

    void Boat::setSailAngle(double sail_angle) {
        Boat::sail_angle = sail_angle;
    }

    void Boat::setSheetLength(double sheet_length) {
        Boat::sheet_length = sheet_length;
    }

    void Boat::setRotationalVelocity(double rotationalVelocity) {
        Boat::rotational_velocity = rotationalVelocity;
    }

    void Boat::setVelocity(double v) {
        Boat::v = v;
    }

}
