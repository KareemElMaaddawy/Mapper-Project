#ifndef MAPPER_M4FUNCTIONS_H
#define MAPPER_M4FUNCTIONS_H

#include <limits>
#include <vector>

struct DeliveryOrder{
    int startingDepot;
    int endingDepot;
    int numberOfStops;
    std::vector<int> &deliveryOrder;
    float turnPenalty;
    DeliveryOrder();
};

int findClosestDepot(std::vector<int>& depots, int intersection, float turn_penalty);

DeliveryOrder loadDeliveryOrder(std::vector<DeliveryInf>& deliveries, std::vector<int>& depots, float turn_penalty);

std::vector<CourierSubPath> findPaths(DeliveryOrder order);

double computeDeliveryTravelTime(DeliveryOrder order);


#endif //MAPPER_M4FUNCTIONS_H
