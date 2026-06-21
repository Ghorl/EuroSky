#include "BFS.h"
#include <queue>
#include <iostream>
#include <algorithm>
bool BFSFinder::findRoute(string origin, string destination, double max_hours) {
    int origin_idx = graph->getAirportIndex(origin);
    int dest_idx = graph->getAirportIndex(destination);
    
    if (origin_idx == -1 || dest_idx == -1) {
        return false;
    }
    
    int n = graph->getAirportCount();
    visited.assign(n, false);
    parent.assign(n, -1);
    
    queue<pair<int, double>> q; // (airport_index, accumulated_time)
    q.push({origin_idx, 0});
    visited[origin_idx] = true;
    
    while (!q.empty()) {
        auto [current, time_acc] = q.front();
        q.pop();
        
        if (current == dest_idx) {
            // Reconstruir ruta
            path.clear();
            int node = dest_idx;
            while (node != -1) {
                path.push_back(graph->getAirportByIndex(node)->codeIATA);
                node = parent[node];
            }
            reverse(path.begin(), path.end());
            
            // Calcular costo total usando el avión más barato por segmento
            total_cost = 0;
            total_time = 0;
            for (int i = 0; i < (int)path.size() - 1; i++) {
                Route* route = graph->getRoute(path[i], path[i+1]);
                if (route) {
                    Aircraft* best = nullptr;
                    double best_c = 1e9;
                    for (Aircraft* ac : route->origin->available_aircraft) {
                        double c = cost_model->calculateTotalCost(route, ac);
                        if (c != -1 && c < best_c) { best_c = c; best = ac; }
                    }
                    if (best) {
                        total_cost += best_c;
                        total_time += route->getFlightTime(best);
                        aircraft_choice = best->model_name;
                    }
                }
            }
            return true;
        }
        
        vector<Route*> outgoing = graph->getOutgoingRoutes(current);

        for (Route* route : outgoing) {
            int next = graph->getAirportIndex(route->destination->codeIATA);

            // Seleccionar el avión más barato que pueda volar esta ruta
            Aircraft* best_aircraft = nullptr;
            double best_cost = 1e9;
            for (Aircraft* ac : route->origin->available_aircraft) {
                double c = cost_model->calculateTotalCost(route, ac);
                if (c != -1 && c < best_cost) {
                    best_cost = c;
                    best_aircraft = ac;
                }
            }

            if (best_aircraft) {
                double new_time = time_acc + route->getFlightTime(best_aircraft);
                if (!visited[next] && new_time <= max_hours) {
                    visited[next] = true;
                    parent[next] = current;
                    q.push({next, new_time});
                }
            }
        }
    }

    return false;
}