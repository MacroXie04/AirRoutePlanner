#include "globals.h"
#include "FileLoader.h"
#include "MapWidget.h"
#include <set>
#include <sstream>

// Define global variables
Graph g;
ArrayList<Vertex *> airports;
std::vector<std::shared_ptr<Waypoint>> currentPaths;

// UI components
Dropdown *fromDd = nullptr;
Dropdown *toDd = nullptr;
ListBox *airportList = nullptr;
ListBox *routeList = nullptr;
MapWidget *mapCanvas = nullptr;

void loadData() {
  g = Graph(); // Reset graph
  airports = ArrayList<Vertex *>();

  // Load data directly from files
  FileLoader::loadIntoGraph(g, airports);
}

void refreshDropdowns() {
  if (fromDd && toDd) {
    fromDd->clear();
    toDd->clear();
    for (int i = 0; i < airports.size(); i++) {
      string label = airports[i]->data;
      fromDd->add(label);
      toDd->add(label);
    }
    if (airports.size() > 0)
      fromDd->value(0);
    if (airports.size() > 1)
      toDd->value(1);
  }
}

void refreshAirportList() {
  if (airportList) {
    airportList->clear();
    for (int i = 0; i < airports.size(); i++) {
      stringstream ss;
      ss << airports[i]->data << " (" << airports[i]->x << "," << airports[i]->y
         << ")";
      airportList->add(ss.str());
    }
  }
}

void refreshRouteList() {
  if (routeList) {
    routeList->clear();
    // Track displayed pairs to avoid duplicates (bidirectional routes)
    std::set<std::pair<std::string, std::string>> displayedPairs;

    for (int i = 0; i < airports.size(); i++) {
      Vertex *u = airports[i];
      for (int j = 0; j < u->edgeList.size(); j++) {
        Edge *e = u->edgeList[j];
        std::string cityA = u->data;
        std::string cityB = e->to->data;

        // Create a normalized pair (alphabetically ordered) to avoid duplicates
        std::pair<std::string, std::string> normalizedPair =
            (cityA < cityB) ? std::make_pair(cityA, cityB)
                            : std::make_pair(cityB, cityA);

        // Only add if not already displayed
        if (displayedPairs.find(normalizedPair) == displayedPairs.end()) {
          displayedPairs.insert(normalizedPair);
          stringstream ss;
          ss << cityA << " <-> " << cityB << " ($" << e->cost << ", "
             << e->duration << "h)";
          routeList->add(ss.str());
        }
      }
    }
  }
}
