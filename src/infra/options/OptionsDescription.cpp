#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
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

    auto OptionDescription::match(const std::string& option, bool approx) const -> MatchResult
    {
        auto result = NO_MATCH;
        
		if (longName == option)
		{
                result = FULL_MATCH;
		}
		else if (approx)
		{
			if (longName.find(option) == 0)
			{
				result = APPROXIMATE_MATCH;
			}
		}
         
        if (result != FULL_MATCH)
        {
            if (shortName == option)
            {
                result = FULL_MATCH;
            }
        }

        return result;        
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
            if (r == OptionDescription::NO_MATCH) continue;

            if (r == OptionDescription::FULL_MATCH)
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
