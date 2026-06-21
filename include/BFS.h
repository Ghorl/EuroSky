#ifndef BFSFINDER_H
#define BFSFINDER_H

#include "PathFinder.h"
#include <queue>

class BFSFinder : public PathFinder {
private:
    vector<int> parent;
    vector<bool> visited;
    
public:
    BFSFinder(Graph* g, CostModel* cm) : PathFinder(g, cm) {}
    
    bool findRoute(string origin, string destination, double max_hours) override;
    string getName() override { return "BFS"; }
};

#endif