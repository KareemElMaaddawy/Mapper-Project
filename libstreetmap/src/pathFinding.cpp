#include "pathFinding.h"

double dot(double oneX, double oneY, double twoX, double twoY) {
    return oneX * twoX + oneY * twoY;
}

double mag(double x, double y) {
    return std::sqrt(x * x + y * y);
}

//helper to check if a left turn occurs, takes the previous street and the current street as parameters, and returns true if a left turn occurs
std::string calculateDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet) {
    StreetSegmentInfo sourceInfo = getStreetSegmentInfo(sourceStreet);
    StreetSegmentInfo destInfo = getStreetSegmentInfo(destStreet);

    if (getStreetName(sourceInfo.streetID) != getStreetName(destInfo.streetID)) {
        LatLon sourceFrom = getIntersectionPosition(sourceInfo.from);
        LatLon sourceTo = getIntersectionPosition(sourceInfo.to);
        LatLon destFrom = getIntersectionPosition(destInfo.from);
        LatLon destTo = getIntersectionPosition(destInfo.to);

        LatLon intersectionPosition;
        LatLon source;
        LatLon dest;

        if (sourceFrom == destFrom) {
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
            std::cerr << "no overlap" << std::endl;
            return "error";
        }

        double vectorOneX = x_from_lon(intersectionPosition.longitude()) - x_from_lon(source.longitude());
        double vectorOneY = y_from_lat(intersectionPosition.latitude()) - y_from_lat(source.latitude());
        double vectorTwoX = x_from_lon(dest.longitude()) - x_from_lon(intersectionPosition.longitude());
        double vectorTwoY = y_from_lat(dest.latitude()) - y_from_lat(intersectionPosition.latitude());

        double angle = acos(dot(vectorOneX, vectorOneY, vectorTwoX, vectorTwoY) /
                            (mag(vectorOneX, vectorOneY) * mag(vectorTwoX, vectorTwoY)));

        if (angle < 180) {
            return "left";
        } else {
            return "right";
        }
    } else {
        return "straight";
    }

}

double calcTurnPenalty(std::vector<StreetSegmentIdx> path) {
    double totalLength = 0;
    double num = 0;
    for(int i = 0; i < path.size(); ++i){
        StreetSegmentInfo segInfo = getStreetSegmentInfo(path[i]);
        double speed  = segInfo.speedLimit;
        double length = findStreetSegmentLength(path[i]);

        totalLength += length;
        num += length * speed;
    }

    return num/totalLength;
}

double computePathTravelTime(const std::vector<StreetSegmentIdx> &path, const double turn_penalty) {
    double travelTime = 0;
    std::string direction;
    for (int i = 0; i < path.size(); ++i) {
        if (i == 0) {//no need to check for left turn for the initial street segment
            travelTime += findStreetSegmentTravelTime(path[i]);
        } else {
            direction = calculateDirection(path[i - 1], path[i]);
            if (direction == "left") {
                travelTime += findStreetSegmentTravelTime(path[i]) + turn_penalty;
            } else {
                travelTime += findStreetSegmentTravelTime(path[i]);
            }
        }
    }
    return travelTime;
}

StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to) {
    int numOfStreetSegments = getNumIntersectionStreetSegment(from);
    for (int i = 0; i < numOfStreetSegments; ++i) {
        StreetSegmentIdx streetSeg = getIntersectionStreetSegment(from, i);
        StreetSegmentInfo streetSegInfo = getStreetSegmentInfo(streetSeg);
        if (streetSegInfo.to == to || streetSegInfo.from == to) {
            return streetSeg;
        }
    }
    return -1;
}

double calculateCost(const double turnPenalty, const IntersectionIdx soruceIntersection,
                     const IntersectionIdx destinationIntersection) {
    double cost = 0;
    std::vector<StreetSegmentIdx> path = {findSegmentBetweenIntersections(soruceIntersection, destinationIntersection)};
    cost += computePathTravelTime(path, turnPenalty);
    return cost;
}

double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection) {
    return findDistanceBetweenTwoPoints(std::make_pair(getIntersectionPosition(currentIntersection),
                                                       getIntersectionPosition(destinationIntersection)));
}

std::vector<StreetSegmentIdx> reconstructPath(std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin,
                                              const IntersectionIdx intersect_id_destination,
                                              const IntersectionIdx intersect_id_start) {
    std::vector<StreetSegmentIdx> path;
    IntersectionIdx currentIntersection = intersect_id_destination;
    while (currentIntersection != intersect_id_start) {
        IntersectionIdx previousIntersection = pathOrigin[currentIntersection];
        path.insert(path.begin(), findSegmentBetweenIntersections(previousIntersection, currentIntersection));
        currentIntersection = previousIntersection;
    }
    return path;
}

std::vector<StreetSegmentIdx> findPathBetweenIntersections(
        const IntersectionIdx intersect_id_start,
        const IntersectionIdx intersect_id_destination,
        const double turn_penalty
) {
    std::cout << intersect_id_start << std::endl;
    std::cout << intersect_id_destination << std::endl << std::endl;
    std::unordered_map<IntersectionIdx, double> costSoFar;
    std::unordered_map<IntersectionIdx, IntersectionIdx> pathOrigin;
    std::priority_queue<prioElem, std::vector<prioElem>, compare> queueOfIntersections; //mini heap to store queue for streets that need to be expanded upon

    queueOfIntersections.push(prioElem{intersect_id_start, 0});
    pathOrigin[intersect_id_start] = intersect_id_start;
    costSoFar[intersect_id_start] = 0;

    while (!queueOfIntersections.empty()) {
        prioElem current = queueOfIntersections.top(); //get value from the top of the queue

        queueOfIntersections.pop(); //remove entry from queue

        if (current.intersection == intersect_id_destination) { //allow for early exit if destination reached
            return reconstructPath(pathOrigin, intersect_id_destination, intersect_id_start);
        }

        std::vector<IntersectionIdx> neighbors = findAdjacentIntersections(current.intersection);

        for (int i = 0; i < neighbors.size(); ++i) {
            double newCost =
                    costSoFar[current.intersection] + calculateCost(turn_penalty, current.intersection, neighbors[i]);
            if (costSoFar.find(neighbors[i]) == costSoFar.end() || newCost < costSoFar[neighbors[i]]) {
                costSoFar[neighbors[i]] = newCost;
                double priority = newCost + calculateHeuristic(neighbors[i], intersect_id_destination);
                queueOfIntersections.push(prioElem{neighbors[i], priority});
                pathOrigin[neighbors[i]] = current.intersection;
            }
        }
    }
    return reconstructPath(pathOrigin, intersect_id_destination, intersect_id_start);
}