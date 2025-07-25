// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/value.hpp>
#include <confy/exception.hpp>
#include <confy/dict.hpp>
#include <doctest/doctest.h>
#include "setenv.inl"

struct test_value final : public confy::value {
    explicit test_value(confy::node_desc desc, confy::node* owner) noexcept
        : value(std::move(desc), owner)
    {}

    test_value()
        : test_value({.name = "test_value", .description = "A test value", .abbr = "tv"}, nullptr)
    {}

    std::string str;

    struct dsl : public value::tdsl<test_value> {
        using value::tdsl<test_value>::tdsl;
    };

    virtual std::string to_string() const noexcept override {
        return str;
    }
    virtual confy::dict to_dict() const noexcept override {
        return str;
    }
    virtual void set_value_from_dict(const confy::dict& d) override {
        str = d.get<std::string>();
    }
    virtual void set_value_from_string(std::string_view sv) override {
        if (sv == "nope") {
            throw confy::exception("nope");
        }
        str = std::string(sv);
    }
    virtual void validate_value() const override {
        if (str == "bad") {
            throw confy::exception("no bad");
        }
    }
};

TEST_CASE("basics") {
    test_value val;
    CHECK(val.name() == "test_value");
    CHECK(val.desc() == "A test value");
    CHECK(val.abbr() == "tv");
    CHECK(val.env_var_data().str.empty());
    CHECK(val.env_var_data().strategy == confy::env::var_strategy::automatic);

    CHECK(val.required());
    CHECK(val.source() == confy::value_source::default_val);
}

TEST_CASE("dsl") {
    test_value val;
    test_value::dsl dsl(val);

    auto& rr = dsl
        .name("test")
        .desc("test value")
        .abbr("t")
        .env_var("TEST")
        .optional()
        .val();

    CHECK(&rr == &val);
    CHECK(val.name() == "test");
    CHECK(val.desc() == "test value");
    CHECK(val.abbr() == "t");
    CHECK(val.env_var_data().str == "TEST");
    CHECK(val.env_var_data().strategy == confy::env::var_strategy::manual);
    CHECK(val.source() == confy::value_source::none);
    CHECK_FALSE(val.required());
}

TEST_CASE("str") {
    test_value val;
    val.set_from_string("xxx", confy::value_source::config_file);
    CHECK(val.str == "xxx");
    val.set_from_string("yyy", confy::value_source::default_val);
    CHECK(val.str == "xxx");
    CHECK(val.source() == confy::value_source::config_file);
    CHECK(val.to_string() == "xxx");
    val.set_from_string("zzz", confy::value_source::cmd_line);
    CHECK(val.str == "zzz");
    CHECK(val.source() == confy::value_source::cmd_line);
    CHECK(val.to_string() == "zzz");
    val.set_from_string("www", confy::value_source::cmd_line);
    CHECK(val.str == "www");

    CHECK_THROWS_WITH(val.set_from_string("nope", confy::value_source::manual_override),
        "test_value: nope");

    CHECK(val.str == "www");
    CHECK(val.source() == confy::value_source::cmd_line);
}

TEST_CASE("dict") {
    using dict = confy::dict;
    test_value val;
    val.set_from_dict(dict("xxx"), confy::value_source::config_file);
    CHECK(val.str == "xxx");
    val.set_from_dict(dict("yyy"), confy::value_source::default_val);
    CHECK(val.str == "xxx");
    CHECK(val.source() == confy::value_source::config_file);
    auto d = val.to_dict();
    CHECK(d.is_string());
    CHECK(d.get<std::string_view>() == "xxx");
    val.set_from_dict(dict("zzz"), confy::value_source::cmd_line);
    CHECK(val.str == "zzz");
    CHECK(val.source() == confy::value_source::cmd_line);
    d = val.to_dict();
    CHECK(d.is_string());
    CHECK(d.get<std::string_view>() == "zzz");
    val.set_from_dict(dict("www"), confy::value_source::cmd_line);
    CHECK(val.str == "www");

    CHECK_THROWS_WITH(val.set_from_dict(dict(1.5), confy::value_source::manual_override),
        "test_value: [json.exception.type_error.302] type must be string, but is number");

    CHECK(val.str == "www");
    CHECK(val.source() == confy::value_source::cmd_line);
}

TEST_CASE("env var") {
    using dict = confy::dict;
    test_value val({.name = "CONFY_TEST_VALUE"}, nullptr);
    val.try_set_from_env_var();
    CHECK(val.source() == confy::value_source::default_val);

    set_env_var("CONFY_TEST_VALUE", "nope");
    CHECK_THROWS_WITH(val.try_set_from_env_var(),
        "CONFY_TEST_VALUE: nope from env var CONFY_TEST_VALUE=nope");

    set_env_var("CONFY_TEST_VALUE", "aaa");
    val.try_set_from_env_var();
    CHECK(val.str == "aaa");
    CHECK(val.source() == confy::value_source::env_var);
    CHECK(val.to_string() == "aaa");

    // env vars don't override values which have an env var env_var
    set_env_var("CONFY_TEST_VALUE", "bbb");
    val.try_set_from_env_var();
    CHECK(val.str == "aaa");

    val.set_from_string("ccc", confy::value_source::manual_override);
    val.try_set_from_env_var();
    CHECK(val.str == "ccc");

    set_env_var("CONFY_TEST_VALUE", nullptr);
}

TEST_CASE("validate") {
    test_value val;

    CHECK(val.source() == confy::value_source::default_val);
    CHECK_NOTHROW(val.validate());
    test_value::dsl(val).optional();
    CHECK(val.source() == confy::value_source::none);
    CHECK_NOTHROW(val.validate());
    test_value::dsl(val).required();
    CHECK(val.source() == confy::value_source::none);
    CHECK_THROWS_WITH(val.validate(), "test_value: value is required but not set");

    val.set_from_string("good", confy::value_source::manual_override);
    CHECK_NOTHROW(val.validate());

    val.set_from_string("bad", confy::value_source::manual_override);
    CHECK_THROWS_WITH(val.validate(), "test_value: value 'bad' failed validation: no bad");
}
