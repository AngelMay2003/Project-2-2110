# Graph notes

Author: David Muagba (AI-assisted).

The graph uses growing adjacency lists. Buildings and neighbors stay in input
order. Each connection is stored at both endpoints and counted once. Names
ignore case and outer whitespace; displayed names keep the first spelling.

BFS uses a queue and marks buildings visited when enqueued. It returns visit
order and distances, including the start at distance 0. An unknown start returns
false. An isolated start returns only itself. BFS takes O(V + E) after name lookup.

Load buildings before connections. Loaders append records, skip blank/comment
lines, trim whitespace and handle CRLF. Bad records print filename, line number
and reason. Each file gets an accepted/rejected summary, including missing files.
Buildings reject duplicates and bad field counts. Connections reject missing or
unknown endpoints, self-connections and duplicates in either direction.

Keep one graph for the whole program. Facilities uses it for building names.
Startup order: buildings, connections, facilities, departments. The final menu
must show errors and return to the menu. Test programs have separate main functions;
do not link them together.

Still needed: department tree, final menu, integration, complete project documents
and CELL testing. No CELL run is claimed here.
