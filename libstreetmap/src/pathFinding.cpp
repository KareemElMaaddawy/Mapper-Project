#include "pathFinding.h"

std::vector<int, Node*> nodesExpanded;
std::vect<std::pair<IntersectionIdx, std::string>> interVisited;
double bestTravelTime;

double findSlope(Point p0, Point p1){
    return (p0.y - p1.y) / (p0.x - p1.y);
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
            sourcePoint.print();
            intersectionPoint.print();
            destinationPoint.print();

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
    return speed/numOfSegment;//returning average
}
//finds the travel time, given a path and a turn penalty
double computePathTravelTime(const std::vector<StreetSegmentIdx> &path, const double turn_penalty) {
    double travelTime = 0;
    std::string direction;
    for (int i = 0; i < path.size(); ++i) {
        if (i == 0) {//no need to check for left turn for the initial street segment
            travelTime += findStreetSegmentTravelTime(path[i]);
        } else {
            direction = findDirection(path[i - 1], path[i]);
            if (direction == "left") {
                travelTime += findStreetSegmentTravelTime(path[i]) + turn_penalty;//if left turn occurs turn penalty is instituted
            } else {
                travelTime += findStreetSegmentTravelTime(path[i]);
            }
        }
    }

    return travelTime;
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
//main path finding function
std::vector<StreetSegmentIdx> findPathBetweenIntersections(
        std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin,
        const IntersectionIdx intersect_id_start,
        const IntersectionIdx intersect_id_destination,
        const double turn_penalty
) {
    bool pathFound = false;
    std::vector<StreetSegmentIdx> path;

    return {}; //returns empty array if no path is possible
}
