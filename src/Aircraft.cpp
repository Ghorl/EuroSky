#include "../include/Aircraft.h"

Aircraft::Aircraft(string m, int capacity, double fuel_consumption, 
                   double speed, double range, 
                   double crew_cost, double maintenance_cost)
    : model_name(m), 
      capacity_passengers(capacity), 
      fuel_consumption_per_hour(fuel_consumption), 
      cruise_speed_kmh(speed),
      max_range_km(range),
      crew_cost_per_hour(crew_cost),
      maintenance_cost_per_hour(maintenance_cost) {}