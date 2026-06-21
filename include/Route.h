#ifndef ROUTE_H
#define ROUTE_H
#include "Aircraft.h"
#include "Airport.h"

class Route{
    public:
       Airport* origin;
       Airport* destination;
       double distance_km;  //From google maps

       bool is_operational;

       double getFlightTime(Aircraft* aircraft);

       Route(Airport* origin, Airport* destination, double distance,bool operational);
};
#endif