#ifndef AIRPORT_H
#define AIRPORT_H
#include <string>
#include <vector>
#include "Aircraft.h"
using namespace std;

class Airport {
    public:
      string codeIATA;
      string name;
      string country;
      
      double latitude;
      double longitude;
      
      vector<Aircraft*> available_aircraft;
      
      double landing_fee;
      double handling_fee;
      
      Airport(string code, string n, string c, double lat, double lon, 
              double landing, double handling);
      
      void addAircraft(Aircraft* aircraft);
      bool hasAircraft(Aircraft* aircraft);
};
#endif