#include "Montecarlo.h"
#include <iostream>
#include <algorithm>

bool MonteCarloFinder::findRoute(string origin, string destination, double max_hours) {
    int origin_idx = graph->getAirportIndex(origin);
    int dest_idx = graph->getAirportIndex(destination);
    
    if (origin_idx == -1 || dest_idx == -1) {
        return false;
    }
    
    cout << "[MonteCarlo] Ejecutando " << iterations << " iteraciones..." << endl;
    
    best_cost = 1e9;
    path.clear();
    
    // Ejecutar múltiples búsquedas aleatorias
    for (int iter = 0; iter < iterations; iter++) {
        if (randomWalk(origin, destination, max_hours)) {
            if (total_cost < best_cost) {
                best_cost = total_cost;
                best_path = path;
                
                if (iter % 100 == 0 && iter > 0) {
                    cout << "  Iteración " << iter << ": Mejor costo = €" 
                         << best_cost << endl;
                }
            }
        }
    }
    
    if (best_cost < 1e9) {
        path = best_path;
        total_cost = best_cost;
        cout << "[✓] MonteCarlo encontró ruta: €" << best_cost << endl;
        return true;
    }
    
    return false;
}

bool MonteCarloFinder::randomWalk(string origin, string destination, double max_hours) {
    vector<string> current_path;
    double accumulated_time = 0;
    double accumulated_cost = 0;
    
    string current = origin;
    current_path.push_back(current);
    
    int max_steps = 50; // Límite de pasos para evitar ciclos infinitos
    int steps = 0;
    
    while (current != destination && steps < max_steps) {
        int current_idx = graph->getAirportIndex(current);
        vector<Route*> outgoing = graph->getOutgoingRoutes(current_idx);
        
        if (outgoing.empty()) {
            return false; // No hay salidas, camino fallido
        }
        
        // Elegir ruta aleatoria
        int random_idx = rand() % outgoing.size();
        Route* chosen_route = outgoing[random_idx];
        
        // Seleccionar el avión más barato que pueda volar esta ruta
        Aircraft* aircraft = nullptr;
        double best_cost = 1e9;
        for (Aircraft* ac : chosen_route->origin->available_aircraft) {
            double c = cost_model->calculateTotalCost(chosen_route, ac);
            if (c != -1 && c < best_cost) {
                best_cost = c;
                aircraft = ac;
            }
        }
        if (!aircraft) return false;

        double flight_time = chosen_route->getFlightTime(aircraft);
        double flight_cost = best_cost;
        
        if (accumulated_time + flight_time > max_hours) {
            return false; // Excede tiempo máximo
        }
        
        current = chosen_route->destination->codeIATA;
        current_path.push_back(current);
        accumulated_time += flight_time;
        accumulated_cost += flight_cost;
        steps++;
    }
    
    if (current == destination) {
        path = current_path;
        total_cost = accumulated_cost;
        total_time = accumulated_time;
        return true;
    }
    
    return false;
}