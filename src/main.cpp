#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "Aircraft.h"
#include "Airport.h"
#include "Route.h"
#include "CostModel.h"
#include "Graph.h"
#include "PathFinder.h"
#include "Dijkstra.h"
#include "BFS.h"
#include "DFS.h"
#include "Montecarlo.h"
#include "Bellmanford.h"

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    
    // VALIDAR ARGUMENTOS
    if (argc < 4 || argc > 5) {
        cerr << "ERROR: Argumentos incorrectos" << endl;
        cerr << "Uso: ./eurosky_test <origen> <destino> <max_horas> [algoritmo]" << endl;
        cerr << "\nAlgoritmos disponibles:" << endl;
        cerr << "  - dijkstra (default, óptimo)" << endl;
        cerr << "  - bfs (ruta más corta en hops)" << endl;
        cerr << "  - dfs (búsqueda en profundidad)" << endl;
        cerr << "  - montecarlo (probabilístico, 1000 iteraciones)" << endl;
        cerr << "  - bellmanford (relajación progresiva)" << endl;
        cerr << "\nEjemplos:" << endl;
        cerr << "  ./eurosky_test MAD AMS 8.0" << endl;
        cerr << "  ./eurosky_test MAD AMS 8.0 dijkstra" << endl;
        cerr << "  ./eurosky_test MAD AMS 8.0 montecarlo" << endl;
        return 1;
    }
    
    string origen = argv[1];
    string destino = argv[2];
    double max_horas = stod(argv[3]);
    string algoritmo = (argc == 5) ? argv[4] : "dijkstra";
    
    // Convertir a minúsculas
    transform(algoritmo.begin(), algoritmo.end(), algoritmo.begin(), ::tolower);
    
    cout << "═══════════════════════════════════════════════════════" << endl;
    cout << "=== EuroSky Connect - Route Optimizer ===" << endl;
    cout << "═══════════════════════════════════════════════════════" << endl;
    cout << "Ruta: " << origen << " → " << destino << endl;
    cout << "Tiempo máximo: " << max_horas << " horas" << endl;
    cout << "Algoritmo: " << algoritmo << endl << endl;
    
    // ========================================
    // [1] CARGAR AVIONES DESDE JSON
    // ========================================
    cout << "[1] Cargando aviones desde aircraft.json..." << endl;
    
    ifstream aircraftFile("data/aircraft.json");
    if (!aircraftFile.is_open()) {
        cerr << "Error: No se puede abrir data/aircraft.json" << endl;
        return 1;
    }
    
    json aircraftData = json::parse(aircraftFile);
    aircraftFile.close();
    
    vector<Aircraft*> aircrafts;
    for (auto& ac : aircraftData) {
        Aircraft* aircraft = new Aircraft(
            ac["model_name"],
            ac["capacity_passengers"],
            ac["fuel_consumption_per_hour"],
            ac["cruise_speed_kmh"],
            ac["max_range_km"],
            ac["crew_cost_per_hour"],
            ac["maintenance_cost_per_hour"]
        );
        aircrafts.push_back(aircraft);
    }
    
    cout << "[✓] " << aircrafts.size() << " aviones cargados" << endl << endl;
    
    // ========================================
    // [2] CARGAR GRAPH DESDE airports.json
    // ========================================
    cout << "[2] Cargando aeropuertos..." << endl;
    Graph graph;
    graph.loadAirports("data/airports.json");
    cout << "[✓] " << graph.getAirportCount() << " aeropuertos cargados" << endl << endl;
    
    // ========================================
    // [3] ASIGNAR AVIONES A AEROPUERTOS
    // ========================================
    cout << "[3] Asignando aviones a aeropuertos..." << endl;
    
    vector<string> airportCodes = {
        "MAD", "BCN", "AMS", "CDG", "LHR", "FRA", "VIE", "ZRH", 
        "MXP", "PRG", "WAW", "BUD", "BRU", "DUB", "FCO", "VLC", 
        "SVQ", "AGP", "LIS", "PMI", "IBZ", "BIO", "HAM", "CPH", 
        "ARN", "STR", "MUC", "GVA", "KRK", "OTP", "SOF", "ATH", 
        "NAP", "VCE", "ALC", "ORY"
    };
    
    for (string code : airportCodes) {
        Airport* apt = graph.getAirport(code);
        if (apt) {
            if (code == "MAD" || code == "BCN" || code == "CDG" || code == "LHR" || code == "AMS") {
                for (Aircraft* ac : aircrafts) {
                    apt->addAircraft(ac);
                }
            }
            else if (code == "FRA" || code == "VIE" || code == "MXP" || code == "PRG" || code == "BRU") {
                for (int i = 0; i < min(10, (int)aircrafts.size()); i++) {
                    apt->addAircraft(aircrafts[i]);
                }
            }
            else {
                for (int i = 0; i < min(6, (int)aircrafts.size()); i++) {
                    apt->addAircraft(aircrafts[i]);
                }
            }
        }
    }
    
    cout << "[✓] Aviones asignados a aeropuertos" << endl << endl;
    
    // ========================================
    // [4] CARGAR RUTAS DESDE routes.json
    // ========================================
    cout << "[4] Cargando rutas..." << endl;
    
    ifstream routesFile("data/routes.json");
    if (!routesFile.is_open()) {
        cerr << "Error: No se puede abrir data/routes.json" << endl;
        return 1;
    }
    
    json routes = json::parse(routesFile);
    routesFile.close();
    
    int routesAdded = 0;
    for (auto& route : routes) {
        string from = route["origin"];
        string to = route["destination"];
        double dist = route["distance"];
        
        Airport* fromAirport = graph.getAirport(from);
        Airport* toAirport = graph.getAirport(to);
        
        if (fromAirport && toAirport) {
            graph.addRoute(fromAirport, toAirport, dist);
            routesAdded++;
        }
    }
    
    cout << "[✓] " << routesAdded << " rutas cargadas" << endl << endl;
    
    // ========================================
    // [5] CREAR MODELO DE COSTOS
    // ========================================
    cout << "[5] Inicializando modelo de costos..." << endl;
    CostModel cost_model(1.5);
    cout << "[✓] Modelo de costos inicializado" << endl << endl;
    
    // ========================================
    // [6] SELECCIONAR Y EJECUTAR ALGORITMO
    // ========================================
    cout << "[6] Ejecutando algoritmo: " << algoritmo << endl;
    cout << "═══════════════════════════════════════════════════════" << endl;
    
    PathFinder* finder = nullptr;
    
    if (algoritmo == "dijkstra") {
        finder = new Dijkstra(&graph, &cost_model);
    }
    else if (algoritmo == "bfs") {
        finder = new BFSFinder(&graph, &cost_model);
    }
    else if (algoritmo == "dfs") {
        finder = new DFSFinder(&graph, &cost_model);
    }
    else if (algoritmo == "montecarlo") {
        finder = new MonteCarloFinder(&graph, &cost_model, 1000);
    }
    else if (algoritmo == "bellmanford") {
        finder = new BellmanFordFinder(&graph, &cost_model);
    }
    else {
        cerr << "Error: Algoritmo desconocido: " << algoritmo << endl;
        return 1;
    }
    
    if (finder->findRoute(origen, destino, max_horas)) {
        cout << "\n✓ RUTA ENCONTRADA:" << endl;
        vector<string> path = finder->getPath();
        cout << "Ruta: ";
        for (int i = 0; i < (int)path.size(); i++) {
            cout << path[i];
            if (i < (int)path.size() - 1) cout << " → ";
        }
        cout << endl;
        cout << "Costo total: €" << finder->getTotalCost() << endl;
        cout << "Tiempo total: " << finder->getTotalTime() << "h" << endl;
        cout << "Algoritmo usado: " << finder->getName() << endl;
        
        finder->saveRouteToJSON("resultados_ruta.json");
        
    } else {
        cout << "\n✗ NO HAY RUTA VÁLIDA" << endl;
        cout << "No se encontró ruta de " << origen << " a " << destino;
        cout << " dentro de " << max_horas << " horas." << endl;

        // Guardar JSON con error para que el frontend lo maneje correctamente
        ofstream errorFile("resultados_ruta.json");
        errorFile << "{\n";
        errorFile << "  \"error\": true,\n";
        errorFile << "  \"mensaje\": \"No se encontro ruta de " << origen << " a " << destino << " dentro de " << max_horas << " horas\",\n";
        errorFile << "  \"origen\": \"" << origen << "\",\n";
        errorFile << "  \"destino\": \"" << destino << "\",\n";
        errorFile << "  \"algoritmo\": \"" << algoritmo << "\",\n";
        errorFile << "  \"ruta\": [],\n";
        errorFile << "  \"aeropuertos\": []\n";
        errorFile << "}\n";
        errorFile.close();
        cout << "[✓] Resultado (sin ruta) guardado en resultados_ruta.json" << endl;
    }
    
    cout << "═══════════════════════════════════════════════════════" << endl;
    cout << "\n=== FIN ===" << endl;
    
    // LIBERAR MEMORIA
    delete finder;
    for (Aircraft* ac : aircrafts) {
        delete ac;
    }
    
    return 0;
}