#ifndef MAPPER_PATHFINDING_H
#define MAPPER_PATHFINDING_H

#include <functional>
#include <queue>
#include <unordered_map>
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
bool checkForLeftTurn(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet);

double computePathTravelTime(const std::vector <StreetSegmentIdx>& path, const double turn_penalty);

StreetSegmentIdx findSegmentBetweenIntersections(const IntersectionIdx from, const IntersectionIdx to);

double calculateCost(const double turnPenalty,const IntersectionIdx soruceIntersection,const IntersectionIdx destinationIntersection);

double calculateHeuristic(const IntersectionIdx currentIntersection, const IntersectionIdx destinationIntersection);

std::vector<StreetSegmentIdx> reconstructPath(
        const std::unordered_map<IntersectionIdx,
        IntersectionIdx> pathOrigin,
        const IntersectionIdx intersect_id_destination,
        const IntersectionIdx intersect_id_start
);

std::vector<StreetSegmentIdx> findPathBetweenIntersections(
        const IntersectionIdx intersect_id_start,
        const IntersectionIdx intersect_id_destination,
        const double turn_penalty
);
#endif //MAPPER_PATHFINDING_H
