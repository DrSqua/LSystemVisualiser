#include "../include/lsystem/LSystemInterpreter.hpp"

template<typename SymbolType>
Production<SymbolType>::Production(const SymbolType& predecessor, const SymbolType& successor) : predecessor(predecessor), successor(successor) {

}

template<typename SymbolType>
bool Production<SymbolType>::operator==(const Production &other) const {
    return false;
}
