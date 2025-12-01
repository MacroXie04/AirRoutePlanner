// =============================================================================
// Test Suite Main Entry Point
// Air Route Planner - Unit Tests
// =============================================================================

#include "test_arraylist.h"
#include "test_graph.h"
#include "test_hashtable.h"
#include "test_linkedlist.h"
#include "test_queue.h"
#include "test_stack.h"

int main(int argc, const char *argv[]) {
  return TestRunner::RunAllTests(argc, const_cast<char **>(argv));
}
