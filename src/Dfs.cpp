#include "DFS.h"
#include <iostream>
#include <algorithm>

bool DFSFinder::findRoute(string origin, string destination, double max_hours) {
    int origin_idx = graph->getAirportIndex(origin);
    int dest_idx = graph->getAirportIndex(destination);
    
    if (origin_idx == -1 || dest_idx == -1) {
        return false;
    }
    
    int n = graph->getAirportCount();
    visited.assign(n, false);
    parent.assign(n, -1);
    
    path.clear();
    total_cost = 1e9;
    
    dfs(origin_idx, dest_idx, 0, 0, max_hours);
    
    return (total_cost < 1e9);
}

void DFSFinder::dfs(int current, int destination, double accumulated_time,
                    double accumulated_cost, double max_hours) {
    
    visited[current] = true;
    
    if (current == destination) {
        // Encontró destino, reconstruir ruta
        vector<string> new_path;
        int node = destination;
        while (node != -1) {
            new_path.push_back(graph->getAirportByIndex(node)->codeIATA);
            node = parent[node];
        }
        reverse(new_path.begin(), new_path.end());
        
        if (accumulated_cost < total_cost) {
            path = new_path;
            total_cost = accumulated_cost;
            total_time = accumulated_time;
        }
        
        visited[current] = false;
        return;
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
            double new_time = accumulated_time + route->getFlightTime(best_aircraft);
            double new_cost = accumulated_cost + best_cost;

            if (!visited[next] && new_time <= max_hours) {
                parent[next] = current;
                dfs(next, destination, new_time, new_cost, max_hours);
            }
        }
    }
    
    visited[current] = false;
}