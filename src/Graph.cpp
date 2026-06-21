#include "../include/Graph.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

Graph::Graph() {}

void Graph::loadAirports(string json_file){
    
    //Leemos el archivo JSON
    ifstream file(json_file);
    if(!file.is_open()){
        cerr << "Error al abrir el archivo: " << json_file << endl;
        return;
    }

    //Parseamos el JSON
    json airports_data = json::parse(file);
    file.close();

    //Para cada aeropuerto en el JSON, creamos un objeto Airport y lo almacenamos
    for(auto& airport_obj : airports_data){
        string code = airport_obj["iata"];
        string name = airport_obj["name"];
        string country = airport_obj["country"];
        double lat = airport_obj["latitude"];
        double lon = airport_obj["longitude"];
        double landing = airport_obj["landing_fee"];
        double handling = airport_obj["handling_fee"];

        //Crearmos objeto Airport
        Airport* airport = new Airport(code, name, country, lat, lon, landing, handling);
        airports.push_back(airport);
        codeToIndex[code] = airports.size() - 1; // Mapeamos el código al índice
    }

    //Inicializamos las matrices de distancias y rutas
    int n = airports.size();
    distance_matrix.assign(n, vector<double>(n, 0.0));
    route_matrix.assign(n, vector<Route*>(n, nullptr));

     cout << "[✓] Cargados " << n << " aeropuertos" << endl;
}

void Graph::addRoute(Airport* orig, Airport* dest, double distance){
    int i = codeToIndex[orig->codeIATA];
    int j = codeToIndex[dest->codeIATA];

    Route* route = new Route(orig, dest, distance, true);

    route_matrix[i][j] = route;
    distance_matrix[i][j] = distance;
}

Airport* Graph::getAirportByIndex(int index){
    if(index >= 0 && index < (int)airports.size()){
        return airports[index];
    }
    return nullptr; // Si no se encuentra el aeropuerto
}
Airport* Graph::getAirport(string code){
    if(codeToIndex.find(code) != codeToIndex.end()){
        return airports[codeToIndex[code]];
    }
    return nullptr; // Si no se encuentra el aeropuerto
}

Route* Graph::getRoute(int from_index, int to_index){
    if(from_index >= 0 && from_index < (int)airports.size() && 
       to_index >= 0 && to_index < (int)airports.size()){
        return route_matrix[from_index][to_index];
    }
    return nullptr;
}

Route* Graph::getRoute(string from_code, string to_code) {
    int i = getAirportIndex(from_code);
    int j = getAirportIndex(to_code);
    return getRoute(i, j);
}

vector<Route*> Graph::getOutgoingRoutes(int airport_index){
    vector<Route*> outgoing;
    if(airport_index >= 0 && airport_index < (int)airports.size()){
        for(int j = 0; j < (int)airports.size(); j++){
            if(route_matrix[airport_index][j] != nullptr){
                outgoing.push_back(route_matrix[airport_index][j]);
            }
        }
    }
    return outgoing;
}

vector<Route*> Graph::getOutgoingRoutes(string airport_code){
    int index = getAirportIndex(airport_code);
    return getOutgoingRoutes(index);
}

int Graph::getAirportCount(){
    return airports.size();
}

int Graph::getAirportIndex(string code){
    if(codeToIndex.find(code) != codeToIndex.end()){
        return codeToIndex[code];
    }
    return -1; // Si no se encuentra el aeropuerto
}

void Graph::displayAirports(){
    for(auto airport : airports){
        cout << airport->codeIATA << " - " << airport->name << " (" << airport->country << ")" << endl;
    }
}

void Graph::displayMatrix(){
    int n = airports.size();
    cout << "Matriz de Distancias:" << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << distance_matrix[i][j] << "\t";
        }
        cout << endl;
    }
}