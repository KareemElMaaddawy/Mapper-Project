#ifndef MAPPER_PATHFINDING_H
#define MAPPER_PATHFINDING_H

#include <functional>
#include <math.h>
#include <queue>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "m3.h"
#include "globalHeader.h"
#include <algorithm>


class Point{
public:
    double x;
    double y;

    Point(LatLon input){
        x = x_from_lon(input.longitude());
        y = y_from_lat(input.latitude());
    }

    Point(double xInput, double yInput){
        x = xInput;
        y = yInput;
    }

    Point subtract(Point second){
        return Point(x - second.x, y - second.y);
    }

    void print(){
        std::cout << "x: " + std::to_string(x) + " y: " + std::to_string(y) << std::endl;
    }
};
struct prioElem{
    IntersectionIdx intersection;
    double priority;
};

struct compare{
    bool operator()(const prioElem & a, const prioElem & b){
        return a.priority > b.priority;
    }
};

//helper to check if a left turn occurs, takes the previous street and the current street as parameters, and returns true if a left turn occurs
std::string findDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet);

bool checkOneWay(IntersectionIdx, IntersectionIdx);

StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to);

double findSlope(Point p0, Point p1);

double calculateCost(const double turnPenalty,const IntersectionIdx soruceIntersection,const IntersectionIdx destinationIntersection);

double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection, const double speed);

double calcTurnPenalty(IntersectionIdx source, IntersectionIdx dest);

std::vector<StreetSegmentIdx> reconstructPath(
        const std::unordered_map<IntersectionIdx,
        IntersectionIdx> pathOrigin,
        const IntersectionIdx intersect_id_destination,
        const IntersectionIdx intersect_id_start
);
#endif //MAPPER_PATHFINDING_H
