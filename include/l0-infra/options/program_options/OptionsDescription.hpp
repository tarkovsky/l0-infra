#ifndef OPTIONSDESCRIPTION_H
#define OPTIONSDESCRIPTION_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "ValueSemantic.hpp"

namespace options {

    struct OptionDescription
	{
        OptionDescription();

        OptionDescription(const std::string& name,
                           const Value_semantic* s);

        OptionDescription(const std::string& name,
                           const Value_semantic* s,
                           const std::string& description);

        enum matchResult { no_match, full_match, approximate_match };

        matchResult match(const std::string& option, bool approx,
                           bool long_ignore_case, bool short_ignore_case) const;
        
        const std::string& getKey(const std::string& option) const;
        
        const std::string& getLongName() const;

        const std::string& getDescription() const;

        std::shared_ptr<const Value_semantic> semantic() const;

        std::string formatName() const;

        std::string formatParameter() const;

    private:
    
        OptionDescription& set_name(const std::string& name);

        std::string shortName, longName, description;
        std::shared_ptr<const Value_semantic> valueSemantic;
    };

    struct  OptionsDescription;
    struct  DescriptionInit {

        DescriptionInit(OptionsDescription* owner);

        DescriptionInit&
        operator()(const std::string& name,
                   const std::string& description);

        DescriptionInit&
        operator()(const std::string& name,
                   const Value_semantic* s);
        
        DescriptionInit&
        operator()(const std::string& name,
                   const Value_semantic* s,
                   const std::string& description);
       
    private:
        OptionsDescription* owner;
    };


    struct OptionsDescription
    {
        OptionsDescription();
        OptionsDescription(const std::string& caption);

        DescriptionInit add_options();
        void add(std::shared_ptr<OptionDescription> desc);

        const OptionDescription* find(const std::string& name,
                                               bool approx,
                                               bool long_ignore_case = false,
                                               bool short_ignore_case = false) const;

        friend std::ostream& operator<<(std::ostream& os, 
                                             const OptionsDescription& desc);

        void print(std::ostream& os, unsigned width = 0) const;

    private:
        enum { default_line_length = 80};

        std::string m_caption;

        std::vector< std::shared_ptr<OptionDescription> > m_options;
    };
}

#endif
