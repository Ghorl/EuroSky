#include "Bellmanford.h"
#include <iostream>
#include <algorithm>

bool BellmanFordFinder::findRoute(string origin, string destination, double max_hours) {
    int origin_idx = graph->getAirportIndex(origin);
    int dest_idx = graph->getAirportIndex(destination);
    
    if (origin_idx == -1 || dest_idx == -1) {
        return false;
    }
    
    int n = graph->getAirportCount();
    distance.assign(n, 1e9);
    parent.assign(n, -1);
    time_accumulated.assign(n, 1e9);
    
    distance[origin_idx] = 0;
    time_accumulated[origin_idx] = 0;
    
    // Relajación (n-1) iteraciones
    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            if (distance[u] == 1e9) continue;
            
            vector<Route*> outgoing = graph->getOutgoingRoutes(u);
            
            for (Route* route : outgoing) {
                int v = graph->getAirportIndex(route->destination->codeIATA);
                
                if (!route->origin->available_aircraft.empty()) {
                    Aircraft* aircraft = route->origin->available_aircraft[0];
                    double new_time = time_accumulated[u] + route->getFlightTime(aircraft);
                    double new_cost = distance[u] + cost_model->calculateTotalCost(route, aircraft);
                    
                    if (new_time <= max_hours && new_cost < distance[v]) {
                        distance[v] = new_cost;
                        time_accumulated[v] = new_time;
                        parent[v] = u;
                    }
                }
            }
        }
    }
    
    // Reconstruir ruta
    if (distance[dest_idx] < 1e9) {
        path.clear();
        int node = dest_idx;
        while (node != -1) {
            path.push_back(graph->getAirportByIndex(node)->codeIATA);
            node = parent[node];
        }
        reverse(path.begin(), path.end());
        
        total_cost = distance[dest_idx];
        total_time = time_accumulated[dest_idx];
        
        // Obtener avión de la primera ruta
        if (!path.empty() && path.size() > 1) {
            Route* first_route = graph->getRoute(path[0], path[1]);
            if (first_route && !first_route->origin->available_aircraft.empty()) {
                aircraft_choice = first_route->origin->available_aircraft[0]->model_name;
            }
        }
        
        return true;
    }
    
    return false;
}