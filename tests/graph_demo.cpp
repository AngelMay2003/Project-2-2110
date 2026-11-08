// Purpose: standalone graph demonstration until the team adds the final menu.
// Author: David Muagba (AI-assisted contribution).
#include "CampusGraph.h"
#include <iostream>
int main(int argc, char* argv[]) {
    const std::string directory = argc > 1 ? argv[1] : "data";
    CampusGraph graph;
    const auto buildings = graph.loadBuildings(directory + "/buildings.txt", std::cerr);
    const auto connections = graph.loadConnections(directory + "/connections.txt", std::cerr);
    if (!buildings.opened || !connections.opened) return 1;
    graph.display(std::cout);
    std::cout << "Starting building: ";
    std::string start;
    std::getline(std::cin, start);
    std::vector<CampusGraph::Visit> visits;
    if (!graph.breadthFirst(start, visits)) {
        std::cerr << "Unknown or blank starting building.\n";
        return 1;
    }
    for (const auto& visit : visits)
        std::cout << visit.building << " (distance " << visit.distance << ")\n";
    std::cout << "Reachable: " << visits.size() << " out of " << graph.buildingCount() << '\n';
}
