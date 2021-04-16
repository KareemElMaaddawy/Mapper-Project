#include "m4.h"
#include "pathFinding.h"





std::vector<CourierSubPath> travelingCourier(
		            const std::vector<DeliveryInf>& deliveries,
	       	        const std::vector<int>& depots, 
		            const float turn_penalty){
    
    
    std::vector<int> interestingIntersections;
                      
    for(int i = 0; i < deliveries.size(); i++){
        int pickUpID = deliveries[i].pickUp;
        interestingIntersections.push_back(pickUpID);
    }
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
        partPath = find_path_dijkstra(firstDepot, interestingIntersections, turn_penalty());
        
        if(partPath.empty() && (find_path_dijkstra_bool == false)){
            wrongPath = true;
            break;
        }
        
        nodeFound = intersectionsReached.back();
        deliveryPath = {firstDepot, nodeFound, partPath};
        entirePath.push_back(deliveryPath);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
