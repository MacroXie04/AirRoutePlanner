CREATE TABLE IF NOT EXISTS airports (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    code TEXT NOT NULL UNIQUE,
    name TEXT NOT NULL,
    x INTEGER NOT NULL,
    y INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS routes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    source_id INTEGER NOT NULL,
    dest_id INTEGER NOT NULL,
    cost INTEGER NOT NULL,
    duration INTEGER NOT NULL,
    FOREIGN KEY(source_id) REFERENCES airports(id),
    FOREIGN KEY(dest_id) REFERENCES airports(id)
);

INSERT INTO airports (code, name, x, y) VALUES 
----Please input airports----


INSERT INTO routes (source_id, dest_id, cost, duration) VALUES
----Please input routes----
