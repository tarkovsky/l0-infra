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

        OptionDescription(const char* name,
                           const Value_semantic* s);

        OptionDescription(const char* name,
                           const Value_semantic* s,
                           const char* description);

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
    
        OptionDescription& set_name(const char* name);

        std::string shortName, longName, description;
        std::shared_ptr<const Value_semantic> valueSemantic;
    };

    struct  OptionsDescription;
    struct  DescriptionInit {

        DescriptionInit(OptionsDescription* owner);

        DescriptionInit&
        operator()(const char* name,
                   const char* description);

        DescriptionInit&
        operator()(const char* name,
                   const Value_semantic* s);
        
        DescriptionInit&
        operator()(const char* name,
                   const Value_semantic* s,
                   const char* description);
       
    private:
        OptionsDescription* owner;
    };


    struct OptionsDescription
    {
        OptionsDescription(unsigned line_length = default_line_length,
                            unsigned min_description_length = default_line_length / 2);

        OptionsDescription(const std::string& caption,
                            unsigned line_length = default_line_length,
                            unsigned min_description_length = default_line_length / 2);
        DescriptionInit add_options();
        void add(std::shared_ptr<OptionDescription> desc);
        OptionsDescription& add(const OptionsDescription& desc);

        unsigned get_option_column_width() const;


        const OptionDescription* find(const std::string& name,
                                       bool approx, 
                                       bool long_ignore_case = false,
                                       bool short_ignore_case = false) const;

        const OptionDescription* find_nothrow(const std::string& name,
                                               bool approx,
                                               bool long_ignore_case = false,
                                               bool short_ignore_case = false) const;


        const std::vector< std::shared_ptr<OptionDescription> >& options() const;

        friend std::ostream& operator<<(std::ostream& os, 
                                             const OptionsDescription& desc);

        void print(std::ostream& os, unsigned width = 0) const;

    private:
        typedef std::map<std::string, int>::const_iterator name2index_iterator;
        typedef std::pair<name2index_iterator, name2index_iterator> 
            approximation_range;

        enum { default_line_length = 80};

        std::string m_caption;
        const unsigned m_line_length;
        const unsigned m_min_description_length;

        std::vector< std::shared_ptr<OptionDescription> > m_options;

        std::vector<bool> belong_to_group;

        std::vector< std::shared_ptr<OptionsDescription> > groups;

    };
}

#endif
