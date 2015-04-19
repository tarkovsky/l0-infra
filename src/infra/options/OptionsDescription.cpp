#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

namespace options {
    
    OptionDescription::OptionDescription
        ( const std::string& name
        , const std::string& description)
        : description(description)
    {
        setName(name);
    }

    bool OptionDescription::match(const std::string& option) const
    {
    	if (longName == option)
    	{
    		return true;
    	}

    	if (shortName == option)
    	{
    		return true;
    	}
    	return false;
    }

    const std::string& OptionDescription::getKey(const std::string& option) const
    {        
        return longName.empty() ? shortName : longName;
    }

    const std::string& OptionDescription::getLongName() const
    {
        return longName;
    }

    OptionDescription& OptionDescription::setName(const std::string& name)
    {
        auto n = name.find(',');
        if (n != string::npos)
        {
            longName = name.substr(0, n);
            shortName = '-' + name.substr(n+1,1);
        }
        else
        {
            longName = name;
        }
        return *this;
    }

    const std::string& OptionDescription::getDescription() const
    {
        return description;
    }

    std::string OptionDescription::formatName() const
    {
        if (!shortName.empty())
        {
            return longName.empty() 
                ? shortName 
                : string(shortName).append(" [ --").
                  append(longName).append(" ]");
        }
        return string("--").append(longName);
    }

    OptionsDescription::OptionsDescription(const std::string& caption)
    : m_caption(caption)
    {
    }
    
    void OptionsDescription::add(std::map<std::string, std::string>&& args)
    {
        for (auto& entry : args)
        {
            DescPtr d(new OptionDescription(entry.first, entry.second));
            m_options.push_back(d);
        }
    }

    const OptionDescription* OptionsDescription::find(const std::string& name) const
    {
        for(auto& one : m_options)
        {
			if(one->match(name)) return one.get();
        }
        return 0;
    }
}
