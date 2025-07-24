#include <confy/v/integer.hpp>
#include <confy/v/std_string.hpp>
#include <doctest/doctest.h>

TEST_CASE("integer") {
    confy::integer<int> i;
    CHECK(i.source() == confy::value_source::none);
    i.set_value(42);
    CHECK(i.source() == confy::value_source::manual_override);
    CHECK(i.val() == 42);
}

TEST_CASE("std_string") {
    confy::std_string s;
    CHECK(s.source() == confy::value_source::none);
    s.set_value("hello");
    CHECK(s.source() == confy::value_source::manual_override);
    CHECK(s.val() == "hello");
}
