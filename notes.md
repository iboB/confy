# Notes

* Old features without a clear answer in the new codebase
    * Collecting value type and value description: say one wants to generate a json-schema or similar from a confy config tree. This was previously done by having more virtual methods in the node class:
        * get_value_type - short description (say `int32` or `enum`)
        * get_value_description - long description (say `a 32-bit signed integer` or `a|c|c|d`)
        * This can be done in the new code as well, but is there a way to make it external? The needs ot json-schema are vastly different from the needs ot a human readable help text.
* Validator description
    * The new code does allow for it, but the exact same problems as above apply
* Store an event log of the configuration process. Only throw exceptions in unrecoverable situations and instead collect all events (errors and non-errors) and return them to the user. Let them decide whether unused values are ok, or whether to throw a single-value or a multi-value exception.
