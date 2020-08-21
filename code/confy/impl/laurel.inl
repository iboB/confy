// laurel = _Y_et _A_nother i_NI_ file parser
//
// Copyright (c) 2020 Borislav Stanimirov
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// usage laurel(istream, section_func, key_func, error_func)
// section_func = a functor compatible with void(string_view)
//    called for each new section
// key_func = a functor compatible with void(string_view k, string_view v)
//    called for each new key and its value
// error_func = a functor compatible with void(laurel_error error, int line)
//    called for errors and gives the line along with the text

#include <string_view>
#include <iostream>
#include <cctype>

namespace
{

enum class laurel_error
{
    missing_cb, // section missing closing bracket [
    missing_eq  // key missing equality sign =
};

const char* laurel_error_to_text(laurel_error error)
{
    switch (error) {
    case laurel_error::missing_cb: return "section missing closing `]`";
    case laurel_error::missing_eq: return "key missing `=`";
    default: return "";
    }
}

template <typename OnSec, typename OnKey, typename OnError>
void laurel(std::istream& in, OnSec&& on_section, OnKey&& on_key, OnError&& on_error)
{
    std::string raw_line;
    int line_num = 0;
    while (!in.eof())
    {
        ++line_num;

        getline(in, raw_line);

        std::string_view line;

        {
            // leading spaces
            size_t start = 0;
            for (; start < raw_line.length(); ++start)
            {
                if (!isspace(raw_line[start])) break;
            }

            // empty line
            if (start == raw_line.length()) continue;

            // trailing spaces
            size_t end = raw_line.length() - 1;
            for (; end < raw_line.length(); --end)
            {
                if (!isspace(raw_line[end])) break;
            }

            line = std::string_view(raw_line.c_str() + start, end - start + 1);
        }

        if (line.empty()) continue;

        // comments
        if (line.front() == ';' || line.front() == '#') continue;

        // section
        if (line.front() == '[')
        {
            auto close = line.find(']');
            if (close == std::string_view::npos) return on_error(laurel_error::missing_cb, line_num);
            on_section(line.substr(1, close - 1));
            continue;
        }

        // value
        auto eq = line.find('=');
        if (eq == std::string_view::npos) return on_error(laurel_error::missing_eq, line_num);

        on_key(line.substr(0, eq), line.substr(eq + 1));
    }
}

}
