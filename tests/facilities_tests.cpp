// Purpose: facility/hash regression tests, separate from the final application.
// Author: David Muagba (AI-assisted).
#include "Facilities.h"
#include <cassert>
#include <iostream>
#include <sstream>
int main() {
    HashTable hash;
    assert(hash.insert("f001", 0));
    assert(hash.insert("F014", 1)); // Both hash to bucket 11 with capacity 17.
    assert(hash.collisions() == 1);
    assert(!hash.insert(" f001 ", 99));
    assert(hash.collisions() == 1 && hash.entries() == 2);
    std::size_t result;
    assert(hash.find("f001", result) && result == 0);
    assert(!hash.find("F999", result));
    assert(!hash.insert("004", 3));
    CampusGraph graph;
    std::string reason;
    assert(graph.addBuilding("Library", reason));
    Facilities facilities(graph);
    std::ostringstream output;
    facilities.load("testdata/collision/facilities.txt", output);
    assert(facilities.index().entries() == 2 && facilities.index().collisions() == 1);
    auto found = facilities.search(" f001 ");
    assert(found.status == Facilities::SearchStatus::Found);
    assert(found.facility->name == "First" && found.facility->building == "Library");
    assert(facilities.search("F014").facility->category == "Study");
    assert(facilities.search("004").status == Facilities::SearchStatus::Invalid);
    assert(facilities.search("F999").status == Facilities::SearchStatus::NotFound);
    assert(output.str().find("2 accepted, 5 rejected") != std::string::npos);
    assert(output.str().find("facilities.txt:4: duplicate") != std::string::npos);
    facilities.load("testdata/collision/missing.txt", output);
    assert(facilities.index().entries() == 2);
    facilities.display(output);
    assert(output.str().find("Total facilities: 2") != std::string::npos);
    std::cout << output.str() << "PASS: facilities/hash checks\n";
}
