#include <limits>
#include <vector>
#include "pathFinding.h"
#include "m4.h"
#include "globalHeader.h"
#include "m4Functions.h"

std::vector<CourierSubPath>
travelingCourier(const std::vector<DeliveryInf> &deliveries, const std::vector<int> &depots, const float turn_penalty) {
    std::vector<CourierSubPath> completePath; //will hold the final path;
    std::vector<StreetSegmentIdx> partialPath; //holds path pieces

    std::vector<deliveryStop> stops; //holds stops that need to be made

    std::vector<int> pickUps;
    std::vector<int> previousPickups;

    struct CourierSubPath partialDeliveryPath; //holds partial delivery path before it gets added to the complete path

    int previousIntersection = NOT_DELIVERY_NODE;
    int startDepot = findClosestPickUp(deliveries, depots), endDepot = depots[0];
    int intersectionFound;

    std::string prevStop;
    std::string currentStop;

    bool duplicate = false;
    bool invalidPath = false;

    //add all deliveries into stops vector as pickups
    for (auto deliveryIt = deliveries.begin();
         deliveryIt != deliveries.end(); deliveryIt++) {

        deliveryStop tempStop((*deliveryIt).pickUp, "pickup"); //create deliveryStop struct
        stops.push_back(tempStop);

    }

    while (!stops.empty()) { //cycle till all interesting intersections reached

        if (previousIntersection == NOT_DELIVERY_NODE) { //first pickup, .: previous has to be a depot
            partialPath = findPathDK(stops, startDepot, turn_penalty); //find path from starting depot to closest pickup
            //if path does not existk
            if (partialPath.empty() && (pathFound == false)) { //ERROR CHECKING IN CASE NO PATH FOUND
                invalidPath = true;
            }

            if(invalidPath){
                break;
            }else{
                pathFound = false;
            }


            intersectionFound = interVisited.back().intersection;//the closest interesting intersection fould
            currentStop = interVisited.back().type; //pickup or dropoff

            partialDeliveryPath = {startDepot, intersectionFound, partialPath};
            completePath.push_back(partialDeliveryPath);

            //adding pickups dropoff into stop vector
            
            for (auto deliveryIt = deliveries.begin();
                 deliveryIt != deliveries.end(); ++deliveryIt) {
                if (deliveryIt->pickUp ==
                    intersectionFound) {//found the intersection that was found from the desired stops
                    deliveryStop tempDropOff((*deliveryIt).dropOff, "dropoff");   //create dropoff struct

                    duplicate = false;//reset flag
                    for (auto it = stops.begin(); it !=
                                                                                 stops.end(); ++it) {//going through stops and making sure the delivery stop isnt already adeed into the stops
                        if (it->intersection == deliveryIt->dropOff) {
                            duplicate = true;
                            break;//leaves if its already added
                        }
                    }

                    if (!duplicate) { //if not duplicated
                        stops.push_back(tempDropOff); // add dropOff into the stops
                        duplicate = false;//reset flag
                    }
                }
            }
        } else {//not the first stop
            partialPath = findPathDK(stops, previousIntersection, turn_penalty);//find next stop

            if (partialPath.empty() && (pathFound == false)) {//ERROR CHECKING
                invalidPath = true;
            }

            if(invalidPath){
                break;
            }else{
                pathFound = false;
            }

            intersectionFound = interVisited.back().intersection;//get intersection that was reached by djikstra
            currentStop = interVisited.back().type;

            if (currentStop == "pickup") {
                for (std::vector<DeliveryInf>::const_iterator deliveryIt = deliveries.begin();
                     deliveryIt != deliveries.end(); ++deliveryIt) {
                    if (deliveryIt->pickUp == intersectionFound) {
                        deliveryStop tempStop((*deliveryIt).dropOff, "dropoff");
                        duplicate = false;
                        for (std::vector<deliveryStop>::iterator it = stops.begin(); it != stops.end(); it++) {
                            if (it->intersection == deliveryIt->dropOff) {
                                duplicate = true;
                            }
                        }
                        if (!duplicate) {
                            stops.push_back(tempStop);
                            duplicate = false;//clear flag
                        }
                    }
                }
            }

            //depending on whether it was a pick up or drop off, pickUpIndices has been updated and path is pushed
            partialDeliveryPath = {previousIntersection, intersectionFound, partialPath};
            completePath.push_back(partialDeliveryPath);
        }

        //update previous intersect id to the last intersection travelled to
        previousIntersection = intersectionFound;

        //keep track of previous string type (pick up or drop off)
        prevStop = currentStop;
        
        //remove the intersection reached from pickUpDropOffLocations
        for (auto stopIt = stops.begin(); stopIt != stops.end(); ++stopIt) {
            if (stopIt->intersection == intersectionFound) {
                stops.erase(stopIt);
                break;
            }
        }
    }

    if (!invalidPath) {
        endDepot = findClosestDepot(depots, previousIntersection, turn_penalty);
        partialPath = findPathBetweenIntersections(previousIntersection, endDepot, turn_penalty);
        if (!partialPath.empty()) {
            partialDeliveryPath = {previousIntersection, endDepot, partialPath};

            completePath.push_back(partialDeliveryPath);

            return completePath;
        } else {
            return std::vector<CourierSubPath>();
        }
    } else {
        return std::vector<CourierSubPath>();
    }
}

double findClosestPickUp(const std::vector<DeliveryInf> &deliveries, const std::vector<int> &depots){
    int closestDepot = depots[0];
    int smallestDistance = std::numeric_limits<int>::max();
    for(int i = 0; i < depots.size(); i++){
        LatLon depotPos = getIntersectionPosition(depots[i]);
        for(int j = 0; j < deliveries.size(); j++){
            LatLon pickUpPos = getIntersectionPosition(deliveries[j].pickUp);
            std::pair<LatLon, LatLon> distancePair (depotPos,pickUpPos);

            double distance = findDistanceBetweenTwoPoints(distancePair);
            if(distance < smallestDistance){
                smallestDistance = distance;
                closestDepot = depots[i];
            }
        }
    }
    return closestDepot;
}

int findClosestDepot(const std::vector<int>& depots, int intersection, float turn_penalty){
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