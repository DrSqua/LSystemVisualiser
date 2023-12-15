#include <iostream>
#include <unordered_set>
#include "LSystemInterpreter.hpp"


namespace std {
    template <typename SymbolType> struct hash<Production<SymbolType>>
    {
        size_t operator()(const Production<SymbolType> & production) const
        {
            return std::hash(production.predecessor);
        }
    };
}

int main() {
    const std::vector<char> axiom = {'f'};
    const std::unordered_set<Production<char>> productions{};
    const std::unordered_set<char> alphabet{'f', 'o'};

    LSystemInterpreter<char> lsystem = LSystemInterpreter(axiom, productions, alphabet);
    return 0;
}
