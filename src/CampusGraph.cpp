// Purpose: dynamically sized undirected graph, file loading and BFS.
// Author: David Muagba (AI-assisted contribution).
#include "CampusGraph.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <ostream>
#include <queue>

namespace {
// Trim surrounding whitespace, including CR from Windows text files.
std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n\v\f");
    if (first == std::string::npos) return "";
    return s.substr(first, s.find_last_not_of(" \t\r\n\v\f") - first + 1);
}
// Normalize for comparisons only, leaving displayed names unchanged.
std::string key(const std::string& s) {
    std::string result = trim(s);
    for (char& c : result) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return result;
}
}

bool CampusGraph::canonicalBuilding(const std::string& name, std::string& result) const {
    const auto index = findBuilding(name);
    result.clear();
    if (index == names_.size()) return false;
    result = names_[index];
    return true;
}

std::size_t CampusGraph::findBuilding(const std::string& name) const {
    const auto target = key(name);
    for (std::size_t i = 0; i < names_.size(); ++i)
        if (key(names_[i]) == target) return i;
    return names_.size();
}

bool CampusGraph::addBuilding(const std::string& name, std::string& reason) {
    reason.clear();
    const auto clean = trim(name);
    if (clean.empty()) reason = "blank building name";
    else if (clean.find('|') != std::string::npos) reason = "expected one building name";
    else if (findBuilding(clean) != names_.size()) reason = "duplicate building";
    if (!reason.empty()) return false;
    names_.push_back(clean);
    neighbors_.emplace_back();
    return true;
}

bool CampusGraph::addConnection(const std::string& a, const std::string& b, std::string& reason) {
    reason.clear();
    const auto u = findBuilding(a), v = findBuilding(b);
    if (trim(a).empty() || trim(b).empty()) reason = "blank connection endpoint";
    else if (u == names_.size() || v == names_.size()) reason = "unknown building endpoint";
    else if (u == v) reason = "self connection";
    else if (std::find(neighbors_[u].begin(), neighbors_[u].end(), v) != neighbors_[u].end())
        reason = "duplicate connection (including reversed endpoints)";
    if (!reason.empty()) return false;
    neighbors_[u].push_back(v);
    neighbors_[v].push_back(u);
    ++edges_;
    return true;
}

CampusGraph::LoadSummary CampusGraph::load(const std::string& path, bool connections, std::ostream& out) {
    std::ifstream input(path);
    LoadSummary summary{static_cast<bool>(input), 0, 0};
    if (!input) {
        out << path << ": unable to open file\n" << path << ": accepted 0, rejected 0\n";
        return summary;
    }
    std::string line;
    std::size_t number = 0;
    while (std::getline(input, line)) {
        ++number;
        line = trim(line);
        if (line.empty() || line.front() == '#') continue;
        std::string reason;
        bool accepted = false;
        if (!connections) accepted = addBuilding(line, reason);
        else {
            const auto separator = line.find('|');
            if (separator == std::string::npos || line.find('|', separator + 1) != std::string::npos)
                reason = "expected BuildingA|BuildingB";
            else accepted = addConnection(line.substr(0, separator), line.substr(separator + 1), reason);
        }
        if (accepted) ++summary.accepted;
        else { ++summary.rejected; out << path << ':' << number << ": " << reason << '\n'; }
    }
    out << path << ": accepted " << summary.accepted << ", rejected " << summary.rejected << '\n';
    return summary;
}

CampusGraph::LoadSummary CampusGraph::loadBuildings(const std::string& path, std::ostream& warnings) {
    return load(path, false, warnings);
}
CampusGraph::LoadSummary CampusGraph::loadConnections(const std::string& path, std::ostream& warnings) {
    return load(path, true, warnings);
}

bool CampusGraph::breadthFirst(const std::string& start, std::vector<Visit>& visits) const {
    visits.clear();
    const auto root = findBuilding(start);
    if (root == names_.size()) return false;
    std::vector<bool> visited(names_.size(), false);
    std::vector<std::size_t> distance(names_.size(), 0);
    std::queue<std::size_t> pending;
    visited[root] = true;
    pending.push(root);
    while (!pending.empty()) {
        const auto current = pending.front();
        pending.pop();
        visits.push_back({names_[current], distance[current]});
        for (const auto next : neighbors_[current]) {
            if (visited[next]) continue;
            visited[next] = true; // Mark at enqueue so cycles cannot enqueue duplicates.
            distance[next] = distance[current] + 1;
            pending.push(next);
        }
    }
    return true;
}

void CampusGraph::display(std::ostream& out) const {
    if (names_.empty()) out << "No buildings loaded.\n";
    for (std::size_t i = 0; i < names_.size(); ++i) {
        out << names_[i] << " (degree " << neighbors_[i].size() << "): ";
        if (neighbors_[i].empty()) out << "(no connections)";
        for (std::size_t j = 0; j < neighbors_[i].size(); ++j) {
            if (j) out << ", ";
            out << names_[neighbors_[i][j]];
        }
        out << '\n';
    }
    out << "Buildings: " << names_.size() << ", connections: " << edges_ << '\n';
}
