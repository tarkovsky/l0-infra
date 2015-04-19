#ifndef OPTIONSDESCRIPTION_Fri_Apr_10_23_35_29_2015_H
#define OPTIONSDESCRIPTION_Fri_Apr_10_23_35_29_2015_H

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace options {

    struct OptionDescription
	{
        OptionDescription(const std::string& name,
                           const std::string& description);

        bool match(const std::string& option) const;
        
        const std::string& getKey(const std::string& option) const;
        const std::string& getLongName() const;
        const std::string& getDescription() const;

        std::string formatName() const;
        std::string formatParameter() const;

    private:
    
        OptionDescription& setName(const std::string& name);

        std::string shortName, longName, description;
    };

    struct OptionsDescription
    {
        OptionsDescription(const std::string& caption);

        void add(std::map<std::string, std::string>&& args);

        const OptionDescription* find(const std::string& name) const;

        friend std::ostream& operator<<(std::ostream& os, 
                                             const OptionsDescription& desc);

        void print(std::ostream& os, unsigned width = 0) const;

    private:
        enum { default_line_length = 80};
        using DescPtr =  std::shared_ptr<OptionDescription>;

        std::string m_caption;

        std::vector<DescPtr > m_options;
    };
}

#endif
