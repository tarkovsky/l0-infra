#ifndef VARIABLESMAP_Fri_Apr_10_22_02_27_2015_H
#define VARIABLESMAP_Fri_Apr_10_22_02_27_2015_H

#include <string>
#include <map>
#include <set>
#include <memory>

namespace options {

    struct ParsedOptions;
    struct VariablesMap;

    void store(const ParsedOptions& options, VariablesMap& m);

    struct  VariableValue
    {
		VariableValue();
		
		bool isEmpty() const ;
        const std::string& getValue() const;

		void setValue(const std::string& value);
		
    private:
        std::string value;
		bool empty;
    };

    struct  VariablesMap :  std::map<std::string, VariableValue>
    {
        VariablesMap();

        const VariableValue& operator[](const std::string& name) const
        { return get(name); }

        void clear(); 
        bool has(const std::string& name) const;
		
	private:
		friend void store(const ParsedOptions& options, VariablesMap& xm);
        const VariableValue& get(const std::string& name) const;
    };
}

#endif
