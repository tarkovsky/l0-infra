#ifndef VALUESEMANTIC_H
#define VALUESEMANTIC_H

#include <string>
#include <vector>
#include "Any.hpp"

namespace options {

    struct Value_semantic {
        virtual std::string name() const = 0;
        virtual unsigned min_tokens() const = 0;

        virtual unsigned max_tokens() const = 0;

        virtual bool is_composing() const = 0;

        virtual bool is_required() const = 0;
        
        virtual void parse(Any& value_store,
                           const std::vector<std::string>& new_tokens) const
            = 0;

        virtual bool apply_default(Any& value_store) const = 0;
                                   
        virtual void notify(const Any& value_store) const = 0;
        
        virtual ~Value_semantic() {}
    };

    struct Untyped_value : public Value_semantic
    {
    		Untyped_value(bool zero_tokens = false)
        : m_zero_tokens(zero_tokens)
        {}

        std::string name() const;

        unsigned min_tokens() const;
        unsigned max_tokens() const;

        bool is_composing() const { return false; }

        bool is_required() const { return false; }
        
        void parse(Any& value_store,
                    const std::vector<std::string>& new_tokens) const;

        bool apply_default(Any&) const { return false; }

        void notify(const Any&) const {}        
    private:
        bool m_zero_tokens;
    };

    struct Typed_value_base
    {
        virtual ~Typed_value_base() {}
    };


    template<class T>
    class typed_value : public Value_semantic,
                        public Typed_value_base
    {
    public:
        typed_value(T* store_to) 
        : m_store_to(store_to), m_composing(false),
          m_multitoken(false), m_zero_tokens(false)
        {} 

        typed_value* default_value(const T& v)
        {
            m_default_value = Any(v);
            return this;
        }

        typed_value* default_value(const T& v, const std::string& textual)
        {
            m_default_value = Any(v);
            m_default_value_as_text = textual;
            return this;
        }

        typed_value* value_name(const std::string& name)
        {
            m_value_name = name;
            return this;
        }

        typed_value* composing()
        {
            m_composing = true;
            return this;
        }

        typed_value* multitoken()
        {
            m_multitoken = true;
            return this;
        }

        typed_value* zero_tokens() 
        {
            m_zero_tokens = true;
            return this;
        }
            
    public:

        std::string name() const;

        bool is_composing() const { return m_composing; }

        unsigned min_tokens() const
        {
            if (m_zero_tokens) {
                return 0;
            } else {
                return 1;
            }
        }

        unsigned max_tokens() const {
            if (m_multitoken) {
                return 32000;
            } else if (m_zero_tokens) {
                return 0;
            } else {
                return 1;
            }
        }

        void parse(Any& value_store,
                    const std::vector< std::string>& new_tokens)
            const;

        virtual bool apply_default(Any& value_store) const
        {
            if (m_default_value.empty()) {
                return false;
            } else {
                value_store = m_default_value;
                return true;
            }
        }

        void notify(const Any& value_store) const;

    private:
        T* m_store_to;
        std::string m_value_name;
        Any m_default_value;
        std::string m_default_value_as_text;
        std::string m_implicit_value_as_text;
        bool m_composing, m_multitoken, m_zero_tokens;
    };


    template<class T>
    typed_value<T>*
    value();

    template<class T>
    typed_value<T>*
    value(T* v);



}

#include "detail/ValueSemantic.hpp"

#endif

