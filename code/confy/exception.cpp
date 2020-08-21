#include "exception.hpp"

namespace confy
{

exception::~exception() = default;

schema_exception::schema_exception(std::string text)
    : m_text(std::move(text))
{}
schema_exception::~schema_exception() = default;
const char* schema_exception::what() const noexcept { return m_text.c_str(); }

}
