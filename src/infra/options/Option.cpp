#include "l0-infra/options/program_options/Option.hpp"
#include <iostream>

namespace options {
	
Option::Option() : unregistered(false), hasValue(false)
{
}

Option::Option(const std::string& xstring_key,
		const std::vector<std::string> &xvalue) :
		string_key(xstring_key), value(xvalue), unregistered(false), hasValue(
				false)
{
}

void Option::dump() const
{
	std::cout << "option dump start string key = " << string_key << std::endl;
	std::cout << "option value dump start" << std::endl;
	for (auto var : value)
	{
		std::cout << var << std::endl;
	}
	std::cout << "option tokens dump start" << std::endl;
	for (auto var : original_tokens)
	{
		std::cout << var << std::endl;
	}
}
	
}
