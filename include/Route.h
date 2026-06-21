#ifndef ROUTE_H
#define ROUTE_H
#include "Aircraft.h"
#include "Airport.h"

class Route{
    public:
       Airport* origin;
       Airport* destination;
       double distance_km;

       bool is_operational;

       // Rentabilidad: pasajeros estimados y precio promedio por boleto según distancia
       double expected_passengers;
       double avg_ticket_price;

       double getFlightTime(Aircraft* aircraft);

       Route(Airport* origin, Airport* destination, double distance, bool operational);
};
#endif