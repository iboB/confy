// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/v/common_value.hpp>
#include <confy/exception.hpp>
#include <doctest/doctest.h>
#include <confy/dict.hpp>

namespace confy {
template <>
struct common_value_type_traits<std::string> {
    using value_type = std::string;
    using ref_type = std::string&;
    using const_ref_type = const std::string&;

    static std::string to_string(const std::string& val) noexcept {
        return val + "-s";
    }
    static dict to_dict(const std::string& val) noexcept {
        return val + "-d";
    }
    static void set_value_from_string(std::string& val, std::string_view str) {
        if (str == "nope") {
            throw confy::exception("nope");
        }
        val = "s-" + std::string(str);
    }
    static void set_value_from_dict(std::string& val, const dict& d) {
        val = "d-" + d.get<std::string>();
    }
};
} // namespace confy

using test_value = confy::common_value<std::string>;

TEST_CASE("basics") {
    test_value val;
    CHECK(val.name().empty());
    CHECK(val.desc().empty());
    CHECK(val.abbr().empty());
    CHECK(val.env_var_data().str.empty());
    CHECK(val.env_var_data().strategy == confy::env::var_strategy::automatic);
    CHECK(val.required());
    CHECK(val.source() == confy::value_source::none);

    val.set_value("aaa", confy::value_source::cmd_line);
    CHECK(val.val() == "aaa");
    val.set_value("xxx", confy::value_source::default_val);
    CHECK(val.val() == "aaa");
    CHECK(val.source() == confy::value_source::cmd_line);
    val.set_value("xxx");
    CHECK(val.val() == "xxx");
    CHECK(val.source() == confy::value_source::manual_override);
}

TEST_CASE("dsl") {
    test_value val;
    test_value::dsl dsl(val);

    auto& rr = dsl
        .name("test")
        .desc("test value")
        .abbr("t")
        .env_var("TEST")
        .default_val("ddd")
        .optional()
        .val();

    CHECK(&rr == &val);
    CHECK(val.name() == "test");
    CHECK(val.desc() == "test value");
    CHECK(val.abbr() == "t");
    CHECK(val.env_var_data().str == "TEST");
    CHECK(val.env_var_data().strategy == confy::env::var_strategy::manual);
    CHECK(val.source() == confy::value_source::default_val);
    CHECK(val.val() == "ddd");
    CHECK_FALSE(val.required());
}

TEST_CASE("str") {
    using dict = confy::dict;
    test_value val({.name = "test_value"}, nullptr);
    val.set_from_string("xxx", confy::value_source::config_file);
    CHECK(val.val() == "s-xxx");
    CHECK(val.to_string() == "s-xxx-s");
    CHECK_THROWS_WITH(val.set_from_string("nope", confy::value_source::manual_override),
        "test_value: nope");
    CHECK(val.val() == "s-xxx");
    CHECK(val.source() == confy::value_source::config_file);
}

TEST_CASE("dict") {
    using dict = confy::dict;
    test_value val({.name = "test_value"}, nullptr);
    val.set_from_dict(dict("xxx"), confy::value_source::config_file);
    CHECK(val.val() == "d-xxx");
    auto d = val.to_dict();
    CHECK(d.is_string());
    CHECK(d.get<std::string_view>() == "d-xxx-d");
}

TEST_CASE("ref") {
    using test_rvalue = confy::common_value<std::string&>;

    {
        std::string str = "test";
        test_rvalue val(str, { .name = "test_value" }, nullptr);

        CHECK(&val.val() == &str);
        CHECK(val.to_string() == "test-s");
        CHECK(val.to_dict() == "test-d");

        val.set_from_string("sss", confy::value_source::config_file);
        CHECK(str == "s-sss");

        val.set_from_dict(confy::dict("ddd"), confy::value_source::cmd_line);
        CHECK(str == "d-ddd");

        val.set_value("xxx", confy::value_source::manual_override);
        CHECK(str == "xxx");
    }

    {
        std::string str;
        test_rvalue val(str);
        test_rvalue::dsl(val)
            .default_val("ddd")
            ;

        CHECK(val.source() == confy::value_source::default_val);
        CHECK(str == "ddd");
    }
}

TEST_CASE("validate") {
    test_value val({.name = "test"}, nullptr);
    test_value::dsl(val)
        .validate([](std::string_view str) { return str != "dog"; })
        .validate([](const std::string& str) { return str != "horse"; }, "!horse")
        ;

    val.set_value("cat");
    CHECK_NOTHROW(val.validate());

    val.set_value("dog");
    CHECK_THROWS_WITH(val.validate(), "test: value 'dog-s' failed validation");

    val.set_value("horse");
    CHECK_THROWS_WITH(val.validate(), "test: value 'horse-s' failed validation: !horse");
}
