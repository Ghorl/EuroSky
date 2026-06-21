#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <map>
#include "Airport.h"
#include "Route.h"
using namespace std;

class Graph {
private:
    vector<Airport*> airports;   //Almacna los aeropuertos
    
    map<string, int> codeToIndex; // Mapea el código IATA al índice en el vector de aeropuertos

    vector<vector<double>> distance_matrix;  //Matriz de distancias entre aeropuertos
    vector<vector<Route*>> route_matrix;     //Matriz de rutas entre aeropuertos

    
public:
    Graph();
    
   //Carga de datos
   void loadAirports(string json_file);
   void addRoute(Airport* orig, Airport* dest, double distance);

   Airport* getAirportByIndex(int index);
   Airport* getAirport(string code);
   Route* getRoute(int from_index, int to_index);
   Route* getRoute(string from_code, string to_code);

   //Dijkstra
   vector<Route*> getOutgoingRoutes(int airport_index);
   vector<Route*> getOutgoingRoutes(string airport_code);
   
   int getAirportCount();
   int getAirportIndex(string code);
    
    // Debug
    void displayAirports();
    void displayMatrix();

};
#endif