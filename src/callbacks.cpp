#include "callbacks.h"
#include "MapWidget.h"
#include <FL/Fl.H>
#include <cstring>
#include <sstream>

using namespace std;

void setupFindRouteCallback(Button *findBtn, Dropdown *fromDd, Dropdown *toDd,
                            Dropdown *prefDd, Memo *resultOutput) {
  findBtn->onClick([=](Widget *w) {
    int fromIdx = fromDd->value();
    int toIdx = toDd->value();
    int prefIdx = prefDd->value();

    if (fromIdx < 0 || toIdx < 0) {
      showMessage("Please select valid origin and destination.");
      return;
    }

    Vertex *start = airports[fromIdx];
    Vertex *end = airports[toIdx];

    if (start == end) {
      showMessage("Origin and Destination cannot be the same.");
      return;
    }

    SearchCriteria criteria;
    switch (prefIdx) {
    case 0:
      criteria = CHEAPEST;
      break;
    case 1:
      criteria = FASTEST;
      break;
    case 2:
      criteria = LEAST_STOPS;
      break;
    default:
      criteria = CHEAPEST;
      break;
    }

    // Search for ALL optimal paths
    std::vector<std::shared_ptr<Waypoint>> results =
        g.searchAll(start, end, criteria);
    currentPaths = results; // Store for drawing
    mapCanvas->redraw();    // Trigger redraw

    if (!results.empty()) {
      stringstream ss;
      ss << "Route: " << start->data << " to " << end->data << "\n";
      ss << "Preference: ";
      if (criteria == CHEAPEST)
        ss << "Cheapest Price";
      else if (criteria == FASTEST)
        ss << "Shortest Time";
      else
        ss << "Least Stops";
      ss << "\n";
      ss << "Found " << results.size() << " optimal route(s)\n";
      ss << "========================================\n\n";

      int pathNum = 1;
      for (const auto &result : results) {
        ss << "--- Route " << pathNum++ << " ---\n";

        // Reconstruct path
        ArrayList<Waypoint *> path;
        Waypoint *curr = result.get();
        while (curr != nullptr) {
          path.append(curr);
          curr = curr->parent.get();
        }

        // Print segments
        for (int i = path.size() - 1; i > 0; i--) {
          Waypoint *from = path[i];
          Waypoint *to = path[i - 1];

          int cost = to->totalCost - from->totalCost;
          int time = to->totalTime - from->totalTime;

          ss << from->vertex->data << " -> " << to->vertex->data << "\n";
          ss << "   Cost: $" << cost << ", Time: " << time << "h\n";
        }

        ss << "Total Cost: $" << result->totalCost << "\n";
        ss << "Total Time: " << result->totalTime << " h\n";
        ss << "Total Stops: " << result->stops << "\n\n";
      }

      resultOutput->value(ss.str());
    } else {
      resultOutput->value("No route found between selected airports.");
    }
  });
}

void setupCopyCallback(Button *copyBtn, Memo *resultOutput) {
  copyBtn->onClick([=](Widget *w) {
    string text = resultOutput->value();
    if (!text.empty()) {
      Fl::copy(text.c_str(), text.length(), 1); // 1 = clipboard
      showMessage("Copied to clipboard!");
    } else {
      showMessage("Nothing to copy.");
    }
  });
}
