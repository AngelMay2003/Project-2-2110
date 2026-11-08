# Graph contribution and integration

Author: David Muagba (AI-assisted implementation).

Scope: CampusGraph module, graph/building loaders, BFS, standalone demo and tests.
Original data files were not modified. Facilities and hashing are now separate modules; department tree and final menu remain unfinished. Do not treat this as a complete milestone submission.

## Design
CampusGraph uses vector<string> names and vector<vector<size_t>> adjacency lists.
Both grow dynamically. Adjacency lists use space proportional to vertices plus
edges and retain file insertion order without sorting. Each edge updates both
endpoints and increments the edge total once. Name comparisons trim surrounding
whitespace and ignore case, while display preserves the first accepted spelling.

BFS uses std::queue, a visited vector marked before enqueue, and a distance vector.
Its returned visits include the start at distance zero. Unknown starts return false
and clear the result. Isolated starts return one visit. BFS runs in O(V + E) after
the start-name lookup. Name lookup is linear; it is unrelated to facility ID hashing.

## Loading
Call loadBuildings before loadConnections on a fresh graph. The methods append to
the same object; they do not reset it. Their summaries count accepted and rejected
non-comment records per call; blank lines and full-line comments are skipped.
Warnings include path, physical line number and reason. Missing files return
opened=false without throwing an input exception. Surrounding whitespace and CRLF
are handled. Inline comments are not defined or removed.

Buildings reject blank names, extra pipe-separated fields and case-insensitive
duplicates. Connections require exactly two nonempty pipe-separated endpoints,
known buildings, no self-edge and no duplicate/reversed duplicate. These are
rubric-derived assumptions; check the full specification for additional rules.

## Team integration
Include CampusGraph.h and keep one CampusGraph instance alive in the final app.
Load the graph files at their required positions in the specification's four-file
startup order. Required order: buildings, connections, facilities, departments.
Use display(out) for adjacency output. Use breadthFirst(start, visits) for traversal;
show an error when it returns false, otherwise print visits and the count out of
buildingCount(). Use getline in the final menu so names with spaces work.
Do not include tests/graph_demo.cpp or tests/graph_tests.cpp in the final app build.

The final menu, exact prompts and numbering are left to the team. Full project
CONTRIBUTIONS.txt is also left to the team because other members' roles and work
have not been verified. This document records only this contribution; no commit
or live demonstration by David is claimed.
