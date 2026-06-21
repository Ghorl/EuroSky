#ifndef BELLMANFORDFINDER_H
#define BELLMANFORDFINDER_H

#include "PathFinder.h"

class BellmanFordFinder : public PathFinder {
private:
    vector<double> distance;
    vector<int> parent;
    vector<double> time_accumulated;
    
public:
    BellmanFordFinder(Graph* g, CostModel* cm) : PathFinder(g, cm) {}
    
    bool findRoute(string origin, string destination, double max_hours) override;
    string getName() override { return "BellmanFord"; }
};

#endif