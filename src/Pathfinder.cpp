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
    
    // Desglose de costos y acumuladores de rentabilidad
    route_data["desglose"] = json::object();
    route_data["desglose"]["combustible"]   = 0.0;
    route_data["desglose"]["tripulacion"]   = 0.0;
    route_data["desglose"]["mantenimiento"] = 0.0;
    route_data["desglose"]["aterrizaje"]    = 0.0;
    route_data["desglose"]["servicios"]     = 0.0;

    double total_income      = 0.0;
    double total_passengers  = 0.0;
    double total_ticket_price = 0.0;
    double total_occupancy   = 0.0;
    int    segments          = 0;

    for (int i = 0; i < (int)path.size() - 1; i++) {
        Route* route = graph->getRoute(path[i], path[i+1]);
        if (route) {
            // Seleccionar avión más barato para el desglose
            Aircraft* best = nullptr;
            double best_c = 1e9;
            for (Aircraft* ac : route->origin->available_aircraft) {
                double c = cost_model->calculateTotalCost(route, ac);
                if (c != -1 && c < best_c) { best_c = c; best = ac; }
            }
            if (!best) continue;

            route_data["desglose"]["combustible"]   = route_data["desglose"]["combustible"].get<double>()   + cost_model->calculateFuelCost(route, best);
            route_data["desglose"]["tripulacion"]   = route_data["desglose"]["tripulacion"].get<double>()   + cost_model->calculateCrewCost(route, best);
            route_data["desglose"]["mantenimiento"] = route_data["desglose"]["mantenimiento"].get<double>() + cost_model->calculateMaintenanceCost(route, best);
            route_data["desglose"]["aterrizaje"]    = route_data["desglose"]["aterrizaje"].get<double>()    + cost_model->calculateLandingFee(route);
            route_data["desglose"]["servicios"]     = route_data["desglose"]["servicios"].get<double>()     + cost_model->calculateHandlingFee(route);

            total_income       += cost_model->calculateIncome(route);
            total_passengers   += route->expected_passengers;
            total_ticket_price += route->avg_ticket_price;
            total_occupancy    += cost_model->calculateOccupancy(route, best);
            segments++;
        }
    }

    // Rentabilidad acumulada
    double avg_passengers = segments > 0 ? total_passengers / segments : 0;
    double avg_ticket     = segments > 0 ? total_ticket_price / segments : 0;
    double avg_occupancy  = segments > 0 ? total_occupancy / segments : 0;
    double net_benefit    = total_income - total_cost;

    route_data["rentabilidad"] = json::object();
    route_data["rentabilidad"]["ingreso_total"]          = total_income;
    route_data["rentabilidad"]["pasajeros_promedio"]     = avg_passengers;
    route_data["rentabilidad"]["precio_promedio_boleto"] = avg_ticket;
    route_data["rentabilidad"]["ocupacion_porcentaje"]   = avg_occupancy;
    route_data["rentabilidad"]["beneficio_neto"]         = net_benefit;
    route_data["rentabilidad"]["es_rentable"]            = net_benefit > 0;

    // Guardar a archivo
    ofstream file(filename);
    file << route_data.dump(4);
    file.close();

    cout << "[✓] Resultado guardado en " << filename << endl;
}