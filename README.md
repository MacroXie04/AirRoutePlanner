# Air Route Planner

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20STEAMplug-lightgrey.svg)](https://github.com/MacroXie04/DeepFive)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

A desktop application for planning optimal air travel routes between airports. Built with C++ and the Bobcat UI framework, this application uses graph algorithms to find the best flight itineraries based on user preferences.

![Air Route Planner Screenshot](Screenshot.png)

## Features

- **Route Planning**: Find optimal routes between any two airports
- **Multiple Optimization Criteria**:
  - **Cheapest Price** - Minimize total flight cost
  - **Shortest Time** - Minimize total travel duration
  - **Least Stops** - Minimize number of connections
- **Interactive Map Visualization**: Visual representation of airports and routes with highlighted paths
- **Data Management**: Add, edit, and delete airports and routes through the GUI
- **SQLite Database**: Persistent storage for airports and routes data
- **Copy to Clipboard**: Export itinerary details for sharing

## Prerequisites

### macOS
```bash
brew install fltk sqlite3
```

### Linux (Debian/Ubuntu)
```bash
sudo apt-get install libfltk1.3-dev libsqlite3-dev
```

## Getting Started

### Build the Application
```bash
make
```

### Initialize the Database
```bash
make init_db
```

### Run the Application
```bash
make run
```

## Usage

### Finding a Route

1. Select a departure airport from the **From** dropdown
2. Select a destination airport from the **To** dropdown
3. Choose your preference:
   - **Cheapest Price**: Finds the route with the lowest total cost
   - **Shortest Time**: Finds the route with the shortest total travel time
   - **Least Number of Stops**: Finds the route with the fewest connections
4. Click **Find Route** to calculate the optimal itinerary

### Managing Data

#### Airports Tab
- View all airports in the system
- Add new airports with code, name, and map coordinates (X, Y)
- Edit existing airports by selecting and clicking "Load Selected for Editing"
- Delete airports (will also remove associated routes)

#### Routes Tab
- View all available flight routes
- Add new routes between airports with cost and duration
- Update or delete existing routes

## Project Structure

```
AirRoutePlanner/
├── src/
│   ├── main.cpp          # Application entry point and UI setup
│   ├── callbacks.cpp     # Event handlers for UI interactions
│   ├── callbacks.h       # Callback function declarations
│   ├── globals.cpp       # Global state and data loading
│   ├── globals.h         # Global variable declarations
│   └── MapWidget.h       # Custom widget for map visualization
├── inc/
│   ├── Graph.h           # Graph data structure with search algorithms
│   ├── ArrayList.h       # Dynamic array implementation
│   ├── LinkedList.h      # Linked list implementation
│   ├── Queue.h           # Queue for BFS
│   ├── Stack.h           # Stack for DFS
│   └── HashTable.h       # Hash table for visited nodes
├── bobcat_ui/            # Bobcat UI Framework (FLTK wrapper)\
├── test/
│   └── test_*.cpp        # Unit tests
├── bin/                  # Compiled binaries (generated)
├── objects/              # Object files (generated)
├── Makefile              # Build configuration
└── Screenshot.png        # Application screenshot
```

## Algorithm Details

The application uses **Uniform Cost Search (UCS)**, a variant of Dijkstra's algorithm, to find optimal paths. The search criteria determines how the algorithm prioritizes paths:

| Criteria | Cost Function | Behavior |
|----------|---------------|----------|
| Cheapest Price | Total flight cost ($) | Finds the most affordable route |
| Shortest Time | Total duration (minutes) | Finds the fastest route |
| Least Stops | Number of connections | Finds the most direct route |

The graph structure stores airports as vertices with edges representing direct flights. Each edge contains:
- **Cost**: Price of the flight in dollars
- **Duration**: Flight time in minutes

## Sample Data

The application comes pre-loaded with 11 airports including:
- **US Domestic**: SFO, LAX, SEA, DEN, DFW, ORD, ATL, NYC, BOS, MIA
- **International**: TPE (Taiwan Taoyuan)

Routes are designed to demonstrate different optimization scenarios:
- Direct flights may be faster but more expensive
- Connecting flights may offer cheaper alternatives
- Some destinations require multiple stops

## Makefile Targets

| Command | Description |
|---------|-------------|
| `make` | Build the application |
| `make run` | Build and run the application |
| `make init_db` | Initialize/reset the database |
| `make clean` | Remove compiled files and database |
| `make test` | Run unit tests |
| `make lint` | Check code formatting |
| `make format` | Auto-format source code |