#include "globals.h"
#include "MapWidget.h"
#include <sstream>

// Define global variables
Graph g;
ArrayList<Vertex*> airports;
std::map<int, Vertex*> airportMap;
std::vector<AirportData> dbAirports;
std::vector<RouteData> dbRoutes;
DatabaseManager db("db/data.db");
Waypoint* currentPath = nullptr;

// UI components
Dropdown* fromDd = nullptr;
Dropdown* toDd = nullptr;
Dropdown* routeSourceDd = nullptr;
Dropdown* routeDestDd = nullptr;
ListBox* airportList = nullptr;
ListBox* routeList = nullptr;
MapWidget* mapCanvas = nullptr;

void loadData() {
    g = Graph(); // Reset graph
    airports = ArrayList<Vertex*>();
    airportMap.clear();

    dbAirports = db.getAirports();
    for (const auto& a : dbAirports) {
        Vertex* v = new Vertex(a.code, a.x, a.y);
        g.addVertex(v);
        airports.append(v);
        airportMap[a.id] = v;
    }

    dbRoutes = db.getRoutes();
    for (const auto& r : dbRoutes) {
        if (airportMap.count(r.source_id) && airportMap.count(r.dest_id)) {
            g.addDirectedEdge(airportMap[r.source_id], airportMap[r.dest_id], r.cost, r.duration);
        }
    }
}

void refreshDropdowns() {
    if (fromDd && toDd) {
        fromDd->clear();
        toDd->clear();
        for(const auto& a : dbAirports) {
            string label = a.code + " - " + a.name;
            fromDd->add(label);
            toDd->add(label);
        }
        if (dbAirports.size() > 0) fromDd->value(0);
        if (dbAirports.size() > 1) toDd->value(1);
    }
    
    if (routeSourceDd && routeDestDd) {
        routeSourceDd->clear();
        routeDestDd->clear();
        for(const auto& a : dbAirports) {
            string label = a.code + " - " + a.name;
            routeSourceDd->add(label);
            routeDestDd->add(label);
        }
        if (dbAirports.size() > 0) routeSourceDd->value(0);
        if (dbAirports.size() > 1) routeDestDd->value(1);
    }
}

void refreshAirportList() {
    if (airportList) {
        airportList->clear();
        for(const auto& a : dbAirports) {
            stringstream ss;
            ss << a.code << " - " << a.name << " (" << a.x << "," << a.y << ")";
            airportList->add(ss.str());
        }
    }
}

void refreshRouteList() {
    if (routeList) {
        routeList->clear();
        for(const auto& r : dbRoutes) {
            // Find airport codes
            string srcCode = "?", dstCode = "?";
            for(const auto& a : dbAirports) {
                if (a.id == r.source_id) srcCode = a.code;
                if (a.id == r.dest_id) dstCode = a.code;
            }
            stringstream ss;
            ss << srcCode << " -> " << dstCode << " ($" << r.cost << ", " << r.duration << "m)";
            routeList->add(ss.str());
        }
    }
}

