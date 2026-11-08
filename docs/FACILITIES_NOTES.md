# Facilities and hash contribution (partial project)

AI-assisted implementation prepared for David Muagba. The assignment prohibits
AI-generated code; instructor permission is necessary before submission.

The custom index uses separate chaining with 17 buckets. For an uppercase key,
start h=0 and apply h=(31*h+character_code)%capacity for each character. One
collision means one accepted insertion into a nonempty bucket. Duplicate and
invalid IDs do not increase collision count. Chains grow dynamically; there is
no fixed entry limit or full-table rejection. Capacity must be positive.

Facilities retains accepted records in a vector in file order. The hash table
maps IDs to vector indices; ID search never scans facilities. Category spelling
is taken from the first accepted matching category; graph lookup supplies the
canonical building spelling. Load buildings before facilities and keep that
graph alive for the Facilities object's lifetime.

Tests executed with C++17, -Wall -Wextra -Wpedantic -Werror:
- F001 and F014 both hash to bucket 11: 2 entries, 1 collision (PASS).
- Lowercase/padded duplicate F001 retained the first entry and collision count (PASS).
- Invalid, found, lowercase and valid-not-found searches (PASS).
- File fixture: 2 accepted, 5 rejected, capacity 17, entries 2, collisions 1 (PASS).
- Rejections: duplicate ID, malformed ID, unknown building, blank field, extra field (PASS).
- Missing file reports error and zero accepted/rejected without crash (PASS).
- Canonical Library building and first accepted Study category retained (PASS).
- Existing graph regression tests rerun successfully (PASS).

These tests ran in the development environment, not CELL. Final menu, department
tree, four-file startup coordination and full milestone documentation remain
unfinished. Use README commands for separate test executables. The team must
verify integration and all required M1 tests on CELL.
