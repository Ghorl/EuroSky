#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <string>
#include <vector>
#include "Graph.h"
#include "CostModel.h"

using namespace std;

class PathFinder {
protected:
    Graph* graph;
    CostModel* cost_model;
    vector<string> path;
    double total_cost;
    double total_time;
    string aircraft_choice;
    
public:
    PathFinder(Graph* g, CostModel* cm) : graph(g), cost_model(cm), 
                                          total_cost(0), total_time(0) {}
    
    virtual ~PathFinder() = default;
    
    // Buscar ruta óptima
    virtual bool findRoute(string origin, string destination, double max_hours) = 0;
    
    // Getters
    virtual vector<string> getPath() { return path; }
    virtual double getTotalCost() { return total_cost; }
    virtual double getTotalTime() { return total_time; }
    virtual string getAircraftChoice() { return aircraft_choice; }
    
    // Salvar resultado a JSON
    virtual void saveRouteToJSON(string filename);
    
    // Nombre del algoritmo
    virtual string getName() = 0;
};

#endif