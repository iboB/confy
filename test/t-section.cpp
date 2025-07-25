// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/section.hpp>
#include <doctest/doctest.h>

TEST_CASE("basics") {
    confy::section sec({
        .name = "test",
        .description = "test section",
        .abbr = "t",
        .env_var = {.str = "foo", .strategy = confy::env::var_strategy::manual_global}
    }, nullptr);

    CHECK(sec.abbr() == "t");
    CHECK(sec.owner() == nullptr);
    CHECK(sec.get_path() == "test");
    CHECK(sec.get_abbr_path() == "t");
    CHECK(sec.desc() == "test section");
    CHECK(sec.env_var_data().str == "foo");
    CHECK(sec.env_var_data().strategy == confy::env::var_strategy::manual_global);

    auto& ssa = sec.add_section({.name = "ssa", .abbr = "a"});
    CHECK(ssa.abbr() == "a");
    CHECK(ssa.owner() == &sec);
    CHECK(ssa.get_path() == "test.ssa");
    CHECK(ssa.get_abbr_path() == "t.a");
    CHECK(ssa.desc().empty());
    CHECK(ssa.env_var_data().str.empty());
    CHECK(ssa.env_var_data().strategy == confy::env::var_strategy::automatic);

    auto& ssb = sec.add_section({.name = "ssb", .abbr = "b"});
    CHECK(ssb.owner() == &sec);
    auto& ssax = ssa.add_section({
        .name = "xxx",
        .description = "a x",
        .abbr = "x",
        .env_var = {.str = "bar", .strategy = confy::env::var_strategy::manual}
    });
    CHECK(ssax.owner() == &ssa);
    CHECK(ssax.desc() == "a x");
    CHECK(ssax.env_var_data().str == "bar");
    CHECK(ssax.env_var_data().strategy == confy::env::var_strategy::manual);

    CHECK(ssa.get_path() == "test.ssa");
    CHECK(ssa.get_abbr_path() == "t.a");
    CHECK(ssb.get_path() == "test.ssb");
    CHECK(ssb.get_abbr_path() == "t.b");
    CHECK(ssax.get_path() == "test.ssa.xxx");
    CHECK(ssax.get_abbr_path() == "t.a.x");
}

TEST_CASE("empty base") {
    confy::section sec({}, nullptr);
    auto& ssa = sec.add_section({.name = "ssa", .abbr = "a"});
    auto& ssb = sec.add_section({.name = "ssb", .abbr = "b"});
    auto& ssax = ssa.add_section({.name = "xxx", .abbr = "x"});
    CHECK(ssa.get_path() == "ssa");
    CHECK(ssa.get_abbr_path() == "a");
    CHECK(ssb.get_path() == "ssb");
    CHECK(ssb.get_abbr_path() == "b");
    CHECK(ssax.get_path() == "ssa.xxx");
    CHECK(ssax.get_abbr_path() == "a.x");
}

TEST_CASE("get_child") {
    confy::section sec({.name = "test", .abbr = "t"}, nullptr);
    auto& ssa = sec.add_section({.name = "ssa", .abbr = "a"});
    auto& ssb = sec.add_section({.name = "ssb", .abbr = "b"});
    auto& ssax = ssa.add_section({.name = "xxx", .abbr = "x"});

    CHECK_FALSE(sec.get_child("a"));
    CHECK_FALSE(sec.get_abbr_child("ssa"));
    CHECK(sec.get_child("ssa") == &ssa);
    CHECK(sec.get_abbr_child("a") == &ssa);
    CHECK(sec.get_child("ssb") == &ssb);
    CHECK(sec.get_abbr_child("b") == &ssb);

    CHECK(sec.get_child("ssa.xxx") == &ssax);
    CHECK(sec.get_abbr_child("a.x") == &ssax);

    CHECK_FALSE(sec.get_child("ssb.xxx"));
    CHECK_FALSE(sec.get_abbr_child("b.x"));
}

TEST_CASE("no dup") {
    confy::section sec({.name = "test", .abbr = "t"}, nullptr);
    auto& ssa = sec.add_section({.name = "ssa", .abbr = "a"});
    ssa.add_section({.name = "xxx", .abbr = "x"});

    CHECK_THROWS_WITH(sec.add_section({.name = "ssa"}),
        "test: child with name 'ssa' already exists");
    CHECK_THROWS_WITH(ssa.add_section({.name = "xxx"}),
        "test.ssa: child with name 'xxx' already exists");
    CHECK_THROWS_WITH(ssa.add_section({.name = "zzz", .abbr = "x"}),
        "test.ssa: child with abbreviation 'x' already exists");
}

TEST_CASE("noops") {
    confy::section sec({.name = "test"}, nullptr);
    sec.add_section({.name = "ssb"});
    auto& ssa = sec.add_section({.name = "ssa"});
    ssa.add_section({.name = "xxx"});

    CHECK_NOTHROW(sec.validate());
    CHECK_NOTHROW(sec.try_set_from_env_var());

    confy::node& node = ssa;
    CHECK(node.to_string() == "");
    CHECK_THROWS_WITH(node.set_from_string("", {}), "test.ssa: section cannot be set from string");
}

TEST_CASE("get_env_var_name") {
    using enum confy::env::var_strategy;

    SUBCASE("automatic") {
        confy::section aa({.name = "aa", .env_var = {.str = "xx"}}, nullptr);
        CHECK(aa.get_env_var_name() == "aa");
        SUBCASE("automatic") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy"}});
            CHECK(bb.get_env_var_name() == "aa_bb");
            SUBCASE("automatic") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz"}});
                CHECK(cc.get_env_var_name() == "aa_bb_cc");
            }
            SUBCASE("manual") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual}});
                CHECK(cc.get_env_var_name() == "aa_bb_zz");
            }
            SUBCASE("manual_global") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual_global}});
                CHECK(cc.get_env_var_name() == "zz");
            }
            SUBCASE("none") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = none}});
                CHECK(cc.get_env_var_name() == "");
            }
        }
        SUBCASE("manual") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = manual}});
            CHECK(bb.get_env_var_name() == "aa_yy");
            SUBCASE("automatic") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz"}});
                CHECK(cc.get_env_var_name() == "aa_yycc");
            }
            SUBCASE("manual") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual}});
                CHECK(cc.get_env_var_name() == "aa_yyzz");
            }
            SUBCASE("manual_global") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual_global}});
                CHECK(cc.get_env_var_name() == "zz");
            }
            SUBCASE("none") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = none}});
                CHECK(cc.get_env_var_name() == "");
            }
        }
        SUBCASE("manual_global") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = manual_global}});
            CHECK(bb.get_env_var_name() == "yy");
            SUBCASE("automatic") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz"}});
                CHECK(cc.get_env_var_name() == "yycc");
            }
            SUBCASE("manual") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual}});
                CHECK(cc.get_env_var_name() == "yyzz");
            }
            SUBCASE("manual_global") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual_global}});
                CHECK(cc.get_env_var_name() == "zz");
            }
            SUBCASE("none") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = none}});
                CHECK(cc.get_env_var_name() == "");
            }
        }
        SUBCASE("none") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = none}});
            CHECK(bb.get_env_var_name() == "");
            SUBCASE("automatic") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz"}});
                CHECK(cc.get_env_var_name() == "");
            }
        }
    }
    SUBCASE("manual") {
        confy::section aa({.name = "aa", .env_var = {.str = "xx", .strategy = manual}}, nullptr);
        CHECK(aa.get_env_var_name() == "xx");
        SUBCASE("automatic") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy"}});
            CHECK(bb.get_env_var_name() == "xxbb");
            SUBCASE("automatic") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz"}});
                CHECK(cc.get_env_var_name() == "xxbb_cc");
            }
            SUBCASE("manual") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual}});
                CHECK(cc.get_env_var_name() == "xxbb_zz");
            }
            SUBCASE("manual_global") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual_global}});
                CHECK(cc.get_env_var_name() == "zz");
            }
        }
        SUBCASE("manual") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = manual}});
            CHECK(bb.get_env_var_name() == "xxyy");
            SUBCASE("automatic") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz"}});
                CHECK(cc.get_env_var_name() == "xxyycc");
            }
            SUBCASE("manual") {
                auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual}});
                CHECK(cc.get_env_var_name() == "xxyyzz");
            }
        }
        SUBCASE("manual_global") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = manual_global}});
            CHECK(bb.get_env_var_name() == "yy");
        }
        SUBCASE("none") {
            auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = none}});
            CHECK(bb.get_env_var_name() == "");
        }
    }
    SUBCASE("manual_global") {
        confy::section aa({.name = "aa", .env_var = {.str = "xx", .strategy = manual_global}}, nullptr);
        CHECK(aa.get_env_var_name() == "xx");
    }
    SUBCASE("none") {
        confy::section aa({.name = "aa", .env_var = {.str = "xx", .strategy = none}}, nullptr);
        CHECK(aa.get_env_var_name() == "");
        auto& bb = aa.add_section({.name = "bb", .env_var = {.str = "yy", .strategy = manual}});
        auto& cc = bb.add_section({.name = "cc", .env_var = {.str = "zz", .strategy = manual}});
        CHECK(cc.get_env_var_name() == "");
    }
}
