#include "l0-infra/options/program_options/Parsers.hpp"
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/ValueSemantic.hpp"
#include "l0-infra/options/program_options/VariablesMap.hpp"

#include <cassert>
#include <iostream>

namespace  options {

    using namespace std;

    void store(const ParsedOptions& options, VariablesMap& map)
    {       
        assert(options.description);

        const OptionsDescription& desc = *options.description;

        std::map<std::string, VariableValue>& m = map;

        std::set<std::string> new_final;

        string option_name;
        string original_token;

        for (auto var : options.options)
        {
            option_name = var.string_key;
            original_token = var.original_tokens.size() ?
                var.original_tokens[0] :
                option_name;

            if (option_name.empty())
                continue;

            if (var.unregistered)
                continue;

            if (map.m_final.count(option_name))
                continue;

            string original_token = var.original_tokens.size() ?
                var.original_tokens[0]     : "";

            const OptionDescription* d = desc.find(option_name, var.hasValue,
                                                    false, false);

            if(!d) continue;
            VariableValue& v = m[option_name];            
            if (v.isDefaulted()) {
                v = VariableValue();
            }
                
            d->semantic()->parse(v.value(), var.value);

            v.m_value_semantic = d->semantic();
                
            if (!d->semantic()->is_composing())
                new_final.insert(option_name);
        }

        map.m_final.insert(new_final.begin(), new_final.end());
    }
     
    AbstractVariablesMap::AbstractVariablesMap()
    : m_next(0)
    {}

    AbstractVariablesMap::
    AbstractVariablesMap(const AbstractVariablesMap* next)
    : m_next(next)
    {}

    const VariableValue& 
    AbstractVariablesMap::operator[](const std::string& name) const
    {
        const VariableValue& v = get(name);
        if (v.empty() && m_next)
            return (*m_next)[name];
        else if (v.isDefaulted() && m_next) {
            const VariableValue& v2 = (*m_next)[name];
            if (!v2.empty() && !v2.isDefaulted())
                return v2;
            else return v;
        } else {
            return v;
        }
    }

    void 
    AbstractVariablesMap::next(AbstractVariablesMap* next)
    {
        m_next = next;
    }

    VariablesMap::VariablesMap()
    {}

    VariablesMap::VariablesMap(const AbstractVariablesMap* next)
    : AbstractVariablesMap(next)
    {}

    void VariablesMap::clear()
    {
        std::map<std::string, VariableValue>::clear();
        m_final.clear();
    }

    const VariableValue&
    VariablesMap::get(const std::string& name) const
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

}
