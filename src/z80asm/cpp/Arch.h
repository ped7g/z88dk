//-----------------------------------------------------------------------------
// z80asm restart - implement each of the supported computer architectures
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <utility>

class Arch {
public:
	enum Type {					// NOTE: keep archTable[] in sync
		GENERAL, TI83, TI83PLUS,
	};
	Arch(Type type = GENERAL) { Init(type); }
	void Init(Type type = GENERAL);

	Type GetType() const { return type; }
	const std::string& GetName() const { return name; }
	auto GetDefinesBegin() const { return defines.cbegin(); }
	auto GetDefinesEnd() const { return defines.cend(); }

	// get opcode values, -1 if invalid
	int INVOKE() const;

private:
	Type			type;
	std::string		name;
	std::vector<std::pair<std::string, int>> 
					defines;
};
