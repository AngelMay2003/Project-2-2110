// Purpose: Graph loading, display and BFS.
#include "CampusGraph.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <ostream>
#include <queue>

namespace {
// Remove outer whitespace and Windows line endings.
std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n\v\f");
    if (first == std::string::npos) return "";
    return s.substr(first, s.find_last_not_of(" \t\r\n\v\f") - first + 1);
}
// Ignore case when comparing names.
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
    if (trim(a).empty() || trim(b).empty()) reason = "empty building name";
    else if (u == names_.size() || v == names_.size()) reason = "unknown building";
    else if (u == v) reason = "self connection";
    else if (std::find(neighbors_[u].begin(), neighbors_[u].end(), v) != neighbors_[u].end())
        reason = "duplicate connection (either direction)";
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
                reason = "expected two building fields";
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
            visited[next] = true; // Mark before enqueue to avoid repeats.
            distance[next] = distance[current] + 1;
            pending.push(next);
        }
    }
    return true;
}

bool CampusGraph::shortestPath(const std::string& from, const std::string& to, std::vector<std::string>& path) const
{
    path.clear();
    const auto source = findBuilding(from);
    const auto target = findBuilding(to);
    if (source == names_.size() || target == names_.size())
        return false;
    
    const auto none = names_.size();
    std::vector<bool> visited(names_.size(), false);
    std::vector<std::size_t> parents(names_.size(), none);
    std::queue<std::size_t> pending;
    visited[source] = true;
    pending.push(source);
    
    while (!pending.empty() && !visited[target])
    {
        const auto current = pending.front();
        pending.pop();
        for (const auto next : neighbors_[current])
        {
            if (visited[next])
                continue;
            visited[next] = true; // Mark before enqueue to avoid repeats.
            parents[next] = current;
            pending.push(next);
        }
    }
    
    if (!visited[target]) return false;

    for (auto at = target; at !=none; at = parents[at])
    {
        path.push_back(names_[at]);
    }
    
    std::reverse(path.begin(), path.end());
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

void CampusGraph::connectivityReport(std::ostream& out) const {
    out << "Graph Connectivity Report\n";
    out << "Total buildings: " << buildingCount() << '\n';
    out << "Total connections: " << edgeCount() << '\n';
    if (names_.empty()) out << "No buildings loaded.\n";

    std::size_t highestDegree = 0;
    for (const auto& neighbors : neighbors_) {
        highestDegree = std::max(highestDegree, neighbors.size());
    }

    std::string highest, isolated;
    for (std::size_t i = 0; i < names_.size(); ++i) {
        if (neighbors_[i].size() == highestDegree) {
            if (!highest.empty()) highest += ", ";
            highest += names_[i];
        }
        if (neighbors_[i].empty()) {
            if (!isolated.empty()) isolated += ", ";
            isolated += names_[i];
        }
    }
    out << "Highest degree (" << highestDegree << "): " << (highest.empty() ? "None" : highest) << '\n';
    out << "Isolated buildings: " << (isolated.empty() ? "None" : isolated) << '\n';

    std::size_t components = 0;
    std::vector<bool> visited(names_.size(), false);
    std::queue<std::size_t> pending;
    // One traversal for each component.
    for (std::size_t start = 0; start < names_.size(); ++start) {
        if (visited[start]) continue;
        ++components;
        visited[start] = true;
        pending.push(start);
        while (!pending.empty()) {
            const auto current = pending.front();
            pending.pop();
            for (const auto next : neighbors_[current]) {
                if (visited[next]) continue;
                visited[next] = true;
                pending.push(next);
            }
        }
    }
    out << "Connected components: " << components << '\n';
}
