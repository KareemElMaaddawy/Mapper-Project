#ifndef MAPPER_PATHFINDING_H
#define MAPPER_PATHFINDING_H

#include <functional>
#include <queue>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "m3.h"
#include "globalHeader.h"

struct prioElem{
    IntersectionIdx intersection;
    double priority = 0;
};

struct compare{
    bool operator()(const prioElem & a, const prioElem & b){
        return a.priority > b.priority;
    }
};

//helper to check if a left turn occurs, takes the previous street and the current street as parameters, and returns true if a left turn occurs
std::string movementDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet);

StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to);

double calculateCost(const double turnPenalty,const IntersectionIdx soruceIntersection,const IntersectionIdx destinationIntersection);

double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection);

double calcTurnPenalty(std::vector<StreetSegmentIdx> path);

std::vector<StreetSegmentIdx> reconstructPath(
        const std::unordered_map<IntersectionIdx,
        IntersectionIdx> pathOrigin,
        const IntersectionIdx intersect_id_destination,
        const IntersectionIdx intersect_id_start
);
#endif //MAPPER_PATHFINDING_H
