#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "globals.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

// Custom Fl_Box subclass for drawing the map using standard FLTK drawing
class MapWidget : public Fl_Box {
public:
  int offsetX, offsetY;

  MapWidget(int x, int y, int w, int h) : Fl_Box(x, y, w, h) {
    box(FL_DOWN_BOX);
    color(FL_WHITE);
    offsetX = x;
    offsetY = y;
  }

  void draw() override {
    // Draw the box background first
    Fl_Box::draw();

    // Offset for drawing inside the widget
    int ox = x() + 5;
    int oy = y() + 5;

    // Draw connections (edges) first
    fl_color(fl_rgb_color(180, 180, 180)); // Light gray for routes
    for (int i = 0; i < airports.size(); i++) {
      Vertex *u = airports[i];
      for (int j = 0; j < u->edgeList.size(); j++) {
        Vertex *v = u->edgeList[j]->to;
        fl_line_style(FL_SOLID, 1);
        fl_line(ox + u->x, oy + u->y, ox + v->x, oy + v->y);
      }
    }

    // Colors for different paths
    Fl_Color pathColors[] = {FL_RED, fl_rgb_color(0, 150, 0), FL_BLUE,
                             fl_rgb_color(255, 128, 0), FL_MAGENTA};
    int numColors = 5;

    // Highlight all paths if any exist
    for (size_t pathIdx = 0; pathIdx < currentPaths.size(); pathIdx++) {
      const auto &path = currentPaths[pathIdx];
      if (path) {
        // Use different colors for different paths
        fl_color(pathColors[pathIdx % numColors]);
        fl_line_style(FL_SOLID, 3);
        Waypoint *curr = path.get();
        while (curr && curr->parent) {
          fl_line(ox + curr->vertex->x, oy + curr->vertex->y,
                  ox + curr->parent->vertex->x, oy + curr->parent->vertex->y);
          curr = curr->parent.get();
        }
      }
    }
    fl_line_style(FL_SOLID, 0); // Reset

    // Draw airports (nodes)
    for (int i = 0; i < airports.size(); i++) {
      Vertex *v = airports[i];

      // Draw circle
      fl_color(FL_BLUE);
      fl_pie(ox + v->x - 8, oy + v->y - 8, 16, 16, 0, 360);

      // Draw label
      fl_color(FL_BLACK);
      fl_font(FL_HELVETICA, 12);
      fl_draw(v->data.c_str(), ox + v->x + 10, oy + v->y + 4);
    }

    fl_line_style(0); // Reset line style
  }
};

#endif // MAPWIDGET_H
