#ifndef VARIABLESMAP_Fri_Apr_10_22_02_27_2015_H
#define VARIABLESMAP_Fri_Apr_10_22_02_27_2015_H

#include <string>
#include <map>
#include <set>
#include <memory>

namespace options {

    struct ParsedOptions;

    struct  VariablesMap : std::map<std::string, std::string>
    {
    private:
        using super = std::map<std::string, std::string>;

    public:
		static VariablesMap&  getInstance();

		void store(const ParsedOptions& options);

        const std::string& operator[](const std::string& name) const;

        void clear(); 
        bool has(const std::string& name) const;
	};
}

#endif
