# Graph test evidence

Executed November 8, 2026 with g++ C++17 and -Wall -Wextra -Wpedantic -Werror.
Both demo and automated test executables compiled successfully, with no warnings.

| Input/check | Expected | Actual | Result |
|---|---|---|---|
| Repository buildings and connections | 7 buildings, 6 undirected edges; no rejects | 7, 6, zero rejects | PASS |
| BFS from Library | Library 0, Student Union 1, Business Building 1, Science Building 2, Gym 2, Engineering Building 3; 6/7 | Exact order/distances; 6/7 | PASS |
| Diamond graph A-B, A-C, B-D, C-D | A 0, B 1, C 1, D 2, each once | Exact match | PASS |
| Case-insensitive and padded start ` a ` | Find A | Found A | PASS |
| Isolated start | Only itself, distance 0 | One visit at 0 | PASS |
| Unknown start and empty graph | false, empty results | false, empty results | PASS |
| Building ` a ` after A; blank name | Reject both | Both rejected | PASS |
| Reversed edge B-A after A-B; self edge; unknown endpoint | Reject, leave count unchanged | Rejected; count unchanged | PASS |
| CRLF, padded comment and blank line fixture | Skip comments/blanks; trim records | 2 buildings accepted, 2 rejected | PASS |
| Malformed/duplicate connection fixture | 1 accepted, 5 rejected | 1 accepted, 5 rejected | PASS |
| Duplicate warning locations | buildings.txt:4 and connections.txt:3 with reasons | Exact locations/reasons present | PASS |
| Missing file | opened=false, report error | No crash; opened=false | PASS |
| Adjacency display | A neighbors B, C in order; isolated message | Exact expected substrings | PASS |

Automated test output:
`PASS: insertion order, undirected edges, validation, BFS, isolated/unknown starts, CRLF and missing file`

This is graph-only evidence. Hash collision/duplicate-ID tests, full M1 test IDs,
menu tests, all-four-file validation and a live demonstration remain unfinished.
