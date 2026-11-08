// Purpose: custom facility-ID index using separate chaining.
// Author: David Muagba (AI-assisted).
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <cstddef>
#include <string>
#include <vector>
class HashTable {
public:
    static constexpr std::size_t DefaultCapacity = 17;
    // Capacity must be positive; chains grow without a fixed entry limit.
    explicit HashTable(std::size_t capacity = DefaultCapacity);
    // Normalize before hashing. Duplicate insertions leave all counters unchanged.
    bool insert(const std::string& id, std::size_t recordIndex);
    // Return the associated record index through result if found.
    bool find(const std::string& id, std::size_t& result) const;
    // Trim, uppercase, and validate facility IDs.
    static std::string normalize(const std::string& id);
    static bool valid(const std::string& normalized);
    std::size_t capacity() const { return buckets_.size(); }
    std::size_t entries() const { return entries_; }
    std::size_t collisions() const { return collisions_; }
private:
    struct Entry { std::string id; std::size_t recordIndex; };
    std::vector<std::vector<Entry>> buckets_;
    std::size_t entries_ = 0, collisions_ = 0;
    // Polynomial hash, reducing modulo capacity at each character.
    std::size_t hash(const std::string& normalized) const;
};
#endif
