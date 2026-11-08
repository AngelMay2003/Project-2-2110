// Purpose: Facility ID hash table.
// Author: David Muagba (AI-assisted).
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <cstddef>
#include <string>
#include <vector>
class HashTable {
public:
    static constexpr std::size_t DefaultCapacity = 17;
    // Use a positive capacity. Chains can grow.
    explicit HashTable(std::size_t capacity = DefaultCapacity);
    // Add a normalized ID. Duplicates do not change counters.
    bool insert(const std::string& id, std::size_t recordIndex);
    // Find the record index for an ID.
    bool find(const std::string& id, std::size_t& result) const;
    // Normalize and validate IDs.
    static std::string normalize(const std::string& id);
    static bool valid(const std::string& normalized);
    std::size_t capacity() const { return buckets_.size(); }
    std::size_t entries() const { return entries_; }
    std::size_t collisions() const { return collisions_; }
private:
    struct Entry { std::string id; std::size_t recordIndex; };
    std::vector<std::vector<Entry>> buckets_;
    std::size_t entries_ = 0, collisions_ = 0;
    // Hash the ID using the table capacity.
    std::size_t hash(const std::string& normalized) const;
};
#endif
