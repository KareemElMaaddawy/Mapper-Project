#include <limits>
#include <vector>
#include "pathFinding.h"
#include "m4.h"
#include "m4Functions.h"

int findClosestDepot(std::vector<int>& depots, int intersection, float turn_penalty){
    int closestDepot = -1;
    int smallestDistance = std::numeric_limits<int>::max();

    for(int i = 0; i < depots.size(); i++){
        int iterDistance = computePathTravelTime(findPathBetweenIntersections(depots[i], intersection, turn_penalty), turn_penalty);

        if(iterDistance < smallestDistance){
            smallestDistance = iterDistance;
            closestDepot = depots[i];
        }
    }

    return closestDepot;
}

DeliveryOrder loadDeliveryOrder(std::vector<DeliveryInf> deliveries, std::vector<int> depots, float turn_penalty){
    DeliveryOrder order;

    order.turnPenalty = turn_penalty;
    order.numberOfStops = deliveries.size()*2 + 2;
    order.deliveryOrder.resize(order.numberOfStops);

    order.startingDepot = findClosestDepot(depots, deliveries[0].pickUp, order.turnPenalty);
    order.endingDepot = findClosestDepot(depots, deliveries[deliveries.size()-1].dropOff, order.turnPenalty);

    for(int i = 0; i < order.deliveryOrder.size()-2; i+=2){
        for(int j = 0; j < 2; j++){
            if(j == 0){
                order.deliveryOrder[i+j] = deliveries[i].pickUp;
            }else{
                order.deliveryOrder[i+j] = deliveries[i].dropOff;
            }

        }
    }

    order.deliveryOrder.insert(order.deliveryOrder.begin(), order.startingDepot);
    order.deliveryOrder.insert(order.deliveryOrder.end(), order.endingDepot);

    return order;
}

std::vector<CourierSubPath> findPaths(DeliveryOrder order){
    std::vector<CourierSubPath> path;
    path.resize(order.numberOfStops-1);

    for(int i = 0; i < order.numberOfStops - 1; i++){
        path[i].start_intersection = order.deliveryOrder[i];
        path[i].end_intersection = order.deliveryOrder[i+1];
        path[i].subpath = findPathBetweenIntersections(order.deliveryOrder[i], order.deliveryOrder[i+1], order.turnPenalty);
    }

    return path;
}

double computeDeliveryTravelTime(DeliveryOrder order){
    int time = 0;
    for(int i = 0; i < order.numberOfStops-1; i++){
        time += computePathTravelTime(findPathBetweenIntersections(order.deliveryOrder[i], order.deliveryOrder[i+1], order.turnPenalty), order.turnPenalty);
    }
    return time;
}

std::vector<CourierSubPath> travelingCourier(
        const std::vector<DeliveryInf>& deliveries,
        const std::vector<int>& depots,
        const float turn_penalty){
    DeliveryOrder order = loadDeliveryOrder(deliveries, depots, turn_penalty);
    return findPaths(order);
}

std::vector<CourierSubPath> travelingCourier(
		            const std::vector<DeliveryInf>& deliveries,
	       	        const std::vector<int>& depots, 
		            const float turn_penalty){
    
    
    std::vector<int> interestingIntersections;
                      
    for(int i = 0; i < deliveries.size(); i++){
        int pickUpID = deliveries[i].pickUp;
        interestingIntersections.push_back(pickUpID);
    }
    
    std::vector<CourierSubPath> entirePath;
    std::vector<int> partPath;
    bool wrongPath = false;

    std::vector<int> pickUpNodes;
    std::vector<int> prevPickUpNodes;
    std::vector<int> pickedUp;
    struct CourierSubpath deliveryPath;


    int firstDepot = depots[0];
    int lastDepot = depots[0];
    int nodeFound;
    int deliveryNode;
    int previousIntersecID = -1;

    while(interestingIntersections.empty() == false){
        
        if(previousIntersecID == -1){
            
        partPath = find_path_dijkstra(firstDepot, interestingIntersections, turn_penalty);
        
        if(partPath.empty() && (djisktra == false)){
            wrongPath = true;
            break;
        }
        
        nodeFound = intersectionsReached.back();
        deliveryPath = {firstDepot, nodeFound, partPath};
        entirePath.push_back(deliveryPath);
        
        
        for(int j = 0; j < deliveries.size(); j++){
            int dropOffID = deliveries[j].dropOff;
            interestingIntersections.push_back(dropOffID);
        }
    }
        
        else{
            partPath = find_path_dijkstra(previousIntersecID, interestingIntersections, turn_penalty);
            if(parthPath.empty() && (find_path_djikstra_bool == false)){
                wrongPath = true;
                break;
            }
            
            nodeFound = intersectionsReached.back();
        }
     
 }
}


