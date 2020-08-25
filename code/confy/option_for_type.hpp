#pragma once

namespace confy
{

template <typename T, typename Enable = void>
struct option_for_type {};

template <typename T>
using option_for_type_t = typename option_for_type<T>::type;

}
