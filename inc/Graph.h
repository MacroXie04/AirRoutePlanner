#ifndef GRAPH_H
#define GRAPH_H

#include "LinkedList.h"
#include <ArrayList.h>
#include <HashTable.h>
#include <Queue.h>
#include <Stack.h>
#include <cstddef>
#include <ostream>
#include <string>

enum SearchCriteria { CHEAPEST, FASTEST, LEAST_STOPS };

struct Edge;

struct Vertex {
    std::string data;
    ArrayList<Edge *> edgeList;
    int x;
    int y;

    Vertex(std::string data, int x = 0, int y = 0) { 
        this->data = data; 
        this->x = x;
        this->y = y;
    }

    ~Vertex();
};

inline std::ostream &operator<<(std::ostream &os, Vertex *v) {
    os << v->data;

    return os;
}

struct Edge {
    Vertex *from;
    Vertex *to;
    int weight; // Kept for compatibility, though we use cost/duration
    int cost;
    int duration;

    Edge(Vertex *from, Vertex *to, int cost, int duration) {
        this->from = from;
        this->to = to;
        this->cost = cost;
        this->duration = duration;
        this->weight = cost; // Default weight to cost
    }
    
    // Legacy constructor compatibility
    Edge(Vertex *from, Vertex *to, int weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
        this->cost = weight;
        this->duration = 0;
    }
};

inline std::ostream &operator<<(std::ostream &os, Edge *e) {
    os << "(" << e->from << ", " << e->to << ") - $" << e->cost << ", " << e->duration << "m";

    return os;
}

inline Vertex::~Vertex() {
    for (int i = 0; i < edgeList.size(); i++) {
        delete edgeList[i];
    }
}

struct Waypoint {
    Waypoint *parent;
    Vertex *vertex;
    ArrayList<Waypoint *> children;
    int partialCost;
    int weight;
    
    // New fields
    int totalCost;
    int totalTime;
    int stops;
    bool keep;

    Waypoint(Vertex *v) {
        parent = nullptr;
        vertex = v;
        weight = 0;
        partialCost = 0;
        totalCost = 0;
        totalTime = 0;
        stops = 0;
        keep = false;
    }

    void expand(SearchCriteria criteria = CHEAPEST) {
        for (int i = 0; i < vertex->edgeList.size(); i++) {
            Edge* e = vertex->edgeList[i];
            Waypoint *temp = new Waypoint(e->to);
            temp->parent = this;
            
            temp->totalCost = totalCost + e->cost;
            temp->totalTime = totalTime + e->duration;
            temp->stops = stops + 1;
            
            // Set partial cost based on criteria
            if (criteria == CHEAPEST) {
                temp->partialCost = temp->totalCost;
            } else if (criteria == FASTEST) {
                temp->partialCost = temp->totalTime;
            } else { // LEAST_STOPS
                temp->partialCost = temp->stops;
            }
            
            temp->weight = e->weight;

            children.append(temp);
        }
    }
};

inline std::ostream &operator<<(std::ostream &os, Waypoint *wp) {
    std::string p = "null";
    if (wp->parent != nullptr) {
        p = wp->parent->vertex->data;
    }

    os << p << " -> " << wp->vertex->data;

    return os;
}

struct Graph {
    ArrayList<Vertex *> vertices;

    ~Graph() {
        for (int i = 0; i < vertices.size(); i++) {
            delete vertices[i];
        }
    }

    void addVertex(Vertex *v) { vertices.append(v); }

    void addEdge(Vertex *x, Vertex *y, int cost, int duration) {
        x->edgeList.append(new Edge(x, y, cost, duration));
        y->edgeList.append(new Edge(y, x, cost, duration));
    }

    void addDirectedEdge(Vertex *x, Vertex *y, int cost, int duration) {
        x->edgeList.append(new Edge(x, y, cost, duration));
    }
    
    // Legacy support
    void addEdge(Vertex *x, Vertex *y, int w) {
        addEdge(x, y, w, 0);
    }

    Waypoint *bfs(Vertex *start, Vertex *destination) {
        std::cout << "Running Breadth-First Search" << std::endl;
        Queue<Waypoint *> frontier;
        HashTable<std::string> seen;
        ArrayList<Waypoint *> allWaypoints;

        Waypoint *first = new Waypoint(start);
        allWaypoints.append(first);

        frontier.enqueue(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (!frontier.isEmpty()) {
            result = frontier.dequeue();

            if (result->vertex == destination) {
                Waypoint *curr = result;
                while (curr != nullptr) {
                    curr->keep = true;
                    curr = curr->parent;
                }
                for (int i = 0; i < allWaypoints.size(); i++) {
                    if (!allWaypoints[i]->keep) {
                        delete allWaypoints[i];
                    }
                }
                return result;
            }

            result->expand();
            for (int i = 0; i < result->children.size(); i++) {
                allWaypoints.append(result->children[i]);
            }
            // Get the neighbors of the current vertex
            // that we are on...

            // The neighbors are stored in the result->children array
            std::cout << std::endl
                      << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                // For every child of the result node
                // If we have not seen it
                // We add it to the frontier (as a queue)
                // We mark it as seen
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data
                              << std::endl;
                    frontier.enqueue(result->children[i]);
                    seen.insert(result->children[i]->vertex->data);
                }
            }

            std::cout << std::endl << "Frontier" << std::endl;

            Link<Waypoint *> *temp = frontier.list.front;
            while (temp != nullptr) {
                std::cout << "(" << temp->data->vertex->data << ","
                          << temp->data->partialCost << ")";
                temp = temp->next;

                if (temp != nullptr) {
                    std::cout << ", ";
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }

        for (int i = 0; i < allWaypoints.size(); i++) {
            delete allWaypoints[i];
        }

        return nullptr;
    }

    Waypoint *dfs(Vertex *start, Vertex *destination) {
        std::cout << "Running Depth-First Search" << std::endl;

        Stack<Waypoint *> frontier;
        HashTable<std::string> seen;
        ArrayList<Waypoint *> allWaypoints;

        Waypoint *first = new Waypoint(start);
        allWaypoints.append(first);

        frontier.push(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (!frontier.isEmpty()) {
            result = frontier.pop();

            if (result->vertex == destination) {
                Waypoint *curr = result;
                while (curr != nullptr) {
                    curr->keep = true;
                    curr = curr->parent;
                }
                for (int i = 0; i < allWaypoints.size(); i++) {
                    if (!allWaypoints[i]->keep) {
                        delete allWaypoints[i];
                    }
                }
                return result;
            }

            result->expand();
            for (int i = 0; i < result->children.size(); i++) {
                allWaypoints.append(result->children[i]);
            }

            std::cout << std::endl
                      << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data
                              << std::endl;
                    frontier.push(result->children[i]);
                    seen.insert(result->children[i]->vertex->data);
                }
            }

            std::cout << std::endl << "Frontier" << std::endl;

            Link<Waypoint *> *temp = frontier.front;
            while (temp != nullptr) {
                std::cout << "(" << temp->data->vertex->data << ","
                          << temp->data->partialCost << ")";
                temp = temp->next;

                if (temp != nullptr) {
                    std::cout << ", ";
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }

        for (int i = 0; i < allWaypoints.size(); i++) {
            delete allWaypoints[i];
        }

        return nullptr;
    }

    Waypoint *search(Vertex *start, Vertex *destination, SearchCriteria criteria) {
        std::cout << "Running Search (UCS variant)" << std::endl;

        // Should be a priority queue
        ArrayList<Waypoint *> frontier;
        HashTable<std::string> seen;
        ArrayList<Waypoint *> allWaypoints;

        Waypoint *first = new Waypoint(start);
        allWaypoints.append(first);

        frontier.append(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (frontier.size() != 0) {
            result = frontier.removeLast();

            if (result->vertex == destination) {
                Waypoint *curr = result;
                while (curr != nullptr) {
                    curr->keep = true;
                    curr = curr->parent;
                }
                for (int i = 0; i < allWaypoints.size(); i++) {
                    if (!allWaypoints[i]->keep) {
                        delete allWaypoints[i];
                    }
                }
                return result;
            }

            result->expand(criteria);
            for (int i = 0; i < result->children.size(); i++) {
                allWaypoints.append(result->children[i]);
            }

            std::cout << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                // Look at each child
                if (!seen.search(result->children[i]->vertex->data)) {
                    // If not in the seen list, let's add it
                    std::cout << "Adding " << result->children[i]->vertex->data
                              << std::endl;
                    frontier.append(result->children[i]);


                    // Sort the frontier....
                    int j = frontier.size() - 1;
                    while (j > 0 && frontier.data[j]->partialCost >
                                        frontier.data[j - 1]->partialCost) {

                        Waypoint *temp = frontier.data[j];
                        frontier.data[j] = frontier.data[j - 1];
                        frontier.data[j - 1] = temp;
                        j--;
                    }

                    seen.insert(result->children[i]->vertex->data);
                } else {
                    // If it is in the seen list, we may have to do some work

                    // First we will check if it is still in the frontier but
                    // with a higher partial cost
                    Waypoint *worsePath = nullptr;

                    for (int k = 0; k < frontier.size(); k++) {
                        if (frontier[k]->vertex->data ==
                            result->children[i]->vertex->data) {
                            if (frontier[k]->partialCost >
                                result->children[i]->partialCost) {
                                worsePath = frontier[k];
                                // The same node was visited before,
                                // but with a higher partial cost
                                break;
                            }
                        }
                    }

                    // If we had a worse node before, we need to change it.
                    if (worsePath) {
                        std::cout
                            << "Found another way to get to "
                            << result->children[i]->vertex->data << ". Was "
                            << worsePath->partialCost << ", but now it is "
                            << result->children[i]->partialCost << std::endl;

                        // Make it so that the children of the worse waypoint
                        // become our children
                        for (int k = 0; k < frontier.size(); k++) {
                            if (frontier[k]->parent->vertex->data ==
                                result->children[i]->vertex->data) {
                                frontier[k]->parent = result->children[i];
                            }
                        }

                        // Replace the worse one with the better one
                        for (int k = 0; k < frontier.size(); k++) {
                            if (frontier[k]->vertex->data ==
                                result->children[i]->vertex->data) {
                                // delete frontier[k];
                                frontier[k] = result->children[i];
                                break;
                            }
                        }

                        // Sort the frontier because the replacement above
                        // may have caused things to fall out of order
                        for (int a = 1; a < frontier.size(); a++) {
                            int b = a;
                            while (b > 0 && frontier[b]->partialCost >
                                                frontier[b - 1]->partialCost) {
                                Waypoint *x = frontier[b];
                                frontier[b] = frontier[b - 1];
                                frontier[b - 1] = x;
                                b--;
                            }
                        }
                    }
                }
            }

            std::cout << std::endl << "Frontier" << std::endl;

            for (int k = frontier.size() - 1; k >= 0; k--) {
                std::cout << "(" << frontier[k]->vertex->data << ", "
                          << frontier[k]->partialCost << ") ";
                if (k > 0) {
                    std::cout << ", ";
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }

        for (int i = 0; i < allWaypoints.size(); i++) {
            delete allWaypoints[i];
        }

        return nullptr;
    }

    // Keep legacy UCS for compatibility if needed, but it calls search with default
    Waypoint *ucs(Vertex *start, Vertex *destination) {
        return search(start, destination, CHEAPEST);
    }
};

inline std::ostream &operator<<(std::ostream &os, const Graph &g) {
    for (int i = 0; i < g.vertices.size(); i++) {
        os << g.vertices[i]->edgeList << std::endl;
    }

    return os;
}

#endif
