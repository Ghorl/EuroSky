#include "../include/Route.h"      
#include "../include/Aircraft.h"   

Route::Route(Airport* orig, Airport* dest, double distance, bool operational)
    : origin(orig), 
      destination(dest), 
      distance_km(distance), 
      is_operational(operational) {}

double Route::getFlightTime(Aircraft* aircraft) {
    if (!aircraft->canFly(distance_km)) {
        return -1;
    }
    return distance_km / aircraft->cruise_speed_kmh;
}