#include "Dijkstra.h"
#include <queue>
#include <iostream>
#include <algorithm>

Dijkstra::Dijkstra(Graph* g, CostModel* cm) 
    : PathFinder(g, cm), dest_index(-1), origin_index(-1) {}

bool Dijkstra::findRoute(string origin, string destination, double max_flight_hours) {
    
    origin_index = graph->getAirportIndex(origin);
    dest_index = graph->getAirportIndex(destination);
    
    if (origin_index == -1 || dest_index == -1) {
        return false;
    }
    
    int n = graph->getAirportCount();
    distance.assign(n, 1e9);
    time_accumulated.assign(n, 1e9);
    parent.assign(n, -1);
    aircraft_choice_vec.assign(n, nullptr);
    visited.assign(n, false);
    
    distance[origin_index] = 0;
    time_accumulated[origin_index] = 0;
    
    // Priority queue: (costo, indice_aeropuerto)
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, origin_index});
    
    while (!pq.empty()) {
        auto [current_cost, current] = pq.top();
        pq.pop();
        
        if (visited[current]) continue;
        visited[current] = true;
        
        if (current == dest_index) {
            break;
        }
        
        // Obtener rutas salientes desde el aeropuerto actual
        vector<Route*> outgoing = graph->getOutgoingRoutes(current);
        
        for (Route* route : outgoing) {
            int next = graph->getAirportIndex(route->destination->codeIATA);
            
            // Evaluar todos los aviones disponibles
            for (Aircraft* aircraft : route->origin->available_aircraft) {
                
                // Validar que el avión pueda hacer ese vuelo
                if (!aircraft->canFly(route->distance_km)) {
                    continue;
                }
                
                double flight_time = route->getFlightTime(aircraft);
                double new_time = time_accumulated[current] + flight_time;
                
                // Validar restricción de tiempo
                if (new_time > max_flight_hours) {
                    continue;
                }
                
                double flight_cost = cost_model->calculateTotalCost(route, aircraft);
                double new_cost = distance[current] + flight_cost;
                
                // Relajación: si encontró mejor camino, actualiza
                if (new_cost < distance[next]) {
                    distance[next] = new_cost;
                    time_accumulated[next] = new_time;
                    parent[next] = current;
                    aircraft_choice_vec[next] = aircraft;
                    
                    pq.push({new_cost, next});
                }
            }
        }
    }
    
    // Si encontró ruta
    if (distance[dest_index] < 1e9) {
        // Reconstruir ruta
        path.clear();
        int node = dest_index;
        while (node != -1) {
            path.push_back(graph->getAirportByIndex(node)->codeIATA);
            node = parent[node];
        }
        reverse(path.begin(), path.end());
        
        total_cost = distance[dest_index];
        total_time = time_accumulated[dest_index];
        
        // Obtener el avión seleccionado
        if (aircraft_choice_vec[dest_index]) {
            aircraft_choice = aircraft_choice_vec[dest_index]->model_name;
        }
        
        return true;
    }
    
    return false;
}