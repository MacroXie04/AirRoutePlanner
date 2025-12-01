#ifndef GLOBALS_H
#define GLOBALS_H

#include <DatabaseManager.h>
#include <Graph.h>
#include <bobcat_ui.h>
#include <dropdown.h>
#include <list_box.h>
#include <map>
#include <vector>

using namespace std;
using namespace bobcat;

// Forward declaration
class MapWidget;

// Global data structures
extern Graph g;
extern ArrayList<Vertex *> airports;
extern std::map<int, Vertex *> airportMap;
extern std::vector<AirportData> dbAirports;
extern std::vector<RouteData> dbRoutes;
extern DatabaseManager db;
extern Waypoint *currentPath;

// UI components that need global access
extern Dropdown *fromDd;
extern Dropdown *toDd;
extern Dropdown *routeSourceDd;
extern Dropdown *routeDestDd;
extern ListBox *airportList;
extern ListBox *routeList;
extern MapWidget *mapCanvas;

// Data loading and UI refresh functions
void loadData();
void refreshDropdowns();
void refreshAirportList();
void refreshRouteList();

#endif // GLOBALS_H
