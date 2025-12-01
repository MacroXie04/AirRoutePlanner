# ========================================================================================= #
#  Makefile       											                    			#
#  Hongzhe Xie                      														#
#  CSE 030 Data Structure 															   		#
#  University of California, Merced    												   		#
# ========================================================================================= #

# ===================================== PROJECT CONFIG ==================================== #
SRC_DIR      := src
TEST_DIR     := test
OBJ_DIR      := objects
LOCAL_BIN    := bin
APP          := app
MAIN         := main
TEST         := test
HEADERS      := $(shell find $(SRC_DIR) -name '*.h')
SRC          := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ          := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_SRC     := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ     := $(TEST_SRC:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/test/%.o)
LOCAL_BIN_DIR:= $(LOCAL_BIN)
BIN_DIR      := $(LOCAL_BIN)
OUT          := $(BIN_DIR)/$(APP)
TEST_OUT     := $(BIN_DIR)/$(TEST)
DB_FILE      := db/data.db
DB_SEED      := db/seed.sql

MAKEFLAGS   += --no-print-directory

# ================================ PLATFORM DETECTION ====================================== #
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  # macOS
  CXX        := clang++
  CXXFLAGS   := -Wall `fltk-config --cxxflags` -std=c++17 -DGL_SILENCE_DEPRECATION -I. -Iinc -Ibobcat_ui -Iigloo
  GLFLAGS    := -framework OpenGL
  LDFLAGS    := `fltk-config --ldflags` -lfltk_gl -lfltk_images $(GLFLAGS) -lsqlite3
else
  # assume Linux
  CXX        := g++
  CXXFLAGS   := -Wall `fltk-config --cxxflags` -std=c++17 -I. -Iinc -Ibobcat_ui -Iigloo
  GLFLAGS    := -lGL -lGLU
  LDFLAGS    := `fltk-config --use-gl --use-images --ldflags` $(GLFLAGS) -lsqlite3
endif

# ==================================== RULES ================================================ #

all: $(OUT)

$(OUT): $(OBJ) | $(OBJ_DIR) $(LOCAL_BIN_DIR)
	$(CXX) $(OBJ) -o $(OUT) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LOCAL_BIN_DIR):
	mkdir -p $(LOCAL_BIN_DIR)

run: all
	@if [ ! -f $(DB_FILE) ]; then \
		echo "Database not found at $(DB_FILE)."; \
		read -p "Do you want to create a new database? [y/N] " answer; \
		if [ "$$answer" = "y" ] || [ "$$answer" = "Y" ]; then \
			sqlite3 $(DB_FILE) < $(DB_SEED); \
			echo "Database initialized at $(DB_FILE)"; \
		else \
			echo "Cannot run without database. Exiting."; \
			exit 1; \
		fi; \
	fi
	@if command -v clear >/dev/null 2>&1 && [ -n "$$TERM" ] && [ "$$TERM" != "dumb" ]; then clear; fi
	@$(LOCAL_BIN_DIR)/$(APP)

init_db:
	@if [ -f $(DB_FILE) ]; then \
		read -p "Database already exists. Do you want to overwrite it? [y/N] " answer; \
		if [ "$$answer" = "y" ] || [ "$$answer" = "Y" ]; then \
			rm -f $(DB_FILE); \
			sqlite3 $(DB_FILE) < $(DB_SEED); \
			echo "Database initialized at $(DB_FILE)"; \
		else \
			echo "Operation cancelled."; \
		fi; \
	else \
		sqlite3 $(DB_FILE) < $(DB_SEED); \
		echo "Database initialized at $(DB_FILE)"; \
	fi

$(TEST_OUT): $(OBJ) $(TEST_OBJ) | $(BIN_DIR) $(LOCAL_BIN_DIR)
	$(CXX) $(filter-out $(OBJ_DIR)/$(MAIN).o,$(OBJ)) $(TEST_OBJ) -o $(TEST_OUT) $(LDFLAGS)

test: $(TEST_OUT)
	$(LOCAL_BIN_DIR)/$(TEST) --output=color || true

autograde: clean $(TEST_OUT)
	xvfb-run $(LOCAL_BIN_DIR)/$(TEST) || true

$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(LOCAL_BIN_DIR)/$(APP) $(LOCAL_BIN_DIR)/$(TEST)
	rm -rf $(OBJ_DIR)
	rmdir $(LOCAL_BIN_DIR) 2> /dev/null || true
	rm -f $(DB_FILE)

.PHONY: all run test autograde clean lint memcheck format init_db

lint:
	clang-format --dry-run --Werror $(SRC) $(HEADERS) $(TEST_SRC)

format:
	clang-format -i $(SRC) $(HEADERS) $(TEST_SRC)

memcheck: $(TEST_OUT)
	valgrind --leak-check=full --error-exitcode=1 $(TEST_OUT)
