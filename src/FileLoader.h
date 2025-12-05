#ifndef FILELOADER_H
#define FILELOADER_H

#include <Graph.h>
#include <cmath>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct AirportInfo {
  std::string name;
  int x;
  int y;
};

struct RouteInfo {
  int source_idx;
  int dest_idx;
  int duration;
  int cost;
};

class FileLoader {
private:
  // Calculate distance between two points
  static double distance(int x1, int y1, int x2, int y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  }

  // Check if a position is far enough from all existing airports
  static bool isValidPosition(int x, int y,
                              const std::vector<AirportInfo> &airports,
                              int minDistance) {
    for (const auto &airport : airports) {
      if (distance(x, y, airport.x, airport.y) < minDistance) {
        return false;
      }
    }
    return true;
  }

public:
  static std::vector<AirportInfo>
  loadAirports(const std::string &filepath = "assets/edges.txt") {
    std::vector<AirportInfo> airports;
    std::ifstream file(filepath);

    if (!file.is_open()) {
      return airports;
    }

    // Random number generation for coordinates
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(50, 620);

    // Minimum distance between cities (in pixels)
    const int MIN_DISTANCE = 100;
    const int MAX_RETRIES = 100;

    std::string line;
    while (std::getline(file, line)) {
      if (line.empty())
        continue;

      // Trim whitespace
      size_t first = line.find_first_not_of(" \t\r\n");
      size_t last = line.find_last_not_of(" \t\r\n");
      if (first == std::string::npos)
        continue;

      std::string name = line.substr(first, (last - first + 1));

      AirportInfo info;
      info.name = name;

      // Try to find a valid position with minimum distance from other airports
      int retries = 0;
      do {
        info.x = dist(rng);
        info.y = dist(rng);
        retries++;
      } while (!isValidPosition(info.x, info.y, airports, MIN_DISTANCE) &&
               retries < MAX_RETRIES);

      airports.push_back(info);
    }

    file.close();
    return airports;
  }

  static std::vector<RouteInfo>
  loadRoutes(const std::string &filepath = "assets/vertices.txt") {
    std::vector<RouteInfo> routes;
    std::ifstream file(filepath);

    if (!file.is_open()) {
      return routes;
    }

    std::string line;
    while (std::getline(file, line)) {
      if (line.empty())
        continue;

      std::stringstream ss(line);
      std::string segment;
      std::vector<std::string> parts;

      while (std::getline(ss, segment, ',')) {
        parts.push_back(segment);
      }

      if (parts.size() >= 4) {
        try {
          RouteInfo r;
          r.source_idx = std::stoi(parts[0]);
          r.dest_idx = std::stoi(parts[1]);
          r.duration = std::stoi(parts[2]);
          r.cost = std::stoi(parts[3]);
          routes.push_back(r);
        } catch (...) {
          // Ignore malformed lines
        }
      }
    }

    file.close();
    return routes;
  }

  // Load data directly into Graph and return airports list
  static void loadIntoGraph(Graph &g, ArrayList<Vertex *> &airports) {
    // Load airports
    std::vector<AirportInfo> airportInfos = loadAirports();

    // Create vertices
    for (const auto &info : airportInfos) {
      Vertex *v = new Vertex(info.name, info.x, info.y);
      g.addVertex(v);
      airports.append(v);
    }

    // Load routes
    std::vector<RouteInfo> routeInfos = loadRoutes();

    // Create bidirectional edges
    for (const auto &r : routeInfos) {
      if (r.source_idx >= 0 && r.source_idx < airports.size() &&
          r.dest_idx >= 0 && r.dest_idx < airports.size()) {
        // Forward direction
        g.addDirectedEdge(airports[r.source_idx], airports[r.dest_idx], r.cost,
                          r.duration);
        // Reverse direction (bidirectional)
        g.addDirectedEdge(airports[r.dest_idx], airports[r.source_idx], r.cost,
                          r.duration);
      }
    }
  }
};

#endif // FILELOADER_H
