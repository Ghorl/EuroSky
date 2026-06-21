#include "../include/Route.h"      
#include "../include/Aircraft.h"   

Route::Route(Airport* orig, Airport* dest, double distance, bool operational)
    : origin(orig),
      destination(dest),
      distance_km(distance),
      is_operational(operational) {

    // Pasajeros y precio promedio según distancia (avance01: 160-240 pax, €95-€380)
    if (distance < 500) {
        expected_passengers = 230;
        avg_ticket_price    = 120.0;
    } else if (distance < 1000) {
        expected_passengers = 210;
        avg_ticket_price    = 200.0;
    } else if (distance < 2000) {
        expected_passengers = 190;
        avg_ticket_price    = 280.0;
    } else {
        expected_passengers = 170;
        avg_ticket_price    = 370.0;
    }
}

double Route::getFlightTime(Aircraft* aircraft) {
    if (!aircraft->canFly(distance_km)) {
        return -1;
    }
    return distance_km / aircraft->cruise_speed_kmh;
}