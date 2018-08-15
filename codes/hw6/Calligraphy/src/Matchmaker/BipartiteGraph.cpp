#include "BipartiteGraph.h"
#include <algorithm>
#include <sstream>
using namespace std;

/* Comparison operator for unordered pairs. Two unordered pairs are equal if they
 * have the same components, which we check by comparing the smaller and larger
 * components separately.
 */
bool operator== (const UnorderedPair& lhs, const UnorderedPair& rhs) {
   return max(lhs.first, lhs.second) == max(rhs.first, rhs.second) &&
          min(lhs.first, lhs.second) == min(rhs.first, rhs.second);
}

/* Two unordered pairs are unequal if they aren't equal to one another. */
bool operator!= (const UnorderedPair& lhs, const UnorderedPair& rhs) {
    return !(lhs == rhs);
}

/* To hash an unordered pair, we hash the two strings in some consistent order.
 * Specifically, we'll hash the larger, then the smaller.
 */
int hashCode(const UnorderedPair& pair) {
    return hashCode(max(pair.first, pair.second) + "#" + min(pair.first, pair.second));
}

/* To print an unordered pair, we'll print the smallest component first, for simplicity. */
ostream& operator<< (ostream& out, const UnorderedPair& pair) {
    ostringstream builder;

    builder << "{ " << min(pair.first, pair.second) << ", " << max(pair.first, pair.second) << " }";

    return out << builder.str();
}

/* We'll display bipartite graphs by reporting their components independently. */
ostream& operator<< (ostream& out, const BipartiteGraph& graph) {
    ostringstream builder;
    builder << "{ Left: " << graph.lhs
            << ", Right: " << graph.rhs
            << ", Edges: " << graph.edges << " }";
    return out << builder.str();
}
