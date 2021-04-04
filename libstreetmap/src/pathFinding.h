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
std::string calculateDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet);

std::string calculateDirectionT(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet);

bool checkOneWay(IntersectionIdx, IntersectionIdx);

StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to);

double dot(double oneX, double oneY, double twoX, double twoY);

double mag(double x, double y);

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
