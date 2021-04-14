#include <limits>
#include <vector>
#include "pathFinding.h"
#include "m4.h"

struct DeliveryOrder{
    int startingDepot;
    int endingDepot;
    int numberOfStops;
    std::vector<int> &deliveryOrder;
    float turnPenalty;
    DeliveryOrder();
};

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

DeliveryOrder loadDeliveryOrder(std::vector<DeliveryInf>& deliveries, std::vector<int>& depots, float turn_penalty){
    DeliveryOrder order;

    order.turnPenalty = turn_penalty;
    order.numberOfStops = deliveries.size()*2 + 2;
    order.deliveryOrder.resize(order.numberOfStops);

    order.startingDepot = findClosestDepot(depots, deliveries[0].pickUp, order.turnPenalty);
    order.endingDepot = findClosestDepot(depots, deliveries[deliveries.size()-1].dropOff, order.turnPenalty);

    for(int i = 0; i < order.deliveryOrder.size(); i+=2){
        if(i == 0){
            order.deliveryOrder[i] = order.startingDepot;
        }else if(i == order.numberOfStops-1){
            order.deliveryOrder[i] = order.endingDepot;
        }else{
            order.deliveryOrder[i] = deliveries[i].pickUp;
            order.deliveryOrder[i+1] = deliveries[i].dropOff;
        }
    }
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
