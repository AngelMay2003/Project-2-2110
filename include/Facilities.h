// Purpose: facility records, validated loading and hash-indexed ID search.
// Author: David Muagba (AI-assisted).
#ifndef FACILITIES_H
#define FACILITIES_H
#include "HashTable.h"
#include "CampusGraph.h"
#include <iosfwd>
struct Facility { std::string id, name, building, category; };
class Facilities {
public:
    enum class SearchStatus { Invalid, NotFound, Found };
    struct SearchResult { SearchStatus status; std::string normalizedId; const Facility* facility; };
    // Use the shared campus graph to resolve accepted building spellings.
    explicit Facilities(const CampusGraph& graph, std::size_t capacity = HashTable::DefaultCapacity);
    // Append accepted records in file order and print diagnostics and summary.
    void load(const std::string& path, std::ostream& out);
    // Search only through the custom table; pointer valid until next load/mutation.
    SearchResult search(const std::string& id) const;
    // Display all fields in accepted file order and a total.
    void display(std::ostream& out) const;
    const HashTable& index() const { return index_; }
private:
    const CampusGraph& graph_;
    HashTable index_;
    std::vector<Facility> records_;
};
#endif
