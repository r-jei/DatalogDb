#ifndef NODE_H
#define NODE_H

#include <set>

struct Node{
    Node();
    std::set<int> edges;
    bool visited;
    int postorder;
};

#endif
