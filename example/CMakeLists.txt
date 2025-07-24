# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
macro(confy_example name)
    set(ename confy-example-${name})
    add_executable(${ename} e-${name}.cpp ${ARGN})
    target_link_libraries(${ename} confy::confy)
    set_target_properties(${ename} PROPERTIES FOLDER example)
endmacro()

confy_example(basic)
