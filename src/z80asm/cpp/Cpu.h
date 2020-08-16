//-----------------------------------------------------------------------------
// z80asm restart - implement each of the supported CPUs
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>

class Cpu {
public:
    enum Type {						// TODO: remove same constants from legacy.h
        NOT_DEFINED = 0,
        Z80 = (1 << 0),
        Z80N = (1 << 1),
        Z180 = (1 << 2),
        R2K = (1 << 3),
        R3K = (1 << 4),
        I8080 = (1 << 5),
        I8085 = (1 << 6),
        GBZ80 = (1 << 7),
    };
    Cpu(Type type = Z80) { Init(type); }
    void Init(Type type = Z80);

    Type GetType() const { return type; }
    const std::string& GetName() const { return name; }
    auto GetDefinesBegin() const { return defines.cbegin(); }
    auto GetDefinesEnd() const { return defines.cend(); }

    // get opcode values, -1 if invalid
    int CALL() const { return 0xCD; }
    int RST(int n) const;

private:
    Type	type;
    std::string name;
    std::vector<std::string> defines;
};
