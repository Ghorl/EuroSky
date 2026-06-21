#include "CostModel.h"
#include <iostream>
#include "Route.h"
#include "Aircraft.h"
using namespace std;

CostModel::CostModel(double fuel_price) 
    : fuel_price_per_liter(fuel_price) {}

double CostModel::calculateFuelCost(Route* route, Aircraft* aircraft) {
    double flight_time_hours = route->getFlightTime(aircraft); 

    if(flight_time_hours == -1) {
        return -1;
    }
    else {
        double liters_consumed = flight_time_hours * aircraft->fuel_consumption_per_hour;
        double fuel_cost = liters_consumed * this->fuel_price_per_liter;
        return fuel_cost;
    }
}

double CostModel::calculateCrewCost(Route* route, Aircraft* aircraft) {
    double flight_time_hours = route->getFlightTime(aircraft); 

    if(flight_time_hours == -1) {
        return -1;
    }
    else {
        double crew_cost = flight_time_hours * aircraft->crew_cost_per_hour;
        return crew_cost;
    }
}

double CostModel::calculateMaintenanceCost(Route* route, Aircraft* aircraft) {
    double flight_time_hours = route->getFlightTime(aircraft); 

    if(flight_time_hours == -1) {
        return -1;
    }
    else {
        double maintenance_cost = flight_time_hours * aircraft->maintenance_cost_per_hour;
        return maintenance_cost;
    }
}

double CostModel::calculateLandingFee(Route* route) {
    return route->destination->landing_fee;
}

double CostModel::calculateHandlingFee(Route* route) {
    return route->destination->handling_fee;
}

double CostModel::calculateTotalCost(Route* route, Aircraft* aircraft) {
    double fuel_cost = calculateFuelCost(route, aircraft);
    double crew_cost = calculateCrewCost(route, aircraft);
    double maintenance_cost = calculateMaintenanceCost(route, aircraft);
    double landing_fee = calculateLandingFee(route);
    double handling_fee = calculateHandlingFee(route);

    if(fuel_cost == -1 || crew_cost == -1 || maintenance_cost == -1) {
        return -1;
    }
    else {
        return fuel_cost + crew_cost + maintenance_cost + landing_fee + handling_fee;
    }
}

double CostModel::calculateIncome(Route* route) {
    return route->expected_passengers * route->avg_ticket_price;
}

double CostModel::calculateNetBenefit(Route* route, Aircraft* aircraft) {
    double income = calculateIncome(route);
    double cost   = calculateTotalCost(route, aircraft);
    if (cost == -1) return -1;
    return income - cost;
}

double CostModel::calculateOccupancy(Route* route, Aircraft* aircraft) {
    if (aircraft->capacity_passengers <= 0) return 0;
    return (route->expected_passengers / aircraft->capacity_passengers) * 100.0;
}

void CostModel::printCostBreakdown(Route* route, Aircraft* aircraft) {
    cout << "\n=== DESGLOSE DE COSTOS ===" << endl;
    cout << "Ruta: " << route->origin->codeIATA 
         << " → " << route->destination->codeIATA << endl;
    cout << "Avión: " << aircraft->model_name << endl << endl;
    
    double fuel = calculateFuelCost(route, aircraft);
    double crew = calculateCrewCost(route, aircraft);
    double maint = calculateMaintenanceCost(route, aircraft);
    double landing = calculateLandingFee(route);
    double handling = calculateHandlingFee(route);
    
    cout << "Combustible:    €" << fuel << endl;
    cout << "Tripulación:    €" << crew << endl;
    cout << "Mantenimiento:  €" << maint << endl;
    cout << "Aterrizaje:     €" << landing << endl;
    cout << "Servicios:      €" << handling << endl;
    cout << "─────────────────────────" << endl;
    cout << "TOTAL:          €" << calculateTotalCost(route, aircraft) << endl;
    cout << "========================\n" << endl;
}