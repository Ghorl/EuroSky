#ifndef AIRCRAFT_H
#define AIRCRAFT_H
#include <string>
using namespace std;

class Aircraft {
    public:
      //Atributos del avión
      string model_name;
      int capacity_passengers;

      //Atributos para el consumo
      double fuel_consumption_per_hour;  // Litros/hora
      double cruise_speed_kmh;
      double max_range_km;
      
      //Atributos para los costos operativos
      double crew_cost_per_hour;
      double maintenance_cost_per_hour;  

      Aircraft(string m, int capacity, double fuel_consumption, 
               double speed, double range, double crew_cost, double maintenance_cost);
      
      bool canFly(double distance) {
        return distance <= max_range_km;
      }
};
#endif