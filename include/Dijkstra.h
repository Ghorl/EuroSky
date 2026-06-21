#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "PathFinder.h"
#include "Graph.h"
#include "CostModel.h"
#include <vector>
#include <limits>

class Dijkstra : public PathFinder {

    private:
        //Estado durante ejecución
        vector<double> distance; //Costo minimo a cada nodo
        vector<double> time_accumulated; //Tiempo acumulado
        vector<int> parent; //De donde viene cada nodo
        vector<Aircraft*> aircraft_choice_vec; //Que avión se eligió para llegar a cada nodo
        vector<bool> visited; //Si el nodo ya fue visitado
        int dest_index; //Índice del destino para reconstruir ruta al final
        int origin_index; //Índice del origen para reconstruir ruta al final
    
    public:
        Dijkstra(Graph* g, CostModel* cm);

        //Ejecuta Dijkstra
        bool findRoute(string origin, string destination, double max_flight_hours) override;
        
        // Métodos virtuales heredados
        string getName() override { return "Dijkstra"; }
};

#endif