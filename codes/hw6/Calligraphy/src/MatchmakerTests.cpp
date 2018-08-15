/*
 * CS106B Assignment 6-A
 * MatchmakerTest.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/15/2018
 *
 * Tests for Matchmaker.cpp
 */
#include "MatchmakerTests.h"
#include "Matchmaker.h"
using namespace std;

ENABLE_TESTS();

ADD_TEST("If everyone is already uniquely paired off, all pairs should be found.") {
    BipartiteGraph graph;

    graph.lhs = { "A", "B", "C" };
    graph.rhs = { "1", "2", "3" };
    graph.edges = {
        { "A", "1" },
        { "B", "2" },
        { "C", "3" }
    };

    /* We should get back all possible pairs. */
    auto match = matchmake(graph);

    expect(match == graph.edges);
}

ADD_TEST("If all possible pairs exist, everyone should end up paired off.") {
    BipartiteGraph graph;

    graph.lhs = { "A", "B" };
    graph.rhs = { "1", "2" };
    graph.edges = {
        { "A", "1" },
        { "A", "2" },
        { "B", "1" },
        { "B", "2" }
    };

    auto match = matchmake(graph);
    expect(match.size() == 2);

    for (auto person: graph.lhs + graph.rhs) {
        bool found = false;
        for (auto edge: match) {
            if (edge.first == person || edge.second == person) {
                found = true;
                break;
            }
        }
        expect(found);
    }
}

ADD_TEST("If no right side element exists") {
    BipartiteGraph graph;

    graph.lhs = { "A", "B" };
    graph.rhs = {};
    graph.edges = {
    };

    auto match = matchmake(graph);
    expect(match.size() == 0);
}

ADD_TEST("If no left side element exists") {
    BipartiteGraph graph;

    graph.lhs = {};
    graph.rhs = { "1", "2" };
    graph.edges = {
    };

    auto match = matchmake(graph);
    expect(match.size() == 0);
}

ADD_TEST("Not everyone gets paied up") {
    BipartiteGraph graph;

    graph.lhs = { "A", "B" };
    graph.rhs = { "1", "2" };
    graph.edges = {
        {"A", "2"},
        {"B", "2"}
    };

    auto match = matchmake(graph);
    expect(match.size() == 1);
}
