#include "doctest.hpp"
#include "impl/laurel.inl"

#include <sstream>
#include <vector>
#include <string>

TEST_SUITE_BEGIN("Laurel");

struct tester_entry
{
    enum etype { sec, key, err };
    etype type;

    std::string str;
    std::string key_value;
    int err_line;

    tester_entry(std::string_view sname) : type(sec), str(sname) {}
    tester_entry(std::string_view kname, std::string_view kval) : type(key), str(kname), key_value(kval) {}
    tester_entry(std::string_view etext, int eline) : type(err), str(etext), err_line(eline) {}
};

struct tester
{
    tester(std::vector<tester_entry> e) : entries(std::move(e)) {}
    ~tester()
    {
        CHECK(counter == entries.size());
    }

    const tester_entry& next() const
    {
        return entries[counter++];
    }

    void operator()(std::string_view sname) const
    {
        auto& e = next();
        CHECK(e.type == tester_entry::sec);
        CHECK(e.str == sname);
    }

    void operator()(std::string_view kname, std::string_view kval) const
    {
        auto& e = next();
        CHECK(e.type == tester_entry::key);
        CHECK(e.str == kname);
        CHECK(e.key_value == kval);
    }

    void operator()(std::string_view etext, int eline) const
    {
        auto& e = next();
        CHECK(e.type == tester_entry::err);
        CHECK(e.str == etext);
        CHECK(e.err_line == eline);
    }

    mutable size_t counter = 0;
    std::vector<tester_entry> entries;
};

TEST_CASE("test parse")
{
    const char* ini = R"ini(
        ; inside comment
val1=55
val2=asd
; another comment
[subsec]
svalue=123.43.12
sp ace=va lue
    )ini";
    tester t({ {"val1", "55"}, {"val2", "asd"}, {"subsec"}, {"svalue", "123.43.12"}, {"sp ace", "va lue"} });
    std::istringstream sini(ini);
    laurel(sini, t, t, t);
}

TEST_CASE("test error =")
{
    const char* ini = "val2asd";
    tester t({ {"key missing `=`", 1} });
    std::istringstream sini(ini);
    laurel(sini, t, t, t);
}

TEST_CASE("test error ]")
{
    const char* ini = "a=5  \n;comment\n[ss";
    tester t({ {"a", "5"}, {"section missing closing `]`", 3} });
    std::istringstream sini(ini);
    laurel(sini, t, t, t);
}