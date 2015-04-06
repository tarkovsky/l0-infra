#ifndef VARIABLESMAP_H
#define VARIABLESMAP_H

#include <string>
#include <map>
#include <set>
#include "Any.hpp"
#include <memory>

namespace options {

    struct ParsedOptions;
    struct Value_semantic;
    struct VariablesMap;

    void store(const ParsedOptions& options, VariablesMap& m);

    struct  VariableValue
    {
        VariableValue() : defaulted(false) {}
        VariableValue(const Any& xv, bool xdefaulted) 
        : v(xv), defaulted(xdefaulted) 
        {}

        bool empty() const;
        bool isDefaulted() const;
        const Any& value() const;

        Any& value();
   
        Any v;
        bool defaulted;

        std::shared_ptr<const Value_semantic> m_value_semantic;

        friend 
        void store(const ParsedOptions& options, VariablesMap& m);

        friend class  VariablesMap;
    };

    struct  AbstractVariablesMap {

        AbstractVariablesMap();
        AbstractVariablesMap(const AbstractVariablesMap* next);

        virtual ~AbstractVariablesMap() {}

        const VariableValue& operator[](const std::string& name) const;

        void next(AbstractVariablesMap* next);

    private:
        virtual const VariableValue& get(const std::string& name) const = 0;

        const AbstractVariablesMap* m_next;
    };

    struct  VariablesMap : private AbstractVariablesMap,
                               public std::map<std::string, VariableValue>
    {
        VariablesMap();
        VariablesMap(const AbstractVariablesMap* next);

        const VariableValue& operator[](const std::string& name) const
        { return AbstractVariablesMap::operator[](name); }

        void clear(); 
        
        bool has(const std::string& name) const;

        const VariableValue& get(const std::string& name) const;

        std::set<std::string> m_final;

        friend 
        void store(const ParsedOptions& options, 
                          VariablesMap& xm);
        
    };

    inline bool VariableValue::empty() const
    {
        return v.empty();
    }

    inline bool VariableValue::isDefaulted() const
    {
        return defaulted;
    }

    inline const Any& VariableValue::value() const
    {
        return v;
    }

    inline Any& VariableValue::value()
    {
        return v;
    }

}

#endif
