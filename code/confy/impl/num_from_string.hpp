// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <charconv>

namespace confy::impl
{
template <typename Num>
bool num_from_string(Num& val, std::string_view str)
{
    auto b = str.data();
    auto e = b + str.length();
    auto result = std::from_chars(b, e, val);
    if (result.ec != std::errc()) return false;
    if (result.ptr != e) return false;
    return true;
}
}
