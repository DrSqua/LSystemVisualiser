//
// Created by robbe on 14/12/2023.
//

#pragma once

#include <vector>
#include <unordered_set>


// This class represents a single production.
// A production is a replacement rule, it tells us which symbol
// to look for, and what (sequence of) symbol(s) to replace it with.
// The original symbol is usually called a “predecessor”.
// The (sequence of) symbol(s) is usually called a “successor”.
template <typename SymbolType>
class Production {
public:
    Production(const SymbolType& predecessor, const SymbolType& successor);

    bool operator==(const Production& other) const;
private:

    SymbolType predecessor;
    SymbolType successor;

};




// This function verifies that all symbols in the production
// (Predecessor and all symbols in the successor) are
// in the given alphabet.
// If all symbols are in the alphabet, this function returns true,
// otherwise, this function returns false.
// The time-complexity of this function should be be O(N)
// where N is the total number of symbols in the production
// (predecessor + successors)
template <typename SymbolType>
bool isValidProduction(
        const Production<SymbolType>& production,
        const std::unordered_set<SymbolType>& alphabet
);


// This class represents the actual L-System.
// It contains an axiom (Initial state), A set of productions
// (Replacement rules) and an alphabet (List of allowed symbols).
template <typename SymbolType>
class LSystemInterpreter {
public:

    // This constructor takes an axiom, a set of productions and
    // an alphabet and stores these in the fields.
    // It should check that all productions are valid using
    // the “isValidProduction” function above.
    // If any of the productions are invalid,
    // it should throw an exception.
    // The constructor should also check that
    // every production has a unique predecessor.
    // It should also check that there is a Production for
    // each symbol in the alphabet.
    // If there is a symbol in the alphabet without a production,
    // you can decide what to do:
    // 1. Throw an exception
    // 2. Add an identity production (A -> A)
    LSystemInterpreter(
        const std::vector<SymbolType>& axiom,
        const std::unordered_set<Production<SymbolType>>& productions,
        const std::unordered_set<SymbolType>& alphabet
    );

    // After `operator()` has been called one or more times,
    // the L-system will have accumulated an internal state.
    // This method should reset this internal state,
    // so the next call to `operator()` starts from
    // the L-system’s axiom again.
    void reset();

    // This function should execute a single iteration of the L-System.
    // When this method is called twice,
    // the second call should use the result of the first call
    // as its starting point.
    // This allows us to use `std::generate()` to iteratively
    // execute the L-System.
    //
    // Be careful when applying your productions!
    // Your productions should always be applied at the same time!
    // An example:
    // We have 2 productions: A -> AB, B -> A, and an axiom “ABA”
    // We can apply the first production first, and get:
    // “ABBAB”
    // And then we can apply the second production, to get:
    // “AAAAA”
    // THIS IS WRONG!
    //
    // You should always execute all productions on the original text!
    // So, in this case, the correct output would be:
    // “ABAAB”
    // (The first and last A’s were replaced by AB,
    // and the middle B by A)
    std::vector<SymbolType> operator() () const;

private:
    std::vector<SymbolType> axiom;
    std::unordered_set<Production<SymbolType>> productions;
    std::unordered_set<SymbolType> alphabet;

    std::vector<SymbolType> current_state;
};

// Implementation of template functions needs to be placed in header file instead of cpp file
// c++ Does tricky things with templates, either we define their implementation here or we instantiate
// the implementation for a specific type, we chose for the former as it is more scalable

template<typename SymbolType>
LSystemInterpreter<SymbolType>::LSystemInterpreter(const std::vector<SymbolType> &axiom,
                                                   const std::unordered_set<Production<SymbolType>> &productions,
                                                   const std::unordered_set<SymbolType> &alphabet):
                                                   axiom(axiom), productions(productions), alphabet(alphabet), current_state(axiom) {

}

template<typename SymbolType>
void LSystemInterpreter<SymbolType>::reset() {
    this->current_state = this->axiom;
}

template<typename SymbolType>
std::vector<SymbolType> LSystemInterpreter<SymbolType>::operator()() const {
    return std::vector<SymbolType>();
}


template<typename SymbolType>
bool isValidProduction(const Production<SymbolType> &production, const std::unordered_set<SymbolType> &alphabet) {
    // Return if both predecessor and successor are both in alphabet set
    return (alphabet.find(production.predecessor) != alphabet.end()) & (alphabet.find(production.successor) != alphabet.end());
}
