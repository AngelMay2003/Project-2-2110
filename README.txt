# Project-2-2110

Partial Milestone 1 campus information system. Existing data files are preserved.
This contribution adds the campus graph, BFS, facilities and custom ID hash table. The department tree and final menu are unfinished.

## Build and run this portion
From the repository root, using a C++17 compiler:

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/CampusGraph.cpp tests/graph_demo.cpp -o build/graph_demo
./build/graph_demo data
```

The optional argument is the data directory (default: `data`). Enter a building
name at the prompt; names may contain spaces and lookup ignores case.
The demo displays the graph, BFS order, distances and reachable count.
It is intentionally separate from the final menu and is not `main.cpp`.

## Layout
- `data/`: groupmate's four original input files, unchanged.
- `include/CampusGraph.h`, `src/CampusGraph.cpp`: reusable graph and its two loaders.
- `tests/graph_demo.cpp`: temporary standalone demonstration.
- `tests/graph_tests.cpp`: automated graph regression checks.
- `docs/GRAPH_NOTES.md`: design, integration and scope.
- `docs/GRAPH_TEST_RESULTS.md`: actual checks run for this contribution.

## Run graph tests
```sh
mkdir -p build/fixtures
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/CampusGraph.cpp tests/graph_tests.cpp -o build/graph_tests
./build/graph_tests build/fixtures
```
Use a disposable fixture directory: the tests write buildings.txt and connections.txt there.
Do not compile both test programs into one executable; each has its own main.

## Still to do
Department tree, complete four-file loading order,
final repeating menu, full design document, full milestone
testing, and complete team contributions/commits. The full assignment requires menu options 1-9 and 0 to exit. Source must be verified on CELL before submission.

## Facilities and hash tests
```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/CampusGraph.cpp src/HashTable.cpp src/Facilities.cpp tests/facilities_tests.cpp -o build/facilities_tests
./build/facilities_tests
```
Run from the project root. Facilities depends on an already-loaded CampusGraph.
Use Facilities::load for the third startup file, search for its three-state ID
result, and display for file-order records. The final menu must format search
results and return after errors. This portion does not implement that menu.

The assignment prohibits AI-generated code. This partial implementation is AI-assisted and requires instructor permission for submission.
