// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>

#if CONFY_SHARED
#   if BUILDING_CONFY
#       define CONFY_API SYMBOL_EXPORT
#   else
#       define CONFY_API SYMBOL_IMPORT
#   endif
#else
#   define CONFY_API
#endif
