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
#include "Node.h"

#define NO_EDGE -1
#define NO_TIME 0
#define NOT_DELIVERY_NODE -1




struct deliveryStop{
    IntersectionIdx intersection;
    std::string type;

    deliveryStop(IntersectionIdx id, std::string intersectionType){
        intersection = id; type = intersectionType;
    }
};

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

    void print(){
        std::cout << "x: " + std::to_string(x) + " y: " + std::to_string(y) << std::endl;
    }
};

struct prioElemPF{
    IntersectionIdx intersection;
    double travelTime;

    prioElemPF(int id, double time){
        intersection = id;
        travelTime = time;
    }
};

struct prioElem{
    Node* node;
    int edgeId;
    double travelTime;

    prioElem(Node* n, int id, double time){
        node = n;
        edgeId = id;
        travelTime = time;
    }
};

struct comparePF{
    bool operator()(const prioElemPF & a, const prioElemPF & b){
        return a.travelTime > b.travelTime;
    }
};

struct compare{
    bool operator()(const prioElem & a, const prioElem & b){
        return a.travelTime > b.travelTime;
    }
};

//helper to check if a left turn occurs, takes the previous street and the current street as parameters, and returns true if a left turn occurs
std::string findDirection(StreetSegmentIdx sourceStreet, StreetSegmentIdx destStreet);
extern bool pathFound;

bool checkOneWay(IntersectionIdx, IntersectionIdx);

bool djikstra(std::vector<deliveryStop> stops, Node* source, const double turn_penalty);

std::vector<StreetSegmentIdx> traceback(IntersectionIdx dest);

std::vector<StreetSegmentIdx> findPathDK(const std::vector<deliveryStop>& stops, const IntersectionIdx departurePoint, const double turn_penalty);

Node* getNodeFromId(IntersectionIdx id);

void clearVisitedNodes();

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

extern std::vector<deliveryStop> interVisited;
#endif //MAPPER_PATHFINDING_H
