# confy

A C++17 application configuration library which is:

* Easy to use
* Configurable
* Extensible

*Although usable for the most part, the library is still in development. It's close to version v1.0.0. The progress towards v1.0.0 can be followed [here](https://github.com/iboB/confy/projects/1)*

## The gist

A short example:

```c++
    struct app_configuration
    {
        std::string name;
        int age = 0;
        enum class role_t
        {
            ceo, employee, intern
        };
        role_t role = role_t::intern;
    };

    // ...

    app_configuration c;

    confy::config cfg;
    cfg.schema()
        .opt(c.name, "name", "n", "The name of the person")
        .opt(c.age, "age", "a", "The age of the person")
        .opt(c.role, "role", "r", "The role of the person");

    cfg.parse_cmd_line(argc, argv);

    std::cout
        << "Name: " << c.name
        << "\nAge: " << c.age
        << "\nRole: " << confy::magic_enum::enum_name(c.role)
        << "\n";
```

It might seem that confy is Yet Another&trade; command line argument parser, but although it can be used as such, that's not its main power. It's configuration. If some of the options are not provided from the command line, it will search environment variables with that name and try to load values from there. The library also supports parsing of .ini files.

Each option in the schema tries to get it's value from the following sources (top has the highest priority):

* Manual user override
* Command line
* Config (.ini) file
* Environment variable
* User-provided default fallback

The library is highly configurable by its schema-definition EDSL and can be extended with various user-defined option types (ie a point) and value sources (ie a .json file).

## Building

You can use CMake. There is no special configuration involved to simply build the library.

## Contributing

All contributions in the form of issues and pull requests are welcome.

### Development

In order to buld the tests, you have to fetch the [doctest](https://github.com/onqtam/doctest) submodule.

## License

The library is distributed under the MIT Software License. See LICENSE.txt for further details or copy [here](http://opensource.org/licenses/MIT).

Copyright &copy; 2020-2024 Borislav Stanimirov.

confy embeds [Magic Enum](https://github.com/Neargye/magic_enum), &copy; 2019-2020 Daniil Goncharov, which also under the MIT Softwre License.
