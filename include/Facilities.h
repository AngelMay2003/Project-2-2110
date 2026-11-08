// Purpose: Facility records and ID search.
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
    // Use building names from the shared graph.
    explicit Facilities(const CampusGraph& graph, std::size_t capacity = HashTable::DefaultCapacity);
    // Load facilities and print the summary.
    void load(const std::string& path, std::ostream& out);
    // Search by hash table. Reloading may invalidate the returned pointer.
    SearchResult search(const std::string& id) const;
    // Show facilities in file order and the total.
    void display(std::ostream& out) const;
    const HashTable& index() const { return index_; }
private:
    const CampusGraph& graph_;
    HashTable index_;
    std::vector<Facility> records_;
};
#endif
