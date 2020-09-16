//-----------------------------------------------------------------------------
// z80asm restart - symbol table
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "App.h"

#include <string>
#include <unordered_map>

template<class T>
class SymbolTable {
public:
    void Set(const std::string& name, const T& value);			// error if duplicate
    void Overwrite(const std::string& name, const T& value);

    bool Exists(const std::string& name);
    const T& Get(const std::string& name) const;				// empty if non-existent

private:
    std::unordered_map<std::string, T> symbols;
};

template<class T>
inline void SymbolTable<T>::Set(const std::string& name, const T& value) {
    if (Exists(name))
        app.reporter.Error(Reporter::Msg::DUPLICATE_SYMBOL, name);
    else
        Overwrite(name, value);
}

template<class T>
inline void SymbolTable<T>::Overwrite(const std::string& name, const T& value) {
    symbols[name] = value;
}

template<class T>
inline bool SymbolTable<T>::Exists(const std::string& name) {
    auto it = symbols.find(name);
    if (it == symbols.end())
        return false;
    else
        return true;
}

template<class T>
inline const T& SymbolTable<T>::Get(const std::string& name) const {
    static T empty;

    auto it = symbols.find(name);
    if (it == symbols.end())
        return empty;
    else
        return *it;
}
