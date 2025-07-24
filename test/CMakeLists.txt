# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/doctest-util@0.1.3)
set_target_properties(doctest PROPERTIES FOLDER test)
set_target_properties(doctest-main PROPERTIES FOLDER test)

macro(confy_test test)
    add_doctest_lib_test(${test} confy t-${test}.cpp ${ARGN})
endmacro()

confy_test(setenv)
confy_test(laurel)
confy_test(cli)

confy_test(value_type_traits)
confy_test(ref_value_types)
confy_test(value_types)
confy_test(std_optional)

confy_test(section)
confy_test(node_tree)

confy_test(configurator)
