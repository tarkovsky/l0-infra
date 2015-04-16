#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/VariablesMap.hpp"

#include <cassert>
#include <iostream>

namespace  options {

	void setMapValue(const Option& option, std::map<std::string, std::string>& m)
	{
		m[option.key] = "";

		if(option.value.empty()) return;
		m[option.key] = option.value;
	}


    void VariablesMap::store(const ParsedOptions& options)
    {       
        for (auto& var : options.options())
        {
            if (var.unregistered) continue;

            const std::string& option_name = var.key;
            if (option_name.empty()) continue;
			
			if(!options.description().find(option_name, var.hasValue))
			    continue;

			setMapValue(var, *this);
        }
    }

	VariablesMap& VariablesMap::getInstance()
	{
		static VariablesMap map;
		return map;
	}

    void VariablesMap::clear()
    {
        super::clear();
    }

    const std::string& VariablesMap::operator[](const std::string& name) const
    {
        static std::string empty;

        auto i = super::find(name);
        return i == super::end() ? empty : i->second;
    }
    
    bool VariablesMap::has(const std::string& name) const
    {
    	return super::count(name) != 0;
    }
}
