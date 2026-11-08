// Purpose: regression checks for graph invariants, BFS and file validation.
// Author: David Muagba (AI-assisted contribution).
#include "CampusGraph.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
int main(int argc, char* argv[]) {
    assert(argc == 2); // Caller supplies a disposable fixture directory.
    const std::string dir = argv[1];
    CampusGraph graph;
    std::string reason;
    for (const auto* name : {"A", "B", "C", "D", "Isolated"}) assert(graph.addBuilding(name, reason));
    assert(!graph.addBuilding(" a ", reason));
    assert(!graph.addBuilding("  ", reason));
    assert(graph.addConnection("a", "B", reason));
    assert(graph.addConnection("A", "C", reason));
    assert(graph.addConnection("B", "D", reason));
    assert(graph.addConnection("C", "D", reason));
    assert(!graph.addConnection("b", "a", reason));
    assert(!graph.addConnection("A", "a", reason));
    assert(!graph.addConnection("A", "missing", reason));
    assert(graph.edgeCount() == 4);
    std::vector<CampusGraph::Visit> visits;
    assert(graph.breadthFirst(" a ", visits));
    assert(visits.size() == 4);
    assert(visits[0].building == "A" && visits[0].distance == 0);
    assert(visits[1].building == "B" && visits[1].distance == 1);
    assert(visits[2].building == "C" && visits[2].distance == 1);
    assert(visits[3].building == "D" && visits[3].distance == 2);
    assert(graph.breadthFirst("isolated", visits) && visits.size() == 1 && visits[0].distance == 0);
    assert(!graph.breadthFirst("unknown", visits) && visits.empty());
    std::ostringstream output;
    graph.display(output);
    assert(output.str().find("A (degree 2): B, C") != std::string::npos);
    assert(output.str().find("Isolated (degree 0): (no connections)") != std::string::npos);
    { std::ofstream f(dir + "/buildings.txt", std::ios::binary);
      f << " # comment\r\n\r\n A \r\n a\r\nB\r\nbad|record\r\n"; }
    { std::ofstream f(dir + "/connections.txt", std::ios::binary);
      f << "# comment\r\n A | B \r\nB|a\r\nA|A\r\nA|Missing\r\nA|B|C\r\n|B\r\n"; }
    CampusGraph loaded;
    auto summary = loaded.loadBuildings(dir + "/buildings.txt", output);
    assert(summary.opened && summary.accepted == 2 && summary.rejected == 2);
    summary = loaded.loadConnections(dir + "/connections.txt", output);
    assert(summary.opened && summary.accepted == 1 && summary.rejected == 5);
    assert(output.str().find("buildings.txt:4: duplicate building") != std::string::npos);
    assert(output.str().find("connections.txt:3: duplicate connection") != std::string::npos);
    assert(!loaded.loadBuildings(dir + "/absent.txt", output).opened);
    CampusGraph empty;
    assert(!empty.breadthFirst("A", visits));
    std::cout << "PASS: insertion order, undirected edges, validation, BFS, isolated/unknown starts, CRLF and missing file\n";
}
