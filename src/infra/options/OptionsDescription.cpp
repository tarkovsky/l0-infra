#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>

using namespace std;

namespace options {

    OptionDescription::OptionDescription()
    {
    }
    
    OptionDescription::OptionDescription(const std::string& name,
                      const std::string& description)
    : description(description)
    {
        this->set_name(name);
    }

    OptionDescription::matchResult
    OptionDescription::match(const std::string& option, 
                              bool approx) const
    {
        auto result = no_match;        
        
		if (longName == option)
		{
                result = full_match;
		}
		else if (approx)
		{
			if (longName.find(option) == 0)
			{
				result = approximate_match;
			}
		}
         
        if (result != full_match)
        {
            if (shortName == option)
            {
                result = full_match;
            }
        }

        return result;        
    }

    const std::string& OptionDescription::getKey(const std::string& option) const
    {        
        if (!longName.empty()) 
            return longName;
        else
            return shortName;
    }

    const std::string& OptionDescription::getLongName() const
    {
        return longName;
    }

    OptionDescription& OptionDescription::set_name(const std::string& name)
    {
        auto n = name.find(',');
        if (n != string::npos)
        {
            assert(n == name.size()-2);
            longName = name.substr(0, n);
            shortName = '-' + name.substr(n+1,1);
        }
        else
        {
            longName = name;
        }
        return *this;
    }

    const std::string&
    OptionDescription::getDescription() const
    {
        return description;
    }

    std::string 
    OptionDescription::formatName() const
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

    OptionsDescription::OptionsDescription()
    {
    }

    OptionsDescription::OptionsDescription(const std::string& caption)
    : m_caption(caption)
    {
    }
    
    void OptionsDescription::add(std::map<std::string, std::string>&& args)
    {
        for (auto& entry : args)
        {
            std::shared_ptr<OptionDescription> d(new OptionDescription(entry.first, entry.second));
            m_options.push_back(d);
        }

    }

    const OptionDescription* OptionsDescription::find(const std::string& name,
													  bool approx) const
    {
        std::shared_ptr<OptionDescription> found;
        bool had_full_match = false;
        vector<string> approximate_matches;
        vector<string> full_matches;
        
        for(auto& one : m_options)
        {

			auto r = one->match(name, approx);
            if (r == OptionDescription::no_match) continue;

            if (r == OptionDescription::full_match)
            {                
                full_matches.push_back(one->getKey(name));
                found = one;
                had_full_match = true;
            } 
            else 
            {                        
                approximate_matches.push_back(one->getKey(name));
                if (!had_full_match)
                    found = one;
            }
        }

        if (full_matches.size() > 1) return 0;

        if (full_matches.empty() && approximate_matches.size() > 1) return 0;

        return found.get();
    }
}
