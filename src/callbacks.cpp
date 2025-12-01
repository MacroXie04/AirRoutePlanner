#include "callbacks.h"
#include "MapWidget.h"
#include <sstream>
#include <cstring>
#include <FL/Fl.H>

using namespace std;

void setupFindRouteCallback(Button* findBtn, Dropdown* fromDd, Dropdown* toDd, 
                            Dropdown* prefDd, Fl_Multiline_Output* resultOutput) {
    findBtn->onClick([=](Widget* w){
        int fromIdx = fromDd->value();
        int toIdx = toDd->value();
        int prefIdx = prefDd->value();

        if (fromIdx < 0 || toIdx < 0) {
            showMessage("Please select valid origin and destination.");
            return;
        }

        Vertex* start = airports[fromIdx];
        Vertex* end = airports[toIdx];

        if (start == end) {
            showMessage("Origin and Destination cannot be the same.");
            return;
        }

        SearchCriteria criteria;
        switch(prefIdx) {
            case 0: criteria = CHEAPEST; break;
            case 1: criteria = FASTEST; break;
            case 2: criteria = LEAST_STOPS; break;
            default: criteria = CHEAPEST; break;
        }

        Waypoint* result = g.search(start, end, criteria);
        currentPath = result; // Store for drawing
        mapCanvas->redraw();  // Trigger redraw

        if (result) {
            stringstream ss;
            ss << "Route: " << start->data << " to " << end->data << "\n";
            ss << "Preference: ";
            if (criteria == CHEAPEST) ss << "Cheapest Price";
            else if (criteria == FASTEST) ss << "Shortest Time";
            else ss << "Least Stops";
            ss << "\n----------------------------------------\n";
            
            // Reconstruct path
            ArrayList<Waypoint*> path;
            Waypoint* curr = result;
            while(curr != nullptr) {
                path.append(curr);
                curr = curr->parent;
            }

            // Print segments
            for(int i = path.size() - 1; i > 0; i--) {
                Waypoint* from = path[i];
                Waypoint* to = path[i-1];
                
                int cost = to->totalCost - from->totalCost;
                int time = to->totalTime - from->totalTime;
                
                ss << from->vertex->data << " -> " << to->vertex->data << "\n";
                ss << "   Cost: $" << cost << ", Time: " << time << "m\n";
            }

            ss << "----------------------------------------\n";
            ss << "Total Cost: $" << result->totalCost << "\n";
            ss << "Total Time: " << result->totalTime << " mins\n";
            ss << "Total Stops: " << result->stops << "\n";

            resultOutput->value(ss.str().c_str());
        } else {
            resultOutput->value("No route found between selected airports.");
        }
    });
}

void setupCopyCallback(Button* copyBtn, Fl_Multiline_Output* resultOutput) {
    copyBtn->onClick([=](Widget* w){
        const char* text = resultOutput->value();
        if (text && strlen(text) > 0) {
            Fl::copy(text, strlen(text), 1); // 1 = clipboard
            showMessage("Copied to clipboard!");
        } else {
            showMessage("Nothing to copy.");
        }
    });
}

void setupAirportCallbacks(Button* addBtn, Button* editBtn, Button* delBtn, Button* loadBtn,
                           Input* codeInput, Input* nameInput, 
                           IntInput* xInput, IntInput* yInput) {
    // Add Airport
    addBtn->onClick([=](Widget* w){
        string code = codeInput->value();
        string name = nameInput->value();
        int x = xInput->value();
        int y = yInput->value();

        if (code.empty() || name.empty()) {
            showMessage("Please fill all airport fields.");
            return;
        }

        if (db.addAirport(code, name, x, y)) {
            showMessage("Airport added successfully!");
            loadData();
            refreshDropdowns();
            refreshAirportList();
            refreshRouteList();
            mapCanvas->redraw();
            
            codeInput->value("");
            nameInput->value("");
            xInput->value(0);
            yInput->value(0);
        } else {
            showMessage("Failed to add airport. Code might be duplicate.");
        }
    });
    
    // Delete Airport
    delBtn->onClick([=](Widget* w){
        int sel = airportList->value();
        if (sel <= 0 || sel > (int)dbAirports.size()) {
            showMessage("Please select an airport to delete.");
            return;
        }
        
        int airportId = dbAirports[sel - 1].id;
        if (db.deleteAirport(airportId)) {
            showMessage("Airport deleted successfully!");
            loadData();
            refreshDropdowns();
            refreshAirportList();
            refreshRouteList();
            mapCanvas->redraw();
        } else {
            showMessage("Failed to delete airport.");
        }
    });
    
    // Load Airport for Editing
    loadBtn->onClick([=](Widget* w){
        int sel = airportList->value();
        if (sel <= 0 || sel > (int)dbAirports.size()) {
            showMessage("Please select an airport to edit.");
            return;
        }
        
        AirportData& a = dbAirports[sel - 1];
        codeInput->value(a.code);
        nameInput->value(a.name);
        xInput->value(a.x);
        yInput->value(a.y);
    });
    
    // Update Airport
    editBtn->onClick([=](Widget* w){
        int sel = airportList->value();
        if (sel <= 0 || sel > (int)dbAirports.size()) {
            showMessage("Please select an airport to update.");
            return;
        }
        
        string code = codeInput->value();
        string name = nameInput->value();
        int x = xInput->value();
        int y = yInput->value();

        if (code.empty() || name.empty()) {
            showMessage("Please fill all airport fields.");
            return;
        }
        
        int airportId = dbAirports[sel - 1].id;
        if (db.updateAirport(airportId, code, name, x, y)) {
            showMessage("Airport updated successfully!");
            loadData();
            refreshDropdowns();
            refreshAirportList();
            refreshRouteList();
            mapCanvas->redraw();
            
            codeInput->value("");
            nameInput->value("");
            xInput->value(0);
            yInput->value(0);
        } else {
            showMessage("Failed to update airport.");
        }
    });
}

void setupRouteCallbacks(Button* addBtn, Button* editBtn, Button* delBtn, Button* loadBtn,
                         IntInput* costInput, IntInput* timeInput) {
    // Add Route
    addBtn->onClick([=](Widget* w){
        int srcIdx = routeSourceDd->value();
        int dstIdx = routeDestDd->value();
        int cost = costInput->value();
        int time = timeInput->value();

        if (srcIdx < 0 || dstIdx < 0) {
            showMessage("Please select source and destination.");
            return;
        }
        
        if (srcIdx == dstIdx) {
            showMessage("Source and Destination cannot be the same.");
            return;
        }

        int srcId = dbAirports[srcIdx].id;
        int dstId = dbAirports[dstIdx].id;

        if (db.addRoute(srcId, dstId, cost, time)) {
            showMessage("Route added successfully!");
            loadData();
            refreshRouteList();
            mapCanvas->redraw();
             
            costInput->value(0);
            timeInput->value(0);
        } else {
            showMessage("Failed to add route.");
        }
    });
    
    // Delete Route
    delBtn->onClick([=](Widget* w){
        int sel = routeList->value();
        if (sel <= 0 || sel > (int)dbRoutes.size()) {
            showMessage("Please select a route to delete.");
            return;
        }
        
        int routeId = dbRoutes[sel - 1].id;
        if (db.deleteRoute(routeId)) {
            showMessage("Route deleted successfully!");
            loadData();
            refreshRouteList();
            mapCanvas->redraw();
        } else {
            showMessage("Failed to delete route.");
        }
    });
    
    // Load Route for Editing
    loadBtn->onClick([=](Widget* w){
        int sel = routeList->value();
        if (sel <= 0 || sel > (int)dbRoutes.size()) {
            showMessage("Please select a route to edit.");
            return;
        }
        
        RouteData& r = dbRoutes[sel - 1];
        // Find the index in dropdown for source and dest
        for (size_t i = 0; i < dbAirports.size(); i++) {
            if (dbAirports[i].id == r.source_id) {
                routeSourceDd->value(i);
            }
            if (dbAirports[i].id == r.dest_id) {
                routeDestDd->value(i);
            }
        }
        costInput->value(r.cost);
        timeInput->value(r.duration);
    });
    
    // Update Route
    editBtn->onClick([=](Widget* w){
        int sel = routeList->value();
        if (sel <= 0 || sel > (int)dbRoutes.size()) {
            showMessage("Please select a route to update.");
            return;
        }
        
        int srcIdx = routeSourceDd->value();
        int dstIdx = routeDestDd->value();
        int cost = costInput->value();
        int time = timeInput->value();

        if (srcIdx < 0 || dstIdx < 0) {
            showMessage("Please select source and destination.");
            return;
        }
        
        if (srcIdx == dstIdx) {
            showMessage("Source and Destination cannot be the same.");
            return;
        }

        int routeId = dbRoutes[sel - 1].id;
        int srcId = dbAirports[srcIdx].id;
        int dstId = dbAirports[dstIdx].id;

        if (db.updateRoute(routeId, srcId, dstId, cost, time)) {
            showMessage("Route updated successfully!");
            loadData();
            refreshRouteList();
            mapCanvas->redraw();
             
            costInput->value(0);
            timeInput->value(0);
        } else {
            showMessage("Failed to update route.");
        }
    });
}

