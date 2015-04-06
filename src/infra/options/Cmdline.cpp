#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/ValueSemantic.hpp"
#include <cassert>

namespace options { namespace detail {

    Cmdline::Cmdline(const vector<string>& args)
    {
        init(args);
    }

    Cmdline::Cmdline(int argc, const char** argv)
    {
        init(vector<string>(argv+1, argv+argc+!argc));
    }

    void
    Cmdline::init(const vector<string>& args)
    {
        this->args = args;        
        desc= 0;
    }
    
    void Cmdline::setDescription(const OptionsDescription& desc)
    {
        this->desc= &desc;
    }

    typedef std::vector<Option> (options::detail::Cmdline::* style_parser)(const std::string&);

    vector<Option> Cmdline::run()
    {
    	assert(desc);

    	style_parser style_parsers[] = {&Cmdline::parseLongOption, &Cmdline::parseShortOption};

    	vector<Option> result;
    	while(!args.empty())
    	{
    		bool unkonwn = false;
    		for(auto parser : style_parsers)
    		{
    			unsigned current_size = static_cast<unsigned>(args.size());
    			vector<Option> next = (this->*parser)(args[0]);

    			if(!next.empty())
    			{
    				for(auto var : next)
    				{
    					//var.dump();
    					result.push_back(var);
    				}
    				args.erase(args.begin());
    			}
    			if (args.size() < current_size) {
    				unkonwn = true;
    				break;
    			}
    		}

    		if (!unkonwn) {
    			Option opt;
    			opt.value.push_back(args[0]);
    			opt.original_tokens.push_back(args[0]);
    			result.push_back(opt);
    			args.erase(args.begin());
    		}
    	}
    	//        for(auto var : result)
    	//        {
    	//        	var.dump();
    	//        }
    	vector<Option> result2;
    	for (unsigned i = 0; i < result.size(); ++i)
    	{
    		result2.push_back(result[i]);
    		Option& opt = result2.back();

    		if (opt.string_key.empty())
    		{
    			continue;
    		}

    		const OptionDescription* xd = desc->find_nothrow(opt.string_key,
    				true,
					true,
					true);

    		if (!xd)
    		{
    			opt.unregistered = true;
    			result2.pop_back();
    			continue;
    		}
    		unsigned min_tokens = xd->semantic()->min_tokens();
    		unsigned max_tokens = xd->semantic()->max_tokens();
    		if (min_tokens < max_tokens && opt.value.size() < max_tokens)
    		{
    			int can_take_more = max_tokens - static_cast<int>(opt.value.size());
    			unsigned j = i+1;
    			for (; can_take_more && j < result.size(); --can_take_more, ++j)
    			{
    				Option& opt2 = result[j];
    				if (!opt2.string_key.empty())
    					break;

    				assert(opt2.value.size() == 1);

    				opt.value.push_back(opt2.value[0]);

    				assert(opt2.original_tokens.size() == 1);

    				opt.original_tokens.push_back(opt2.original_tokens[0]);
    			}
    			i = j-1;
    		}
    	}
    	result.swap(result2);

		for(auto var: result)
		{
			//var.dump();
		}
		return result;
    }

    vector<Option> 
    Cmdline::parseLongOption(const string& tok)
    {
        vector<Option> result;

        if (tok.size() >= 3 && tok[0] == '-' && tok[1] == '-')
        {   
            string name, adjacent;
            string::size_type p = tok.find('=');
            if (p != tok.npos)
            {
                name = tok.substr(2, p-2);
                adjacent = tok.substr(p+1);
            }
            else
            {
                name = tok.substr(2);
            }
            Option opt;
            opt.string_key = name;
            if (!adjacent.empty())
            {
                opt.value.push_back(adjacent);
                opt.hasValue = true;
            }
            opt.original_tokens.push_back(tok);
            result.push_back(opt);
        }
        return result;
    }


    vector<Option> 
    Cmdline::parseShortOption(const string& tok)
    {
        if (tok.size() >= 2 && tok[0] == '-' && tok[1] != '-')
        {   
            vector<Option> result;

            string name = tok.substr(0,2);
            string adjacent = tok.substr(2);

            for(;;) {
                const OptionDescription* d = desc->find_nothrow(name, false, false, true);

                if(d && !adjacent.empty()){
                    // 'adjacent' is in fact further option.
                    Option opt;
                    opt.string_key = d->getLongName();
                    opt.original_tokens.push_back(tok);
                    if(adjacent[0] == '=')
                    {
                    	adjacent.erase(adjacent.begin());
                    	opt.value.push_back(adjacent);
                    	result.push_back(opt);
                    	return result;
                    }
                    result.push_back(opt);
                    
                    if (adjacent.empty())
                    {
                        break;
                    }

                    name = string("-") + adjacent[0];
                    adjacent.erase(adjacent.begin());
                }
                else
                {
                    
                    Option opt;
                    opt.string_key = d?d->getLongName():name;
                    opt.original_tokens.push_back(tok);
                    if (!adjacent.empty())
                        opt.value.push_back(adjacent);
                    result.push_back(opt);

                    break;
                }
            }
            return result;
        }
        return vector<Option>();
    }

}}
