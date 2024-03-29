#pragma once

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>


// This class represents a single production.
// A production is a replacement rule, it tells us which symbol
// to look for, and what (sequence of) symbol(s) to replace it with.
// The original symbol is usually called a “predecessor”.
// The (sequence of) symbol(s) is usually called a “successor”.
template <typename SymbolType>
class Production {
public:
    Production() = default;
    Production(const SymbolType& predecessor, const std::vector<SymbolType>& successor);

    bool operator==(const Production &other) const;
    SymbolType getPredecessor() const { return predecessor; }
    std::vector<SymbolType> getSuccessor() const { return successor; }

private:
    SymbolType predecessor;
    std::vector<SymbolType> successor;
};

// Defining std::hash implementation for type Production<SymbolType>
template <typename SymbolType>
struct std::hash<Production<SymbolType>> {
    size_t operator()(const Production<SymbolType>& production) const {
        return std::hash<SymbolType>{}(production.getPredecessor());
        }
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

    mutable std::vector<SymbolType> currentState;
};

// Implementation of template functions needs to be placed in header file instead of cpp file
// c++ Does tricky things with templates, either we define their implementation here or we instantiate
// the implementation for a specific type, we chose for the former as it is more scalable


template<typename SymbolType>
bool isValidProduction(const Production<SymbolType> &production, const std::unordered_set<SymbolType> &alphabet) {
    // Return if both predecessor and successor are both in alphabet set
    // Check successor_valid with all_of iterator containing lambda
    // Check predecessor separately
    // Evaluated by looping through alphabet, if iter==end then char is not found in alphabet

    const auto successor = production.getSuccessor();
    const bool successor_valid = std::all_of(successor.begin(), successor.end(), [&alphabet](const SymbolType v) {
        return (alphabet.find(v) != alphabet.end());
    });
    return (alphabet.find(production.getPredecessor()) != alphabet.end()) & successor_valid;
}


template<typename SymbolType>
Production<SymbolType>::Production(const SymbolType &predecessor, const std::vector<SymbolType> &successor) : predecessor(predecessor), successor(successor) { }


template<typename SymbolType>
bool Production<SymbolType>::operator==(const Production &other) const {
    return this->predecessor == other.predecessor;
}


template<typename SymbolType>
LSystemInterpreter<SymbolType>::LSystemInterpreter(const std::vector<SymbolType> &axiom,
                                                   const std::unordered_set<Production<SymbolType>> &productions,
                                                   const std::unordered_set<SymbolType> &alphabet):
                                                   axiom(axiom), alphabet(alphabet), currentState(axiom) {
    // Loop productions and check if valid
    for (const auto& production: productions) {
        if (!isValidProduction(production, alphabet)) {
            throw std::invalid_argument( "Any of productions is not valid" );
        }
    }
    // If error was not thrown Assign productions
    this->productions = productions;
}

template<typename SymbolType>
void LSystemInterpreter<SymbolType>::reset() {
    this->currentState = this->axiom;
}

template<typename SymbolType>
std::vector<SymbolType> apply_productions(SymbolType value, std::unordered_set<Production<SymbolType>> productions) {
    // First find the right production to apply
    auto iter = productions.begin();
    while (iter != productions.end()) {
        if (iter->getPredecessor() == value) {
            return iter->getSuccessor();
        }
        iter++;
    }
    return {value};
}

template<typename SymbolType>
std::vector<SymbolType> LSystemInterpreter<SymbolType>::operator()() const {
    std::vector<SymbolType> result;  // Define result vector with size minimum to current
    result.reserve(this->currentState.size());
    
    for (const auto& v : this->currentState) {
        const std::vector<SymbolType> production_result = apply_productions(v, this->productions);
        std::copy(production_result.begin(), production_result.end(), std::back_inserter(result));
    }

    this->currentState = result;
    return result;
}
