#include "l0-infra/options/program_options/VariablesMap.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/OptionsDescription.hpp"

namespace  options {

	void setMapValue(const Option& option, std::map<std::string, std::string>& m)
	{
		m[option.key()] = "";

		if(option.value().empty()) return;
		m[option.key()] = option.value();
	}

    void VariablesMap::store(const ParsedOptions& options)
    {       
        for (auto& var : options.options())
        {
            const std::string& option_name = var.key();
            if (option_name.empty()) continue;
			
			setMapValue(var, *this);
        }
    }

    void VariablesMap::parseArgs(int argc, const char** const argv
                    , const OptionsDescription& desc)
    {
    	store(CommandLineParser(argc, argv, desc).run());
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
