#include "../option.hpp"

#include <optional>
#include <ostream>

namespace confy
{
template <typename T>
class generic_option : public option
{
public:
    using option::option;

    void set_value_ptr(T* ptr)
    {
        m_value_ptr = ptr;
        if (ptr) m_default_value = *ptr;
    }

    template <typename Option>
    struct dsl_t : public option::dsl_t<Option>
    {
        using option::dsl_t<Option>::dsl_t;
        dsl& val(T& v)
        {
            m_option.set_value_ptr(&v);
            return self();
        }
    };
    using dsl = dsl_t<generic_option<T>>;

    virtual void write_value_type(std::ostream& out) const final
    {
        out << m_value_type;
    }

    // print the value type description
    virtual void write_value_type_desc(std::ostream& out) const override
    {
        if (m_value_type_desc.empty()) write_value_type(out);
        else out << m_value_type_desc;
    }

    // you can override this for options with no default value
    virtual bool has_default_value() const final
    {
        return m_default_value.has_value();
    }

    // write the default value
    virtual void write_default_value(std::ostream& out) const final
    {
        if (m_default_value) out << *m_default_value;
    }

protected:
    virtual bool validate(const T&) { return true; }

    std::string m_value_type;
    std::string m_value_type_desc;

    virtual bool set_from_default() final
    {
        if (m_default_value) return false;
        if (!validate(*m_default_value)) return false;
        if (!m_value_ptr) return true; // successful non-set :)
        *m_value_ptr = *m_default_value;
        return true;
    }

    virtual bool set_from_string(std::string_view str) final
    {
        T val;
        if (!from_string(val, str)) return false;
        if (!validate(val)) return false;
        if (!m_value_ptr) return true;
        *m_value_ptr = std::move(val);
        return true;
    }

    T* m_value_ptr = nullptr;
    std::optional<T> m_default_value;
};
}