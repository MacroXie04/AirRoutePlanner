#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

struct AirportData {
    int id;
    std::string code;
    std::string name;
    int x;
    int y;
};

struct RouteData {
    int id;
    int source_id;
    int dest_id;
    int cost;
    int duration;
};

class DatabaseManager {
    sqlite3 *db;
    std::string dbPath;

public:
    DatabaseManager(const std::string& path) : db(nullptr), dbPath(path) {
        open();
    }

    ~DatabaseManager() {
        close();
    }

    bool open() {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        return true;
    }

    void close() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    std::vector<AirportData> getAirports() {
        std::vector<AirportData> airports;
        const char *sql = "SELECT id, code, name, x, y FROM airports ORDER BY code";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                AirportData a;
                a.id = sqlite3_column_int(stmt, 0);
                a.code = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                a.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                a.x = sqlite3_column_int(stmt, 3);
                a.y = sqlite3_column_int(stmt, 4);
                airports.push_back(a);
            }
        } else {
            std::cerr << "Failed to fetch airports: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return airports;
    }

    std::vector<RouteData> getRoutes() {
        std::vector<RouteData> routes;
        const char *sql = "SELECT id, source_id, dest_id, cost, duration FROM routes";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                RouteData r;
                r.id = sqlite3_column_int(stmt, 0);
                r.source_id = sqlite3_column_int(stmt, 1);
                r.dest_id = sqlite3_column_int(stmt, 2);
                r.cost = sqlite3_column_int(stmt, 3);
                r.duration = sqlite3_column_int(stmt, 4);
                routes.push_back(r);
            }
        } else {
            std::cerr << "Failed to fetch routes: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return routes;
    }

    bool addAirport(const std::string& code, const std::string& name, int x, int y) {
        std::string sql = "INSERT INTO airports (code, name, x, y) VALUES (?, ?, ?, ?)";
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;
        
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, x);
        sqlite3_bind_int(stmt, 4, y);

        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }

    bool addRoute(int sourceId, int destId, int cost, int duration) {
        std::string sql = "INSERT INTO routes (source_id, dest_id, cost, duration) VALUES (?, ?, ?, ?)";
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;
        
        sqlite3_bind_int(stmt, 1, sourceId);
        sqlite3_bind_int(stmt, 2, destId);
        sqlite3_bind_int(stmt, 3, cost);
        sqlite3_bind_int(stmt, 4, duration);

        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }
    
    bool deleteAirport(int id) {
        // First delete all routes associated with this airport
        std::string sqlRoutes = "DELETE FROM routes WHERE source_id = ? OR dest_id = ?";
        sqlite3_stmt *stmtRoutes;
        if (sqlite3_prepare_v2(db, sqlRoutes.c_str(), -1, &stmtRoutes, 0) != SQLITE_OK) return false;
        sqlite3_bind_int(stmtRoutes, 1, id);
        sqlite3_bind_int(stmtRoutes, 2, id);
        sqlite3_step(stmtRoutes);
        sqlite3_finalize(stmtRoutes);
        
        // Then delete the airport
        std::string sql = "DELETE FROM airports WHERE id = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;
        sqlite3_bind_int(stmt, 1, id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }
    
    bool deleteRoute(int id) {
        std::string sql = "DELETE FROM routes WHERE id = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;
        sqlite3_bind_int(stmt, 1, id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }
    
    bool updateAirport(int id, const std::string& code, const std::string& name, int x, int y) {
        std::string sql = "UPDATE airports SET code = ?, name = ?, x = ?, y = ? WHERE id = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, x);
        sqlite3_bind_int(stmt, 4, y);
        sqlite3_bind_int(stmt, 5, id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }
    
    bool updateRoute(int id, int sourceId, int destId, int cost, int duration) {
        std::string sql = "UPDATE routes SET source_id = ?, dest_id = ?, cost = ?, duration = ? WHERE id = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;
        sqlite3_bind_int(stmt, 1, sourceId);
        sqlite3_bind_int(stmt, 2, destId);
        sqlite3_bind_int(stmt, 3, cost);
        sqlite3_bind_int(stmt, 4, duration);
        sqlite3_bind_int(stmt, 5, id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }
};

#endif

