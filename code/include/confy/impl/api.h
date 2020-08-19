#pragma once

#if !defined(CONFY_API)
#   if defined(CONFY_SHARED)
#       if defined(_WIN32)
#           define CONFY_SYM_EXPORT __declspec(dllexport)
#           define CONFY_SYM_IMPORT __declspec(dllimport)
#       else
#           define CONFY_SYM_EXPORT __attribute__((__visibility__("default")))
#           define CONFY_SYM_IMPORT
#       endif

#       if defined(CONFY_IMPLEMENT)
#           define CONFY_API CONFY_SYM_EXPORT
#       else
#           define CONFY_API CONFY_SYM_IMPORT
#       endif
#   else
#       define CONFY_API
#   endif
#endif
