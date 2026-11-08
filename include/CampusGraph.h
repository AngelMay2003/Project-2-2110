// Purpose: Campus graph and BFS.
// Author: David Muagba (AI-assisted contribution).
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H
#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

class CampusGraph {
public:
    // Get the stored building name.
    bool canonicalBuilding(const std::string& name, std::string& result) const;
    struct Visit { std::string building; std::size_t distance; };
    struct LoadSummary { bool opened; std::size_t accepted; std::size_t rejected; };
    // Add a building unless it is invalid or already exists.
    bool addBuilding(const std::string& name, std::string& reason);
    // Add a valid two-way connection.
    bool addConnection(const std::string& a, const std::string& b, std::string& reason);
    // Load records and report rejected lines.
    LoadSummary loadBuildings(const std::string& path, std::ostream& warnings);
    LoadSummary loadConnections(const std::string& path, std::ostream& warnings);
    // Find reachable buildings and distances. Return false for an unknown start.
    bool breadthFirst(const std::string& start, std::vector<Visit>& visits) const;
    // Show neighbors, degrees and totals.
    void display(std::ostream& out) const;
    std::size_t buildingCount() const { return names_.size(); }
    std::size_t edgeCount() const { return edges_; }
private:
    std::vector<std::string> names_;
    std::vector<std::vector<std::size_t>> neighbors_;
    std::size_t edges_ = 0;
    std::size_t findBuilding(const std::string& name) const;
    LoadSummary load(const std::string& path, bool connections, std::ostream& warnings);
};
#endif
