#include "pathFinding.h"

bool pathFound;
std::vector<deliveryStop> interVisited;
double shortestTravelTime;
std::vector<Node*> visitedNodes;

double findSlope(Point p0, Point p1){
    return (p0.y - p1.y) / (p0.x - p1.y);
}

void resetNodes(){
    for(std::vector<Node*>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); it++){
        (*it) -> bestTime = std::numeric_limits<int>::max();
        //(*it)->bestTime = std::numeric_limits<int>::max();
    }
}

std::string findDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet){
    StreetSegmentInfo sourceInfo = getStreetSegmentInfo(sourceStreet);//get segment info for streets
    StreetSegmentInfo destInfo = getStreetSegmentInfo(destStreet);

    if (getStreetName(sourceInfo.streetID) != getStreetName(destInfo.streetID)) {//asssumes if streetID is the same no turn occurs
        LatLon sourceFrom = getIntersectionPosition(sourceInfo.from);
        LatLon sourceTo = getIntersectionPosition(sourceInfo.to);
        LatLon destFrom = getIntersectionPosition(destInfo.from);
        LatLon destTo = getIntersectionPosition(destInfo.to);

        LatLon intersectionPosition;
        LatLon source;
        LatLon dest;

        if (sourceFrom == destFrom) { //finding the points of the turn
            intersectionPosition = sourceFrom;
            source = sourceTo;
            dest = destTo;
        } else if (sourceFrom == destTo) {
            intersectionPosition = sourceFrom;
            source = sourceTo;
            dest = destFrom;
        } else if (sourceTo == destFrom) {
            intersectionPosition = sourceTo;
            source = sourceFrom;
            dest = destTo;
        } else if (sourceTo == destTo) {
            intersectionPosition = sourceTo;
            source = sourceFrom;
            dest = destFrom;
        } else {
            std::cerr << "no overlap" << std::endl;//error checking
            return "error";
        }

        Point sourcePoint(source);
        Point intersectionPoint(intersectionPosition);
        Point destinationPoint(dest);

        bool result = findSlope(sourcePoint, intersectionPoint) > findSlope(sourcePoint, destinationPoint);
        if (!result) {//if angle is greater than 0 left turn
            return "left";
        } else {
            return "right";
        }
    } else {
        return "straight";
    }
}


//calculates turn penalty as the average of the speed limits around the source intersection and destination intersection
double calcTurnPenalty(IntersectionIdx source, IntersectionIdx dest) {

    int numOfSegment = 0;
    double speed = 0;
    std::vector<IntersectionIdx> sourceNeighbors = findAdjacentIntersections(source);
    std::vector<IntersectionIdx> destNeighbors = findAdjacentIntersections(dest);

    for(int sourceNeighbor : sourceNeighbors){//finding source speed limits
        StreetSegmentInfo temp = getStreetSegmentInfo(findSegmentBetweenIntersections(source, sourceNeighbor));
        speed += temp.speedLimit;
        numOfSegment++;
    }

    for(int destNeighbor : destNeighbors){//finding destination speed limits
        StreetSegmentInfo temp = getStreetSegmentInfo(findSegmentBetweenIntersections(dest, destNeighbor));
        speed += temp.speedLimit;
        numOfSegment++;
    }
    

    return  speed/numOfSegment;//returning average

}
//finds the travel time, given a path and a turn penalty
double computePathTravelTime(const std::vector<StreetSegmentIdx> &path, const double turn_penalty) {
    
    double abort = 0;
      #pragma omp parallel num_threads(4) 
{
        double travelTime = 0;
    std::string direction;
    for (int i = 0; i < path.size(); ++i) {
        if (i == 0) {//no need to check for left turn for the initial street segment
            travelTime += findStreetSegmentTravelTime(path[i]);
        } else {
            direction = findDirection(path[i - 1], path[i]);
            if (direction != "straight") {
                travelTime += findStreetSegmentTravelTime(path[i]) + turn_penalty;//if left turn occurs turn penalty is instituted
            } else {
                travelTime += findStreetSegmentTravelTime(path[i]);
            }
        }
    }
    abort = travelTime;
}
    return abort;
}
//finds the street segment between a pair of intersections
StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to) {
    int numOfStreetSegments = getNumIntersectionStreetSegment(from);
    for (int i = 0; i < numOfStreetSegments; ++i) {//loops through street segments belonging to an intersection till the opposite intersection matches the desired intersection
        StreetSegmentIdx streetSeg = getIntersectionStreetSegment(from, i);
        StreetSegmentInfo streetSegInfo = getStreetSegmentInfo(streetSeg);
        if (streetSegInfo.to == to || streetSegInfo.from == to) {
            return streetSeg;
        }
    }
    return -1;//error checking
}
//calculates the cost to travel to a node
double calculateCost(const double turnPenalty, const StreetSegmentIdx sourceSegment,
                     const StreetSegmentIdx destinationSegment) {
    double cost = 0;
    std::vector<StreetSegmentIdx> path = {sourceSegment, destinationSegment};
    cost += computePathTravelTime(path, turnPenalty);
    return cost;
}
//manhattan distance/segment speed
double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection, const double speed) {
    std::pair<double, double> currentPosition = {x_from_lon(getIntersectionPosition(currentIntersection).longitude()), y_from_lat(getIntersectionPosition(currentIntersection).latitude())};
    std::pair<double, double> destPosition = {x_from_lon(getIntersectionPosition(destinationIntersection).longitude()), y_from_lat(getIntersectionPosition(destinationIntersection).latitude())};

    return (abs(currentPosition.first - destPosition.first) + abs(currentPosition.second - destPosition.second))/(speed);//manhattan distance
}
//constructs a path, given a vector of intersections ids
std::vector<StreetSegmentIdx> reconstructPath(std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin,
                                              const IntersectionIdx intersect_id_destination,
                                              const IntersectionIdx intersect_id_start) {
    std::vector<StreetSegmentIdx> path;
    IntersectionIdx currentIntersection = intersect_id_destination;
    while (currentIntersection != intersect_id_start) { //loops till start is reached
        IntersectionIdx previousIntersection = pathOrigin[currentIntersection]; //finds where the interestion was reached from
        path.insert(path.begin(), findSegmentBetweenIntersections(previousIntersection, currentIntersection)); //finds the street segment in between and inserts into the path
        currentIntersection = previousIntersection; //steps back one intersection
    }
    return path;
}
//checks to make sure that illegal moves arent made
bool checkOneWay(IntersectionIdx source, IntersectionIdx dest){
    StreetSegmentInfo segInfo = getStreetSegmentInfo(findSegmentBetweenIntersections(source, dest));
    if(segInfo.oneWay){//if one way, then the source intersection has to be the "from" intersection
        if(source == segInfo.from){
            return true;
        }else{
            return false;
        }
    }else{//if not one way, anything goes
        return true;
    }
}

Node* getNodeFromId(IntersectionIdx id){
    return &nodes[id];
}

std::vector<StreetSegmentIdx> findPathDK(const std::vector<deliveryStop>& stops, const IntersectionIdx departurePoint, const double turn_penalty){
    std::vector<StreetSegmentIdx> path;
    Node* source = getNodeFromId(departurePoint);

    bool found = multiDestDjikstra(stops, source, turn_penalty);

    if(found){
        IntersectionIdx id = interVisited.back().intersection;
        path = traceback(id);
    }

    return path;
}


std::vector<StreetSegmentIdx> traceback(IntersectionIdx dest){
  
    std::vector<StreetSegmentIdx> path;

    Node* currentNode = getNodeFromId(dest);

    IntersectionIdx interId = dest;

    int reachingSegment = currentNode->reachingEdge;
    while(reachingSegment != NO_EDGE){
        path.insert(path.begin(),reachingSegment);
        StreetSegmentInfo segInfo = getStreetSegmentInfo(reachingSegment);

        if(segInfo.from == interId){
            currentNode = getNodeFromId(segInfo.to);
        }else{
            currentNode = getNodeFromId(segInfo.from);
        }

        interId = currentNode->id;
        reachingSegment = currentNode->reachingEdge;
    }
    return path;
}

bool multiDestDjikstra(std::vector<deliveryStop> stops, Node* source, const double turn_penalty) {
    std::priority_queue<prioElem, std::vector<prioElem>, compare> queueOfIntersections;
    queueOfIntersections.push(prioElem(source, NO_EDGE, NO_TIME));
    shortestTravelTime = 0;

    bool turn = false;

    while (!queueOfIntersections.empty()) {
        //std::cout << queueOfIntersections.size() << std::endl;
        prioElem elementOfInterest = queueOfIntersections.top();
        queueOfIntersections.pop();

        Node *currentNode = elementOfInterest.node;

        if (elementOfInterest.travelTime < currentNode->bestTime) {
            currentNode->bestTime = elementOfInterest.travelTime;
            visitedNodes.push_back(currentNode);
            currentNode->reachingEdge = elementOfInterest.edgeId;

            for (std::vector<deliveryStop>::iterator stopIt = stops.begin(); stopIt != stops.end(); stopIt++) {
                if (currentNode->id == stopIt->intersection) {
                    shortestTravelTime = elementOfInterest.travelTime;
                    interVisited.push_back(*stopIt);
                    queueOfIntersections = std::priority_queue<prioElem, std::vector<prioElem>, compare>();
                    pathFound = true;
                    resetNodes();
                    return true;
                }
            }

            IntersectionIdx nextIntersection;

            for (std::vector<int>::iterator segmentIt = currentNode->outgoingEdges.begin();
                 segmentIt != currentNode->outgoingEdges.end(); segmentIt++) {
                StreetSegmentInfo segInfo = getStreetSegmentInfo(*segmentIt);
                if (segInfo.to == currentNode->id) {
                    if (segInfo.oneWay) {
                        continue;
                    }
                    nextIntersection = segInfo.from;
                } else {
                    nextIntersection = segInfo.to;
                }
                if (currentNode != source) {
                    StreetSegmentInfo turnInfo = getStreetSegmentInfo(currentNode->reachingEdge);
                    turn = segInfo.streetID != turnInfo.streetID;
                }
                if (turn) {
                    queueOfIntersections.push(prioElem(getNodeFromId(nextIntersection), (*segmentIt),
                                                       elementOfInterest.travelTime +
                                                       findStreetSegmentTravelTime(*segmentIt) + turn_penalty));
                } else {
                    queueOfIntersections.push(prioElem(getNodeFromId(nextIntersection), (*segmentIt),
                                                       elementOfInterest.travelTime +
                                                       findStreetSegmentTravelTime(*segmentIt)));
                }
            }
        }
    }
    resetNodes();
    return false;
}

std::vector<StreetSegmentIdx> findPathBetweenIntersections(
        const IntersectionIdx intersect_id_start,
        const IntersectionIdx intersect_id_destination,
        const double turn_penalty
) {
    std::unordered_map<IntersectionIdx, double> costSoFar; //holds costs to travel to nodes
    std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin; // holds previous node
    std::vector<IntersectionIdx> visited;//holds if a node has been visted
    std::priority_queue<prioElemPF, std::vector<prioElemPF>, comparePF> queueOfIntersections; //mini heap to store queue for streets that need to be expanded upon

    queueOfIntersections.push(prioElemPF{intersect_id_start, 0}); //push source intersection to heap
    pathOrigin[intersect_id_start] = intersect_id_start;
    costSoFar[intersect_id_start] = 0; //zero cost for source

    while (!queueOfIntersections.empty()) { //loops till heap is empty
        prioElemPF current = queueOfIntersections.top(); //get value from the top of the queue
        queueOfIntersections.pop(); //remove entry from queue

        if (current.intersection == intersect_id_destination) { //allow for early exit if destination reached
            return reconstructPath(pathOrigin, intersect_id_destination, intersect_id_start);
        }

        visited.push_back(current.intersection); //mark intersection as visited

        std::vector<IntersectionIdx> neighbors = findAdjacentIntersections(current.intersection); //find neighboring intersections

        for (int & neighbor : neighbors) { //loop through neighbors

            if(std::find(visited.begin(), visited.end(), neighbor) == visited.end()) {//only expand the intersection if it hasnt been expanded
                double newCost =
                        costSoFar[current.intersection] + calculateCost(turn_penalty, findSegmentBetweenIntersections(
                                pathOrigin[current.intersection], current.intersection),
                                                                        findSegmentBetweenIntersections(
                                                                                current.intersection, neighbor));//calculate new cost of reaching intersection
                if (costSoFar.find(neighbor) == costSoFar.end() || newCost < costSoFar[neighbor]) {//if no cost exists or new cost is lower that existing cost
                    if(checkOneWay(current.intersection, neighbor)) {//check if legal movement
                        costSoFar[neighbor] = newCost;//new cost becomes the new cost
                        StreetSegmentInfo heuristicInfo = getStreetSegmentInfo(
                                findSegmentBetweenIntersections(current.intersection, neighbor));
                        double priority = newCost + calculateHeuristic(neighbor, intersect_id_destination,
                                                                       heuristicInfo.speedLimit);//calculate priority for the heap
                        queueOfIntersections.push(prioElemPF{neighbor, priority});//push to heap
                        pathOrigin[neighbor] = current.intersection;//mark path origin

                    }
                }
            }
        }
    }
    return {}; //returns empty array if no path is possible
}
