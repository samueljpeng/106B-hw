#ifndef BipartiteGraph_Included
#define BipartiteGraph_Included

#include "hashset.h"
#include <ostream>

/* Type: UnorderedPair
 *
 * A type representing an unordered pair of elements. Two unordered pairs will compare equal
 * to one another if they have the same components in either order, so { "Hi", "Bye" } and
 * { "Bye", "Hi" } will compare equal.
 */
struct UnorderedPair {
    std::string first;
    std::string second;
};

/* Equality and hashing operations for UnorderedPairs, so we can toss them into a HashSet. */
bool operator== (const UnorderedPair& lhs, const UnorderedPair& rhs);
bool operator!= (const UnorderedPair& lhs, const UnorderedPair& rhs);
int hashCode(const UnorderedPair& pair);

/* Stream insertion for UnorderedPairs, for diagnostic purposes. */
std::ostream& operator<< (std::ostream& lhs, const UnorderedPair& pair);

/* Type: BipartiteGraph
 *
 * A struct representing a bipartite graph. The graph is encoded as two sets corresponding
 * to the left and right sets of nodes, along with a set of edges.
 */
struct BipartiteGraph {
    HashSet<std::string>   lhs;
    HashSet<std::string>   rhs;
    HashSet<UnorderedPair> edges;
};

/* Utility operator to print a BipartiteGraph, for debugging purposes. */
std::ostream& operator<< (std::ostream& lhs, const BipartiteGraph& graph);

/* Type: Matching
 *
 * A type representing a matching in a graph.
 */
using Matching = HashSet<UnorderedPair>;

#endif
