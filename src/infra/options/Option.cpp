#include "l0-infra/options/program_options/Option.hpp"
#include <iostream>

namespace options {

Option::Option(const std::string& key,const std::string& value)
	: key(key)
	, value(value)
	, unregistered(false)
	, hasValue(!value.empty())
{
}

void Option::dump() const
{
	std::cout << "option dump start string key = " << key << std::endl;
	std::cout << "option value dump start" << std::endl;
	std::cout << value << std::endl;
}
	
}
