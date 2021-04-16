#ifndef MAPPER_NODE_H
#define MAPPER_NODE_H

#include <vector>

class Node {
    int id;
    std::vector<int> outgoingEdges;
    int reachingEdge;
    double bestTime;
    bool expand;


    Node(int intersectionID, int reachingEdgeID, double time);
    Node();
};


#endif //MAPPER_NODE_H
