#ifndef MONTECARLOFINDER_H
#define MONTECARLOFINDER_H

#include "PathFinder.h"
#include <random>
#include <ctime>

class MonteCarloFinder : public PathFinder {
private:
    int iterations;
    vector<string> best_path;
    double best_cost;
    
    // Búsqueda aleatoria única
    bool randomWalk(string origin, string destination, double max_hours);
    
public:
    MonteCarloFinder(Graph* g, CostModel* cm, int iter = 1000) 
        : PathFinder(g, cm), iterations(iter), best_cost(1e9) {
        srand(time(0));
    }
    
    bool findRoute(string origin, string destination, double max_hours) override;
    string getName() override { return "MonteCarlo"; }
    int getIterations() { return iterations; }
};

#endif