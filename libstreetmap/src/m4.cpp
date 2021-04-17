#include <limits>
#include <vector>
#include "pathFinding.h"
#include "m4.h"
#include "globalHeader.h"
#include "m4Functions.h"

std::vector<CourierSubPath> travelingCourier(
        const std::vector<DeliveryInf> &deliveries,
        const std::vector<int> &depots,
        const float turn_penalty) {
    
    bool duplicate = false;
    std::vector<deliveryStop> interestingIntersections;
    
    for(int i = 0; i < deliveries.size(); i++){
        deliveryStop order(deliveries[i].pickUp, "pickUp");
        
        for(int j = 0; j < interestingIntersections.size(); j++){
            if(interestingIntersections[j].intersection == deliveries[i].pickUp){
                duplicate = true;
                break;
            }
        }
        
        if(!duplicate){
            interestingIntersections.push_back(order);
        }
    }
    
    bool wrongPath = false;
    
    std::vector<CourierSubPath> entirePath;
    std::vector<int> partPath;
    std::vector<int> deliveriesPickedUp;
    std::vector<deliveryStop> reVisit;
    
    int previousIntersection = -1;
    int firstDepot = depots[0];
    int lastDepot = depots[0];
    int nodeFound;
    
    struct CourierSubPath deliverySubPath;
    
    std::string pickUpDropOffPrevious, pickUpDropOffCurrent;
  
    while(interestingIntersections.empty() == false){
        if(previousIntersection == -1){
            partPath = findPathDK(interestingIntersections, firstDepot, turn_penalty);
            
            if((partPath.empty()) && (djikstra(interestingIntersections, getNodeFromId(firstDepot), turn_penalty) == false)){
                wrongPath = true;
                break;
            }
            nodeFound = interVisited.back().intersection;
            pickUpDropOffCurrent = interVisited.back().type;
            
            deliverySubPath = {firstDepot, nodeFound, partPath};
            entirePath.push_back(deliverySubPath);
            
            for(int i = 0; i < deliveries.size(); i++){
                if(deliveries[i].pickUp == nodeFound){
                    deliveryStop order(deliveries[i].dropOff, "dropOff");
                    duplicate = false;
                    for(int j = 0 ; j < interestingIntersections.size(); j++){
                       if(interestingIntersections[j].intersection == deliveries[i].dropOff){
                           duplicate = true;
                           break;
                       } 
                    }
                    
                    if(!duplicate){
                        interestingIntersections.push_back(order);
                        duplicate = false;
                    }
                }
            }
            
        }
    }
}

