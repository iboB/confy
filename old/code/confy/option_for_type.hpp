#pragma once

namespace confy
{

// specialize this to give a specific option type for one of your types
template <typename T, typename Enable = void>
struct option_for_type {};

// convenience typedef
template <typename T>
using option_for_type_t = typename option_for_type<T>::type;

}
