#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Tabs.H>
#include <bobcat_ui.h>
#include <button.h>
#include <dropdown.h>
#include <group.h>
#include <input.h>
#include <int_input.h>
#include <iostream>
#include <list_box.h>
#include <memo.h>
#include <window.h>

#include "MapWidget.h"
#include "callbacks.h"
#include "globals.h"

using namespace std;
using namespace bobcat;

int main() {
  loadData();

  Application_ app;

  // Main Window - wider to accommodate management panel
  Window *win = new Window(1280, 700, "Air Route Planner");

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

  // Result display - use Fl_Multiline_Output for non-editable, non-selectable
  // text
  Fl_Box *resultLabel = new Fl_Box(20, 260, 260, 20, "Itinerary Details:");
  resultLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

  Fl_Multiline_Output *resultOutput =
      new Fl_Multiline_Output(20, 280, 260, 340);
  resultOutput->box(FL_DOWN_BOX);
  resultOutput->color(FL_WHITE);

  Button *copyBtn = new Button(20, 630, 260, 30, "Copy to Clipboard");

  controls->end();

  // --- CENTER PANEL: MAP VISUALIZATION ---
  mapCanvas = new MapWidget(300, 10, 680, 680);

  // --- RIGHT PANEL: DATA MANAGEMENT ---
  Fl_Tabs *tabs = new Fl_Tabs(990, 10, 280, 680);

  // Airport Tab
  Fl_Group *airportTab = new Fl_Group(990, 35, 280, 655, "Airports");
  airportTab->box(FL_FLAT_BOX);

  airportList = new ListBox(1000, 60, 260, 300, "Airport List:");

  // Edit/Add Airport Section
  Fl_Box *addAirportLabel =
      new Fl_Box(1000, 380, 260, 20, "Add / Edit Airport:");
  addAirportLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

  Fl_Box *codeLabel = new Fl_Box(1000, 405, 40, 25, "Code:");
  codeLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  Input *newCode = new Input(1045, 405, 205, 25);

  Fl_Box *nameLabel = new Fl_Box(1000, 435, 45, 25, "Name:");
  nameLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  Input *newName = new Input(1045, 435, 205, 25);

  Fl_Box *xLabel = new Fl_Box(1000, 465, 25, 25, "X:");
  xLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  IntInput *newX = new IntInput(1025, 465, 100, 25);

  Fl_Box *yLabel = new Fl_Box(1135, 465, 25, 25, "Y:");
  yLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  IntInput *newY = new IntInput(1160, 465, 90, 25);

  Button *loadAirportBtn =
      new Button(1000, 500, 250, 25, "Load Selected for Editing");
  Button *addAirportBtn = new Button(1000, 535, 80, 30, "Add");
  Button *editAirportBtn = new Button(1090, 535, 80, 30, "Update");
  Button *delAirportBtn = new Button(1180, 535, 70, 30, "Delete");

  airportTab->end();

  // Route Tab
  Fl_Group *routeTab = new Fl_Group(990, 35, 280, 655, "Routes");
  routeTab->box(FL_FLAT_BOX);

  routeList = new ListBox(1000, 60, 260, 300, "Route List:");

  // Edit/Add Route Section
  Fl_Box *addRouteLabel = new Fl_Box(1000, 380, 260, 20, "Add / Edit Route:");
  addRouteLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

  routeSourceDd = new Dropdown(1000, 420, 250, 25, "From:");
  routeDestDd = new Dropdown(1000, 470, 250, 25, "To:");

  Fl_Box *costLabel = new Fl_Box(1000, 515, 40, 25, "Cost:");
  costLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  IntInput *newCost = new IntInput(1045, 515, 70, 25);

  Fl_Box *timeLabel = new Fl_Box(1130, 515, 40, 25, "Time:");
  timeLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  IntInput *newTime = new IntInput(1175, 515, 70, 25);

  Button *addRouteBtn = new Button(1000, 555, 80, 30, "Add");
  Button *editRouteBtn = new Button(1090, 555, 80, 30, "Update");
  Button *delRouteBtn = new Button(1180, 555, 70, 30, "Delete");
  Button *loadRouteBtn =
      new Button(1000, 595, 250, 25, "Load Selected for Editing");

  routeTab->end();

  tabs->end();

  refreshDropdowns();
  refreshAirportList();
  refreshRouteList();

  // Setup callbacks
  setupFindRouteCallback(findBtn, fromDd, toDd, prefDd, resultOutput);
  setupCopyCallback(copyBtn, resultOutput);
  setupAirportCallbacks(addAirportBtn, editAirportBtn, delAirportBtn,
                        loadAirportBtn, newCode, newName, newX, newY);
  setupRouteCallbacks(addRouteBtn, editRouteBtn, delRouteBtn, loadRouteBtn,
                      newCost, newTime);

  win->show();
  return app.run();
}
