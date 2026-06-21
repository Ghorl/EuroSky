#ifndef COSTMODEL_H
#define COSTMODEL_H

class Route;
class Aircraft;

class CostModel {
    private:
       //atributos para los costos
        double fuel_price_per_liter;
    
    public:
        //Constructor para inicializar los costos
        CostModel(double fuel_price);
        
        //Metodos para calcular los costos independientemente de la ruta y el avión
        double calculateFuelCost(Route* route, Aircraft* aircraft);
        double calculateCrewCost(Route* route, Aircraft* aircraft);
        double calculateMaintenanceCost(Route* route, Aircraft* aircraft);
        double calculateLandingFee(Route* route);
        double calculateHandlingFee(Route* route);
        
        //Metodo final para calcular el costo total
        double calculateTotalCost(Route* route, Aircraft* aircraft);

        void printCostBreakdown(Route* route, Aircraft* aircraft);
};
#endif
