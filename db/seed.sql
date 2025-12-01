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
('SFO', 'San Francisco', 50, 200),
('NYC', 'New York', 450, 150),
('LAX', 'Los Angeles', 60, 280),
('ORD', 'Chicago', 300, 150),
('DFW', 'Dallas', 250, 300),
('DEN', 'Denver', 180, 200),
('ATL', 'Atlanta', 350, 280),
('SEA', 'Seattle', 80, 50),
('MIA', 'Miami', 400, 400),
('BOS', 'Boston', 470, 100),
('TPE', 'Taiwan Taoyuan', 550, 250);


INSERT INTO routes (source_id, dest_id, cost, duration) VALUES
(1, 3, 150, 90),   -- SFO-LAX
(3, 1, 150, 90),
(1, 8, 200, 120),  -- SFO-SEA
(8, 1, 200, 120),
(3, 8, 250, 150),  -- LAX-SEA
(8, 3, 250, 150),
(3, 6, 180, 140),  -- LAX-DEN
(6, 3, 180, 140),
(1, 6, 220, 160),  -- SFO-DEN
(6, 1, 220, 160),

(6, 4, 160, 150),  -- DEN-ORD
(4, 6, 160, 150),
(6, 5, 140, 110),  -- DEN-DFW
(5, 6, 140, 110),
(5, 4, 170, 140),  -- DFW-ORD
(4, 5, 170, 140),
(5, 7, 130, 120),  -- DFW-ATL
(7, 5, 130, 120),
(4, 7, 150, 120),  -- ORD-ATL
(7, 4, 150, 120),

(2, 10, 120, 80),  -- NYC-BOS
(10, 2, 120, 80),
(2, 9, 250, 180),  -- NYC-MIA
(9, 2, 250, 180),
(7, 9, 140, 110),  -- ATL-MIA
(9, 7, 140, 110),
(7, 2, 200, 130),  -- ATL-NYC
(2, 7, 200, 130),
(4, 2, 210, 130),  -- ORD-NYC
(2, 4, 210, 130),

(1, 2, 450, 330),  -- SFO-NYC (Long haul)
(2, 1, 450, 330),
(3, 2, 420, 320),  -- LAX-NYC
(2, 3, 420, 320),
(3, 7, 380, 270),  -- LAX-ATL
(7, 3, 380, 270),
(8, 4, 300, 240),  -- SEA-ORD
(4, 8, 300, 240),
(8, 2, 480, 340),  -- SEA-NYC
(2, 8, 480, 340),
(5, 2, 280, 200),  -- DFW-NYC
(2, 5, 280, 200),
(9, 10, 260, 210), -- MIA-BOS
(10, 9, 260, 210),

(11, 1, 680, 780),  -- TPE-SFO (Taiwan routes)
(1, 11, 680, 780),
(11, 3, 650, 750),  -- TPE-LAX
(3, 11, 650, 750),
(11, 2, 850, 960),  -- TPE-NYC
(2, 11, 850, 960);

