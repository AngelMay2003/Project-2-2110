// Purpose: Facility loading and lookup.
// Author: David Muagba (AI-assisted).
#include "Facilities.h"
#include <fstream>
#include <ostream>
namespace {
// Trim outer whitespace.
std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n\v\f");
    return first == std::string::npos ? "" : s.substr(first, s.find_last_not_of(" \t\r\n\v\f") - first + 1);
}
// Compare names without case or outer whitespace.
std::string key(const std::string& s) { return HashTable::normalize(s); }
}
Facilities::Facilities(const CampusGraph& graph, std::size_t capacity) : graph_(graph), index_(capacity) {}
void Facilities::load(const std::string& path, std::ostream& out) {
    std::ifstream file(path);
    std::size_t accepted = 0, rejected = 0, lineNumber = 0;
    if (!file) out << path << ": unable to open file\n";
    std::string line;
    while (std::getline(file, line)) {
        ++lineNumber;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        std::vector<std::string> fields;
        std::size_t start = 0;
        while (true) {
            auto end = line.find('|', start);
            fields.push_back(trim(line.substr(start, end == std::string::npos ? end : end - start)));
            if (end == std::string::npos) break;
            start = end + 1;
        }
        std::string reason, building;
        if (fields.size() != 4) reason = "expected four fields";
        else {
            for (const auto& field : fields) if (field.empty()) reason = "empty required field";
            fields[0] = HashTable::normalize(fields[0]);
            if (reason.empty() && !HashTable::valid(fields[0])) reason = "ID must be F followed by exactly three digits";
            if (reason.empty() && !graph_.canonicalBuilding(fields[2], building)) reason = "unknown building: " + fields[2];
            if (reason.empty() && search(fields[0]).status == SearchStatus::Found) reason = "duplicate facility ID: " + fields[0];
        }
        if (!reason.empty()) { ++rejected; out << path << ':' << lineNumber << ": " << reason << '\n'; continue; }
        std::string category = fields[3];
        for (const auto& record : records_) if (key(record.category) == key(category)) { category = record.category; break; }
        index_.insert(fields[0], records_.size());
        records_.push_back({fields[0], fields[1], building, category});
        ++accepted;
    }
    out << path << ": " << accepted << " accepted, " << rejected << " rejected (hash capacity "
        << index_.capacity() << ", entries " << index_.entries() << ", collisions " << index_.collisions() << ")\n";
}
Facilities::SearchResult Facilities::search(const std::string& id) const {
    const auto normalized = HashTable::normalize(id);
    if (!HashTable::valid(normalized)) return {SearchStatus::Invalid, normalized, nullptr};
    std::size_t record;
    if (!index_.find(normalized, record)) return {SearchStatus::NotFound, normalized, nullptr};
    return {SearchStatus::Found, normalized, &records_[record]};
}
void Facilities::display(std::ostream& out) const {
    if (records_.empty()) out << "No facilities loaded.\n";
    for (const auto& f : records_) out << f.id << " | " << f.name << " | " << f.building << " | " << f.category << '\n';
    out << "Total facilities: " << records_.size() << '\n';
}
