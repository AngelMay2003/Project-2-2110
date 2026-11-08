// Purpose: normalized ID hashing, chained insertion and lookup.
// Author: David Muagba (AI-assisted).
#include "HashTable.h"
#include <stdexcept>
std::string HashTable::normalize(const std::string& id) {
    const auto first = id.find_first_not_of(" \t\r\n\v\f");
    if (first == std::string::npos) return "";
    auto value = id.substr(first, id.find_last_not_of(" \t\r\n\v\f") - first + 1);
    for (char& c : value) if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    return value;
}
bool HashTable::valid(const std::string& id) {
    if (id.size() != 4 || id[0] != 'F') return false;
    for (std::size_t i = 1; i < id.size(); ++i) if (id[i] < '0' || id[i] > '9') return false;
    return true;
}
HashTable::HashTable(std::size_t capacity) : buckets_(capacity) {
    if (!capacity) throw std::invalid_argument("Hash capacity must be positive");
}
std::size_t HashTable::hash(const std::string& id) const {
    std::size_t value = 0;
    for (unsigned char c : id) value = (value * 31 + c) % buckets_.size();
    return value;
}
bool HashTable::find(const std::string& id, std::size_t& result) const {
    const auto key = normalize(id);
    if (!valid(key)) return false;
    for (const auto& entry : buckets_[hash(key)])
        if (entry.id == key) { result = entry.recordIndex; return true; }
    return false;
}
bool HashTable::insert(const std::string& id, std::size_t recordIndex) {
    const auto key = normalize(id);
    if (!valid(key)) return false;
    std::size_t unused;
    if (find(key, unused)) return false;
    auto& bucket = buckets_[hash(key)];
    const bool collision = !bucket.empty();
    bucket.push_back({key, recordIndex});
    ++entries_;
    if (collision) ++collisions_;
    return true;
}
