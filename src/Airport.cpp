#include "../include/Airport.h"

Airport::Airport(string code, string n, string c, double lat, double lon, 
                 double landing, double handling)
    : codeIATA(code), name(n), country(c), latitude(lat), longitude(lon),
      landing_fee(landing), handling_fee(handling) {}

void Airport::addAircraft(Aircraft* aircraft) {
    available_aircraft.push_back(aircraft);
}

bool Airport::hasAircraft(Aircraft* aircraft) {
    for (Aircraft* a : available_aircraft) {
        if (a == aircraft) return true;
    }
    return false;
}