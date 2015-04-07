#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/Parsers.hpp"

#include <cassert>

using namespace std;

namespace options {

   namespace {

       template< class charT >
       std::basic_string< charT >  tolower_(const std::basic_string< charT >& str)
       {
           std::basic_string< charT > result;
           for (typename std::basic_string< charT >::size_type i = 0; i < str.size(); ++i)
           {
               result.append(1, static_cast< charT >(std::tolower(str[i])));
           }   
           return result;
       }

    }

    OptionDescription::OptionDescription()
    {
    }
    
    OptionDescription::
    OptionDescription(const std::string& name,
                       const Value_semantic* s)
    : valueSemantic(s)
    {
        this->set_name(name);
    }
                                           

    OptionDescription::
    OptionDescription(const std::string& name,
                      const Value_semantic* s,
                      const std::string& description)
    : description(description), valueSemantic(s)
    {
        this->set_name(name);
    }

    OptionDescription::matchResult
    OptionDescription::match(const std::string& option, 
                              bool approx, 
                              bool long_ignore_case,
                              bool short_ignore_case) const
    {
        matchResult result = no_match;        
        
        std::string local_long_name((long_ignore_case ? tolower_(longName) : longName));
        
        if (!local_long_name.empty()) {
        
            std::string local_option = (long_ignore_case ? tolower_(option) : option);

            if (*local_long_name.rbegin() == '*')
            {
                // The name ends with '*'. Any specified name with the given
                // prefix is OK.
                if (local_option.find(local_long_name.substr(0, local_long_name.length()-1))
                    == 0)
                    result = approximate_match;
            }

            if (local_long_name == local_option)
            {
                result = full_match;
            }
            else if (approx)
            {
                if (local_long_name.find(local_option) == 0)
                {
                    result = approximate_match;
                }
            }
        }
         
        if (result != full_match)
        {
            std::string local_option(short_ignore_case ? tolower_(option) : option);
            std::string local_short_name(short_ignore_case ? tolower_(shortName) : shortName);
            if (local_short_name == local_option)
            {
                result = full_match;
            }
        }

        return result;        
    }

    const std::string& 
    OptionDescription::getKey(const std::string& option) const
    {        
        if (!longName.empty()) 
            if (longName.find('*') != string::npos)
                // The '*' character means we're long_name
                // matches only part of the input. So, returning
                // long name will remove some of the information,
                // and we have to return the option as specified
                // in the source.
                return option;
            else
                return longName;
        else
            return shortName;
    }

    const std::string&
    OptionDescription::getLongName() const
    {
        return longName;
    }

    OptionDescription&
    OptionDescription::set_name(const std::string& name)
    {
        string::size_type n = name.find(',');
        if (n != string::npos) {
            assert(n == name.size()-2);
            longName = name.substr(0, n);
            shortName = '-' + name.substr(n+1,1);
        } else {
            longName = name;
        }
        return *this;
    }

    const std::string&
    OptionDescription::getDescription() const
    {
        return description;
    }

    std::shared_ptr<const Value_semantic>
    OptionDescription::semantic() const
    {
        return valueSemantic;
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

    std::string 
    OptionDescription::formatParameter() const
    {
        if (valueSemantic->max_tokens() != 0)
            return valueSemantic->name();
        else
            return "";
    }

    DescriptionInit::
    DescriptionInit(OptionsDescription* owner)
    : owner(owner)
    {}

    DescriptionInit&
    DescriptionInit::
    operator()(const std::string& name,
               const std::string& description)
    {
        std::shared_ptr<OptionDescription> d(
            new OptionDescription(name, new Untyped_value(true), description));

        owner->add(d);
        return *this;
    }

    DescriptionInit&
    DescriptionInit::
    operator()(const std::string& name,
               const Value_semantic* s)
    {
        std::shared_ptr<OptionDescription> d(new OptionDescription(name, s));
        owner->add(d);
        return *this;
    }

    DescriptionInit&
    DescriptionInit::
    operator()(const std::string& name,
               const Value_semantic* s,
               const std::string& description)
    {
        std::shared_ptr<OptionDescription> d(new OptionDescription(name, s, description));

        owner->add(d);
        return *this;
    }

    OptionsDescription::OptionsDescription()
    {
    }

    OptionsDescription::OptionsDescription(const std::string& caption)
    : m_caption(caption)
    {

    }
    
    void OptionsDescription::add(std::shared_ptr<OptionDescription> desc)
    {
        m_options.push_back(desc);
    }

    DescriptionInit OptionsDescription::add_options()
    {       
        return DescriptionInit(this);
    }

    const OptionDescription* OptionsDescription::find(const std::string& name,
                                      bool approx,
                                      bool long_ignore_case,
                                      bool short_ignore_case) const
    {
        std::shared_ptr<OptionDescription> found;
        bool had_full_match = false;
        vector<string> approximate_matches;
        vector<string> full_matches;
        
        for(auto one : m_options)
        {
            OptionDescription::matchResult r = 
                one->match(name, approx, long_ignore_case, short_ignore_case);

            if (r == OptionDescription::no_match)
                continue;

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
    
    std::ostream& operator<<(std::ostream& os, const OptionsDescription& desc)
    {
        desc.print(os);
        return os;
    }

    namespace {

        void format_paragraph(std::ostream& os,
                              std::string par,
                              unsigned indent,
                              unsigned line_length)
        {                    
            assert(indent < line_length);
            line_length -= indent;

            string::size_type par_indent = par.find('\t');

            if (par_indent == string::npos)
            {
                par_indent = 0;
            }
            else
            {
                par.erase(par_indent, 1);

                assert(par_indent < line_length);

                if (par_indent >= line_length)
                {
                    par_indent = 0;
                }            
            }
          
            if (par.size() < line_length)
            {
                os << par;
            }
            else
            {
                string::const_iterator       line_begin = par.begin();
                const string::const_iterator par_end = par.end();

                bool first_line = true; // of current paragraph!        
            
                while (line_begin < par_end)  // paragraph lines
                {
                    if (!first_line)
                    {
                        if ((*line_begin == ' ') &&
                            ((line_begin + 1 < par_end) &&
                             (*(line_begin + 1) != ' ')))
                        {
                            line_begin += 1;  // line_begin != line_end
                        }
                    }

                    unsigned remaining = static_cast<unsigned>(std::distance(line_begin, par_end));
                    string::const_iterator line_end = line_begin + 
                        ((remaining < line_length) ? remaining : line_length);
            
                    if ((*(line_end - 1) != ' ') &&
                        ((line_end < par_end) && (*line_end != ' ')))
                    {
                        string::const_iterator last_space =
                            find(reverse_iterator<string::const_iterator>(line_end),
                                 reverse_iterator<string::const_iterator>(line_begin),
                                 ' ')
                            .base();
                
                        if (last_space != line_begin)
                        {                 
                            if (static_cast<unsigned>(std::distance(last_space, line_end)) < 
                                (line_length / 2))
                            {
                                line_end = last_space;
                            }
                        }                                                
                    } // prevent chopped words
             
                    copy(line_begin, line_end, ostream_iterator<char>(os));
              
                    if (first_line)
                    {
                        indent += static_cast<unsigned>(par_indent);
                        line_length -= static_cast<unsigned>(par_indent); // there's less to work with now
                        first_line = false;
                    }

                    if (line_end != par_end)
                    {
                        os << '\n';
                
                        for(unsigned pad = indent; pad > 0; --pad)
                        {
                            os.put(' ');
                        }                                                        
                    }
              
                    line_begin = line_end;              
                } // paragraph lines
            }          
        }                              
        
    
        void format_one(std::ostream& os, const OptionDescription& opt, 
                        unsigned first_column_width)
        {
            stringstream ss;
            ss << "  " << opt.formatName() << ' ' << opt.formatParameter();
            
            os << ss.str();

            if (!opt.getDescription().empty())
            {
                if (ss.str().size() >= first_column_width)
                {
                   os.put('\n'); // first column is too long, lets put description in new line
                   for (unsigned pad = first_column_width; pad > 0; --pad)
                   {
                      os.put(' ');
                   }
                } else {
                   for(unsigned pad = first_column_width - static_cast<unsigned>(ss.str().size()); pad > 0; --pad)
                   {
                      os.put(' ');
                   }
                }
            }
        }
    }

    void OptionsDescription::print(std::ostream& os, unsigned width) const
    {
        if (!m_caption.empty())
            os << m_caption << ":\n";

        if (!width)
            width = default_line_length;

        /* The options formatting style is stolen from Subversion. */
        for (unsigned i = 0; i < m_options.size(); ++i)
        {
            const OptionDescription& opt = *m_options[i];

            format_one(os, opt, width);

            os << "\n";
        }
    }

}
