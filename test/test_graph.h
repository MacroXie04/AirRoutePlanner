#ifndef TEST_GRAPH_H
#define TEST_GRAPH_H

#include <igloo/igloo.h>
#include <Graph.h>

using namespace igloo;

// Helper function to delete a Waypoint and its entire tree (parent chain and children)
inline void deleteWaypointTree(Waypoint* wp) {
    if (wp == nullptr) return;
    
    // Delete all children recursively
    for (int i = 0; i < wp->children.size(); i++) {
        deleteWaypointTree(wp->children[i]);
    }
    
    delete wp;
}

// Helper to find and delete the root of a Waypoint tree
inline void deleteWaypointFromResult(Waypoint* result) {
    if (result == nullptr) return;
    
    // Find the root (topmost parent)
    Waypoint* root = result;
    while (root->parent != nullptr) {
        root = root->parent;
    }
    
    // Delete entire tree from root
    deleteWaypointTree(root);
}

// Helper to clean up a Graph's vertices and edges
inline void cleanupGraph(Graph& g) {
    for (int i = 0; i < g.vertices.size(); i++) {
        Vertex* v = g.vertices[i];
        // Delete all edges from this vertex
        for (int j = 0; j < v->edgeList.size(); j++) {
            delete v->edgeList[j];
        }
        delete v;
    }
}

// =============================================================================
// Graph Search Tests
// Tests for graph pathfinding with different optimization criteria
// =============================================================================
Context(GraphTests) {
    Graph g;
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    Vertex* v4;

    void SetUp() {
        g = Graph();
        v1 = new Vertex("A");
        v2 = new Vertex("B");
        v3 = new Vertex("C");
        v4 = new Vertex("D");

        g.addVertex(v1);
        g.addVertex(v2);
        g.addVertex(v3);
        g.addVertex(v4);

        // A -> B: Cost 10, Time 20
        g.addDirectedEdge(v1, v2, 10, 20);
        
        // B -> C: Cost 10, Time 20
        g.addDirectedEdge(v2, v3, 10, 20);

        // A -> D: Cost 50, Time 5
        g.addDirectedEdge(v1, v4, 50, 5);

        // D -> C: Cost 50, Time 5
        g.addDirectedEdge(v4, v3, 50, 5);
        
        // Path 1 (A->B->C): Total Cost 20, Total Time 40, Stops 2
        // Path 2 (A->D->C): Total Cost 100, Total Time 10, Stops 2
    }
    
    void TearDown() {
        cleanupGraph(g);
    }

    Spec(CheapestPath) {
        Waypoint* result = g.search(v1, v3, CHEAPEST);
        Assert::That(result != nullptr);
        Assert::That(result->totalCost, Equals(20));
        Assert::That(result->vertex->data, Equals("C"));
        
        // Verify path is A -> B -> C
        Assert::That(result->parent->vertex->data, Equals("B"));
        Assert::That(result->parent->parent->vertex->data, Equals("A"));
        deleteWaypointFromResult(result);
    }

    Spec(FastestPath) {
        Waypoint* result = g.search(v1, v3, FASTEST);
        Assert::That(result != nullptr);
        Assert::That(result->totalTime, Equals(10));
        
        // Verify path is A -> D -> C
        Assert::That(result->parent->vertex->data, Equals("D"));
        Assert::That(result->parent->parent->vertex->data, Equals("A"));
        deleteWaypointFromResult(result);
    }
    
    Spec(LeastStops) {
        // Add a direct edge A -> C with high cost and time
        // A -> C: Cost 200, Time 200
        g.addDirectedEdge(v1, v3, 200, 200);
        
        Waypoint* result = g.search(v1, v3, LEAST_STOPS);
        Assert::That(result != nullptr);
        Assert::That(result->stops, Equals(1)); // Direct flight is 1 stop
        
        // Verify path is A -> C
        Assert::That(result->parent->vertex->data, Equals("A"));
        deleteWaypointFromResult(result);
    }

    Spec(NoPath) {
        Vertex* isolated = new Vertex("Isolated");
        g.addVertex(isolated);
        
        Waypoint* result = g.search(v1, isolated, CHEAPEST);
        Assert::That(result, IsNull());
        // Note: isolated vertex is cleaned up by TearDown via cleanupGraph
    }
};

// =============================================================================
// BFS Tests
// Tests for Breadth-First Search traversal
// =============================================================================
Context(GraphBFSTests) {
    Graph g;
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    Vertex* v4;

    void SetUp() {
        g = Graph();
        v1 = new Vertex("A");
        v2 = new Vertex("B");
        v3 = new Vertex("C");
        v4 = new Vertex("D");

        g.addVertex(v1);
        g.addVertex(v2);
        g.addVertex(v3);
        g.addVertex(v4);

        // A -> B, B -> C, A -> D, D -> C
        g.addDirectedEdge(v1, v2, 10, 20);
        g.addDirectedEdge(v2, v3, 10, 20);
        g.addDirectedEdge(v1, v4, 50, 5);
        g.addDirectedEdge(v4, v3, 50, 5);
    }
    
    void TearDown() {
        cleanupGraph(g);
    }

    Spec(BFSFindsPath) {
        Waypoint* result = g.bfs(v1, v3);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("C"));
        deleteWaypointFromResult(result);
    }

    Spec(BFSFindsDirectNeighbor) {
        Waypoint* result = g.bfs(v1, v2);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("B"));
        Assert::That(result->parent->vertex->data, Equals("A"));
        deleteWaypointFromResult(result);
    }

    Spec(BFSNoPath) {
        Vertex* isolated = new Vertex("Isolated");
        g.addVertex(isolated);
        Waypoint* result = g.bfs(v1, isolated);
        Assert::That(result, IsNull());
        // Note: isolated vertex is cleaned up by TearDown via cleanupGraph
    }

    Spec(BFSSameStartEnd) {
        Waypoint* result = g.bfs(v1, v1);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("A"));
        deleteWaypointFromResult(result);
    }
};

// =============================================================================
// DFS Tests
// Tests for Depth-First Search traversal
// =============================================================================
Context(GraphDFSTests) {
    Graph g;
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    Vertex* v4;

    void SetUp() {
        g = Graph();
        v1 = new Vertex("A");
        v2 = new Vertex("B");
        v3 = new Vertex("C");
        v4 = new Vertex("D");

        g.addVertex(v1);
        g.addVertex(v2);
        g.addVertex(v3);
        g.addVertex(v4);

        g.addDirectedEdge(v1, v2, 10, 20);
        g.addDirectedEdge(v2, v3, 10, 20);
        g.addDirectedEdge(v1, v4, 50, 5);
        g.addDirectedEdge(v4, v3, 50, 5);
    }
    
    void TearDown() {
        cleanupGraph(g);
    }

    Spec(DFSFindsPath) {
        Waypoint* result = g.dfs(v1, v3);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("C"));
        deleteWaypointFromResult(result);
    }

    Spec(DFSFindsDirectNeighbor) {
        Waypoint* result = g.dfs(v1, v2);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("B"));
        deleteWaypointFromResult(result);
    }

    Spec(DFSNoPath) {
        Vertex* isolated = new Vertex("Isolated");
        g.addVertex(isolated);
        Waypoint* result = g.dfs(v1, isolated);
        Assert::That(result, IsNull());
        // Note: isolated vertex is cleaned up by TearDown via cleanupGraph
    }

    Spec(DFSSameStartEnd) {
        Waypoint* result = g.dfs(v1, v1);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("A"));
        deleteWaypointFromResult(result);
    }
};

// =============================================================================
// Graph Edge Cases
// Tests for vertex, edge, and waypoint construction
// =============================================================================
Context(GraphEdgeCases) {
    Spec(VertexWithCoordinates) {
        Vertex* v = new Vertex("Test", 100, 200);
        Assert::That(v->data, Equals("Test"));
        Assert::That(v->x, Equals(100));
        Assert::That(v->y, Equals(200));
        delete v;
    }

    Spec(EdgeWithCostAndDuration) {
        Vertex* from = new Vertex("A");
        Vertex* to = new Vertex("B");
        Edge* e = new Edge(from, to, 100, 60);
        Assert::That(e->cost, Equals(100));
        Assert::That(e->duration, Equals(60));
        Assert::That(e->weight, Equals(100)); // weight defaults to cost
        delete e;
        delete from;
        delete to;
    }

    Spec(LegacyEdgeConstructor) {
        Vertex* from = new Vertex("A");
        Vertex* to = new Vertex("B");
        Edge* e = new Edge(from, to, 50);
        Assert::That(e->weight, Equals(50));
        Assert::That(e->cost, Equals(50));
        Assert::That(e->duration, Equals(0));
        delete e;
        delete from;
        delete to;
    }

    Spec(WaypointInitialization) {
        Vertex* v = new Vertex("Test");
        Waypoint* wp = new Waypoint(v);
        Assert::That(wp->parent, IsNull());
        Assert::That(wp->totalCost, Equals(0));
        Assert::That(wp->totalTime, Equals(0));
        Assert::That(wp->stops, Equals(0));
        delete wp;
        delete v;
    }

    Spec(UndirectedEdge) {
        Graph g;
        Vertex* v1 = new Vertex("A");
        Vertex* v2 = new Vertex("B");
        g.addVertex(v1);
        g.addVertex(v2);
        g.addEdge(v1, v2, 100, 60);
        
        // Both vertices should have edges
        Assert::That(v1->edgeList.size(), Equals(1));
        Assert::That(v2->edgeList.size(), Equals(1));
        
        // Edge from A to B
        Assert::That(v1->edgeList[0]->to->data, Equals("B"));
        // Edge from B to A
        Assert::That(v2->edgeList[0]->to->data, Equals("A"));
        
        cleanupGraph(g);
    }

    Spec(LegacyUCS) {
        Graph g;
        Vertex* v1 = new Vertex("A");
        Vertex* v2 = new Vertex("B");
        g.addVertex(v1);
        g.addVertex(v2);
        g.addDirectedEdge(v1, v2, 10, 20);
        
        Waypoint* result = g.ucs(v1, v2);
        Assert::That(result != nullptr);
        Assert::That(result->vertex->data, Equals("B"));
        
        deleteWaypointFromResult(result);
        cleanupGraph(g);
    }
};

// =============================================================================
// Complex Graph Scenario
// Tests with a realistic multi-city airline route network
// =============================================================================
Context(ComplexGraphTests) {
    Graph g;
    Vertex* nyc;
    Vertex* lax;
    Vertex* ord;
    Vertex* dfw;
    Vertex* den;

    void SetUp() {
        g = Graph();
        nyc = new Vertex("NYC", 0, 0);
        lax = new Vertex("LAX", 100, 0);
        ord = new Vertex("ORD", 50, 50);
        dfw = new Vertex("DFW", 50, -25);
        den = new Vertex("DEN", 75, 25);

        g.addVertex(nyc);
        g.addVertex(lax);
        g.addVertex(ord);
        g.addVertex(dfw);
        g.addVertex(den);

        // NYC -> ORD: $150, 2hr
        g.addDirectedEdge(nyc, ord, 150, 120);
        // NYC -> DFW: $200, 3hr
        g.addDirectedEdge(nyc, dfw, 200, 180);
        // ORD -> DEN: $100, 2hr
        g.addDirectedEdge(ord, den, 100, 120);
        // DFW -> DEN: $80, 1.5hr
        g.addDirectedEdge(dfw, den, 80, 90);
        // DEN -> LAX: $120, 2hr
        g.addDirectedEdge(den, lax, 120, 120);
        // ORD -> LAX: $250, 4hr (direct but expensive)
        g.addDirectedEdge(ord, lax, 250, 240);
        // NYC -> LAX: $500, 5hr (direct, most expensive)
        g.addDirectedEdge(nyc, lax, 500, 300);
    }
    
    void TearDown() {
        cleanupGraph(g);
    }

    Spec(CheapestNYCtoLAX) {
        // Cheapest: NYC -> ORD -> DEN -> LAX = $150 + $100 + $120 = $370
        Waypoint* result = g.search(nyc, lax, CHEAPEST);
        Assert::That(result != nullptr);
        Assert::That(result->totalCost, Equals(370));
        deleteWaypointFromResult(result);
    }

    Spec(FastestNYCtoLAX) {
        // Fastest: NYC -> LAX direct = 300min (5hr)
        // Other paths take longer, so direct is optimal
        Waypoint* result = g.search(nyc, lax, FASTEST);
        Assert::That(result != nullptr);
        Assert::That(result->totalTime, Equals(300));
        deleteWaypointFromResult(result);
    }

    Spec(LeastStopsNYCtoLAX) {
        // Direct flight NYC -> LAX is 1 stop
        Waypoint* result = g.search(nyc, lax, LEAST_STOPS);
        Assert::That(result != nullptr);
        Assert::That(result->stops, Equals(1));
        Assert::That(result->parent->vertex->data, Equals("NYC"));
        deleteWaypointFromResult(result);
    }

    Spec(MultipleVertices) {
        Assert::That(g.vertices.size(), Equals(5));
    }
};

#endif // TEST_GRAPH_H

