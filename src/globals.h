#ifndef GLOBALS_H
#define GLOBALS_H

#include <Graph.h>
#include <bobcat_ui.h>
#include <dropdown.h>
#include <list_box.h>
#include <memory>
#include <vector>

using namespace std;
using namespace bobcat;

// Forward declaration
class MapWidget;

// Global data structures
extern Graph g;
extern ArrayList<Vertex *> airports;
extern std::vector<std::shared_ptr<Waypoint>> currentPaths;

// UI components that need global access
extern Dropdown *fromDd;
extern Dropdown *toDd;
extern ListBox *airportList;
extern ListBox *routeList;
extern MapWidget *mapCanvas;

// Data loading and UI refresh functions
void loadData();
void refreshDropdowns();
void refreshAirportList();
void refreshRouteList();

#endif // GLOBALS_H
