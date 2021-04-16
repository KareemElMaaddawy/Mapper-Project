#include "Node.h"
#include "m1.h"

Node::Node() {
    id = 0;
    outgoingEdges = findStreetSegmentsOfIntersection(id);
    reachingEdge = -1;
    bestTime = std::numeric_limits<int>::max();
    expand = true;
}

Node::Node(int intersectionID, int reachingEdgeID, double time) {
    id = intersectionID;
    outgoingEdges = findStreetSegmentsOfIntersection(id);
    reachingEdge = reachingEdgeID;
    bestTime = time;
    expand = true;
}