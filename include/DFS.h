#ifndef DFSFINDER_H
#define DFSFINDER_H

#include "PathFinder.h"
#include <stack>

class DFSFinder : public PathFinder {
private:
    vector<int> parent;
    vector<bool> visited;
    
    void dfs(int current, int destination, double accumulated_time, 
             double accumulated_cost, double max_hours);
    
public:
    DFSFinder(Graph* g, CostModel* cm) : PathFinder(g, cm) {}
    
    bool findRoute(string origin, string destination, double max_hours) override;
    string getName() override { return "DFS"; }
};

#endif