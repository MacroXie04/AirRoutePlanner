#include <FL/Fl_Tabs.H>
#include <bobcat_ui.h>
#include <button.h>
#include <dropdown.h>
#include <group.h>
#include <iostream>
#include <list_box.h>
#include <memo.h>
#include <textbox.h>
#include <window.h>

#include "MapWidget.h"
#include "callbacks.h"
#include "globals.h"

using namespace std;
using namespace bobcat;

int main() {
  loadData();

  Application_ app;

  // Main Window - with right panel for viewing data
  bobcat::Window *win = new bobcat::Window(1280, 700, "Air Route Planner");

  // --- LEFT PANEL: ROUTE PLANNER ---
  Group *controls = new Group(10, 10, 280, 680);
  controls->box(FL_FLAT_BOX);

  // Planner Section
  fromDd = new Dropdown(20, 40, 260, 30, "From:");
  toDd = new Dropdown(20, 100, 260, 30, "To:");

  Dropdown *prefDd = new Dropdown(20, 160, 260, 30, "Preference:");
  prefDd->add("Cheapest Price");
  prefDd->add("Shortest Time");
  prefDd->add("Least Number of Stops");
  prefDd->value(0);

  Button *findBtn = new Button(20, 210, 260, 40, "Find Route");

  // Result display using bobcat TextBox for label
  new TextBox(20, 260, 260, 20, "Itinerary Details:");

  // Using bobcat Memo for multiline text display (set readonly)
  Memo *resultOutput = new Memo(20, 280, 260, 340);
  resultOutput->box(FL_DOWN_BOX);
  resultOutput->color(FL_WHITE);
  resultOutput->readonly(1); // Make it read-only for display

  Button *copyBtn = new Button(20, 630, 260, 30, "Copy to Clipboard");

  controls->end();

  // --- CENTER PANEL: MAP VISUALIZATION ---
  mapCanvas = new MapWidget(300, 10, 680, 680);

  // --- RIGHT PANEL: DATA VIEWING (Read-only) ---
  Fl_Tabs *tabs = new Fl_Tabs(990, 10, 280, 680);

  // Airport Tab
  Fl_Group *airportTab = new Fl_Group(990, 35, 280, 655, "Airports");
  airportTab->box(FL_FLAT_BOX);

  airportList = new ListBox(1000, 60, 260, 600, "Airport List:");

  airportTab->end();

  // Route Tab
  Fl_Group *routeTab = new Fl_Group(990, 35, 280, 655, "Routes");
  routeTab->box(FL_FLAT_BOX);

  routeList = new ListBox(1000, 60, 260, 600, "Route List:");

  routeTab->end();

  tabs->end();

  refreshDropdowns();
  refreshAirportList();
  refreshRouteList();

  // Setup callbacks
  setupFindRouteCallback(findBtn, fromDd, toDd, prefDd, resultOutput);
  setupCopyCallback(copyBtn, resultOutput);

  win->show();
  return app.run();
}
