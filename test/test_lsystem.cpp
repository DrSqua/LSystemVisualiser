
#include <iostream> // For std::cout and std::endl
#include "catch2/catch.hpp"

#include <unordered_set>
#include <vector>
#include "lsystem/LSystemInterpreter.hpp"


TEST_CASE("Test Is valid production function") {
    using TestType = std::string;

    const auto invalid_production = Production<TestType>("n", {"n", "o"});
    const auto semi_invalid_production_1 = Production<TestType>("j", {"n", "o"});
    const auto semi_invalid_production_2 = Production<TestType>("n", {"j", "a"});
    const auto valid_production = Production<TestType>("j", {"a", "a"});
    const std::unordered_set<TestType> alphabet{"j", "a"};

    CHECK(! isValidProduction(invalid_production, alphabet));
    CHECK(! isValidProduction(semi_invalid_production_1, alphabet));
    CHECK(! isValidProduction(semi_invalid_production_2, alphabet));
    CHECK(  isValidProduction(valid_production, alphabet));
}

TEST_CASE("Constructor") {
    using TestType = std::string;

    const std::vector<TestType> axiom = {"f"};
    std::unordered_set<Production<TestType>> productions{
        Production<TestType>("f", {"o", "o"})
    };

    const std::unordered_set<TestType> alphabet{"f", "o"};

    LSystemInterpreter<TestType> lsystem = LSystemInterpreter(axiom, productions, alphabet);
}

TEST_CASE("Identity for character not in production") {
    // TODO
}

TEST_CASE("One Iteration") {
    using TestType = std::string;

    const std::vector<TestType> axiom = {"f"};
    std::unordered_set<Production<TestType>> productions{
            Production<TestType>("f", {"f", "o", "o"})
    };
    const std::unordered_set<TestType> alphabet{"f", "o"};

    LSystemInterpreter<TestType> lsystem = LSystemInterpreter(axiom, productions, alphabet);
    std::vector<std::vector<TestType>> results(1);

    std::generate(results.begin(), results.end(), lsystem);

    const std::vector<TestType> expected{"f", "o", "o"};
    CHECK(expected == results.at(0));
}

TEST_CASE("10 Iterations") {
    using TestType = std::string;

    const std::vector<TestType> axiom = {"f"};
    std::unordered_set<Production<TestType>> productions{
            Production<TestType>("f", {"f", "o", "o"})
    };
    const std::unordered_set<TestType> alphabet{"f", "o"};

    LSystemInterpreter<TestType> lsystem = LSystemInterpreter(axiom, productions, alphabet);
    std::vector<std::vector<TestType>> results(5);

    std::generate(results.begin(), results.end(), lsystem);

    const std::vector<TestType> expected{"f", "o", "o", "o", "o", "o", "o", "o", "o", "o", "o"};
    CHECK(expected == results.at(4));
}

TEST_CASE("Multiple Productions") {
    using TestType = std::string;

    const std::vector<TestType> axiom = {"a", "b", "a"};
    std::unordered_set<Production<TestType>> productions{
            Production<TestType>("a", {"a", "b"}),
            Production<TestType>("b", {"a"})
    };
    const std::unordered_set<TestType> alphabet{"a", "b"};

    LSystemInterpreter<TestType> lsystem = LSystemInterpreter(axiom, productions, alphabet);
    std::vector<std::vector<TestType>> results(1);

    std::generate(results.begin(), results.end(), lsystem);

    const std::vector<TestType> expected{"a", "b", "a", "a", "b",};
    CHECK(expected == results.at(0));
}