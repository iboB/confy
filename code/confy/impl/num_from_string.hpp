// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// a normal person would use charconv here
// but gcc is stupid :(
//
//#include <charconv>
//
//namespace confy::impl
//{
//template <typename Num>
//bool num_from_string(Num& val, std::string_view str)
//{
//    auto b = str.data();
//    auto e = b + str.length();
//    auto result = std::from_chars(b, e, val);
//    if (result.ec != std::errc()) return false;
//    if (result.ptr != e) return false;
//    return true;
//}
//}

#include <string>
#include <string_view>

namespace confy::impl
{
void ucheck(const std::string& s)
{
    if (s.empty()) return;
    if (s.front() == '-') throw std::invalid_argument("negative");
}
void stov(const std::string& s, std::size_t* pos, int& val)
{
    val = stoi(s, pos);
}
void stov(const std::string& s, std::size_t* pos, unsigned int& val)
{
    ucheck(s);
    val = int(stoul(s, pos));
}
void stov(const std::string& s, std::size_t* pos, long& val)
{
    val = stol(s, pos);
}
void stov(const std::string& s, std::size_t* pos, unsigned long& val)
{
    ucheck(s);
    val = stol(s, pos);
}
void stov(const std::string& s, std::size_t* pos, long long& val)
{
    val = stoll(s, pos);
}
void stov(const std::string& s, std::size_t* pos, unsigned long long& val)
{
    ucheck(s);
    val = stoul(s, pos);
}
void stov(const std::string& s, std::size_t* pos, float& val)
{
    val = stof(s, pos);
}
void stov(const std::string& s, std::size_t* pos, double& val)
{
    val = stod(s, pos);
}
void stov(const std::string& s, std::size_t* pos, long double& val)
{
    val = stold(s, pos);
}

template <typename Num>
bool num_from_string(Num& val, std::string_view view)
{
    try
    {
        std::string str(view);
        size_t pos;
        Num tval;
        stov(str, &pos, tval);
        if (pos < str.length()) return false;
        val = tval;
        return true;
    }
    catch (...)
    {
        return false;
    }
}
}