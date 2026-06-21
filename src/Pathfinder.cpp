#include "PathFinder.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void PathFinder::saveRouteToJSON(string filename) {
    json route_data;
    
    route_data["origen"] = path.empty() ? "" : path[0];
    route_data["destino"] = path.empty() ? "" : path[path.size() - 1];
    route_data["algoritmo"] = getName();
    route_data["costo_total"] = total_cost;
    route_data["tiempo_total"] = total_time;
    route_data["avion"] = aircraft_choice;
    route_data["ruta"] = json::array();
    
    // Cargar ruta como array
    for (const string& code : path) {
        route_data["ruta"].push_back(code);
    }
    
    // Aeropuertos en ruta
    route_data["aeropuertos"] = json::array();
    for (const string& code : path) {
        Airport* apt = graph->getAirport(code);
        if (apt) {
            json apt_data;
            apt_data["iata"] = apt->codeIATA;
            apt_data["lat"] = apt->latitude;
            apt_data["lng"] = apt->longitude;
            apt_data["nombre"] = apt->name;
            route_data["aeropuertos"].push_back(apt_data);
        }
    }
    
    // Desglose de costos
    route_data["desglose"] = json::object();
    route_data["desglose"]["combustible"] = 0.0;
    route_data["desglose"]["tripulacion"] = 0.0;
    route_data["desglose"]["mantenimiento"] = 0.0;
    route_data["desglose"]["aterrizaje"] = 0.0;
    route_data["desglose"]["servicios"] = 0.0;
    
    for (int i = 0; i < (int)path.size() - 1; i++) {
        Route* route = graph->getRoute(path[i], path[i+1]);
        if (route && !route->origin->available_aircraft.empty()) {
            Aircraft* aircraft = route->origin->available_aircraft[0];
            
            double fuel = cost_model->calculateFuelCost(route, aircraft);
            double crew = cost_model->calculateCrewCost(route, aircraft);
            double maint = cost_model->calculateMaintenanceCost(route, aircraft);
            double land = cost_model->calculateLandingFee(route);
            double hand = cost_model->calculateHandlingFee(route);
            
            route_data["desglose"]["combustible"] = route_data["desglose"]["combustible"].get<double>() + fuel;
            route_data["desglose"]["tripulacion"] = route_data["desglose"]["tripulacion"].get<double>() + crew;
            route_data["desglose"]["mantenimiento"] = route_data["desglose"]["mantenimiento"].get<double>() + maint;
            route_data["desglose"]["aterrizaje"] = route_data["desglose"]["aterrizaje"].get<double>() + land;
            route_data["desglose"]["servicios"] = route_data["desglose"]["servicios"].get<double>() + hand;
        }
    }
    
    // Guardar a archivo
    ofstream file(filename);
    file << route_data.dump(4);
    file.close();
    
    cout << "[✓] Resultado guardado en " << filename << endl;
}