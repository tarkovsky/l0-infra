#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/ValueSemantic.hpp"
#include "l0-infra/options/program_options/VariablesMap.hpp"

#include <cassert>
#include <iostream>

namespace  options {

    using namespace std;

	void setMapValue(const Option& option, std::map<std::string, VariableValue>& m)
	{
		m[option.string_key].setValue("");
		
		if(option.value.empty()) return ;
		m[option.string_key].setValue(option.value[0]);
	}


    void store(const ParsedOptions& options, VariablesMap& map)
    {       
        assert(options.description);

        for (auto var : options.options)
        {
            if (var.unregistered) continue;

            const string& option_name = var.string_key;
            if (option_name.empty()) continue;
			
			if(!options.description->
			   find(option_name, var.hasValue)) continue;

			setMapValue(var, map);
        }
    }
     
    VariablesMap::VariablesMap()
    {}

    void VariablesMap::clear()
    {
        std::map<std::string, VariableValue>::clear();
    }

    const VariableValue& VariablesMap::get(const std::string& name) const
    {
        static VariableValue empty;
        const_iterator i = this->find(name);
        if (i == this->end())
            return empty;
        else
            return i->second;
    }
    
    bool VariablesMap::has(const std::string& name) const
    {
    	return std::map<std::string, VariableValue>::count(name) >= 1;
    }

	VariableValue::VariableValue() : empty(false)
	{
	}

	bool VariableValue::isEmpty() const
	{
		return empty;
	}

	void VariableValue::setValue(const std::string& str)
	{
		value = str;
		empty = true;
	}

	const std::string& VariableValue::getValue() const
	{
		return value;
	}
   

}
