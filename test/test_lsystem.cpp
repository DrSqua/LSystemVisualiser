
#include <iostream> // For std::cout and std::endl
#include "catch2/catch.hpp"

#include <unordered_set>
#include <vector>
#include "lsystem/LSystemInterpreter.hpp"


TEST_CASE("fill_vector, Same Seed Yields Same Names") {
    const std::vector<char> axiom = {'f'};
    const std::unordered_set<Production<char>> productions{};
    const std::unordered_set<char> alphabet{'f', 'o'};

    LSystemInterpreter<char> lsystem = LSystemInterpreter(axiom, productions, alphabet);
}

