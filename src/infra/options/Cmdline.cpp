#include <l0-infra/options/program_options/detail/Cmdline.hpp>
#include "l0-infra/options/program_options/OptionsDescription.hpp"
#include "l0-infra/options/program_options/ValueSemantic.hpp"
#include "l0-infra/options/program_options/Option.hpp"
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

    void Cmdline::init(const vector<string>& args)
    {
        this->args = args;        
        desc= 0;
    }
    
    void Cmdline::setDescription(const OptionsDescription& desc)
    {
        this->desc= &desc;
    }

    typedef std::vector<Option> (options::detail::Cmdline::* style_parser)(const std::string&);


    namespace
	{
    	void fillUnknowOption(vector<Option>& result, const string& arg)
    	{
			Option opt;
			opt.value.push_back(arg);
			opt.original_tokens.push_back(arg);
			result.push_back(opt);
    	}
	}

	bool Cmdline::tryAllParser(const std::string& arg, vector<Option>& result)
	{
		static style_parser style_parsers[] = {&Cmdline::parseLongOption
											   , &Cmdline::parseShortOption};
		for(auto parser : style_parsers)
		{
			auto next  = (this->*parser)(arg);

			if(next.empty()) continue;

			result.insert(result.begin(), next.begin(), next.end());
			return true;
		}

		return false;
	}
		
    vector<Option> Cmdline::run()
    {
    	assert(desc);

    	vector<Option> result;
    	for(auto arg : args)
    	{
    		if (tryAllParser(arg, result))
			{
    			fillUnknowOption(result, arg);
    		}
    	}

		return result;
    }

namespace
{
    inline bool isLongOption(const string& tok)
    {
    	return tok.size() >= 3 && tok[0] == '-' && tok[1] == '-';
    }

    struct OptionParser
	{

	};

    struct LongOptionParser
	{
    	LongOptionParser(const string& tok)
    		: tok(tok)
    		, foundOption(0)
    	{
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
    	}

    	bool tryFind(const OptionsDescription& desc)
    	{
    		return foundOption = desc.find(name, false);
    	}

    	void fillOption(vector<Option>& result)
    	{
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

	private:
    	string name;
		string adjacent;
		const string& tok;
		const OptionDescription* foundOption;
	};
}

    vector<Option> Cmdline::parseLongOption(const string& tok)
    {
        vector<Option> result;

        if (!isLongOption(tok)) return result;

        LongOptionParser parser(tok);

        if(parser.tryFind(*desc))
        {
        	parser.fillOption(result);
        }

        return result;
    }

    namespace
	{
    	bool isShortOption(const string& tok)
    	{
    		return tok.size() >= 2 && tok[0] == '-' && tok[1] != '-';
    	}

    	struct ShortOptionParser
		{
    		ShortOptionParser(const string& tok)
    			: name(tok.substr(0,2))
    		 	, adjacent(tok.substr(2))
    			, tok(tok)
    			, foundOption(0)
    		{
    		}

    		bool tryFind(const OptionsDescription& desc)
    		{
    			return foundOption = desc.find(name, false);
    		}

    		bool finished() const
    		{
    			return adjacent.empty();
    		}

    		void addOption(vector<Option>& result)
    		{
    			Option opt;
    			opt.string_key = foundOption ? foundOption->getLongName():name;
    			opt.original_tokens.push_back(tok);
    			fillValue(opt.value);
    			result.push_back(opt);
    		}

    		void goNext()
    		{
    			name = string("-") + adjacent[0];
    			adjacent.erase(adjacent.begin());
    		}

		private:
    		void fillValue(vector<string>& value)
    		{
    			if(adjacent[0] == '=')
    			{
    				adjacent.erase(adjacent.begin());
    				value.push_back(adjacent);
    				adjacent.clear();
    			}
    		}

		private:
    		string name;
    		string adjacent;
    		const string& tok;
    		const OptionDescription* foundOption;
		};
	}

    vector<Option> Cmdline::parseShortOption(const string& tok)
    {
    	vector<Option> result;
    	if (!isShortOption(tok)) return result;

    	ShortOptionParser parser(tok);

    	while(true)
    	{
    		if(!parser.tryFind(*desc))
    		{
    			if(parser.finished())
    			{
    				parser.addOption(result);
    				break;
    			}
    			parser.goNext();
    		}
    		else
    		{
    			parser.addOption(result);
    			parser.goNext();
    		}
    	}
    	return result;
    }

}}
