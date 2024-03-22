# Program

This C++ tool was developed to help users parse CLI arguments. It is inspired by the argparse module in Python. The tool is able to parse arguments and flags, and return the values in a dictionary. The tool is also able to display a help message to the user, which describes the usage of the tool.

# How to use

To use cppargparser, you must first include the header file in your program. For example:

```cpp
#include "cppargparser.hpp"
```

To add arguments and flags to the parser, you must use `addArgument` function, which takes as input `name: std::string`, `positional: bool`, `required: bool`, `help: std::string`, `type: cppargparser::ArgType::Type` and `default_value: std::string`. For example:

```cpp
#include "cppargparser.hpp"

int main(int argc, const char* argv[]) {
    cppargparser::ArgParser *parser = new cppargparser::ArgParser();
    parser->addArgument("-string", false, false, "A string argument", cppargparser::ArgType::STRING, ""); // You can ommit all arguments except the first two. The default values are: required = false, help = "", type = cppargparser::ArgType::STRING and default_value = ""
    parser->addArgument("-int", false, false, "An integer argument", cppargparser::ArgType::INT, "");
    parser->addArgument("-float", false, false, "A float argument", cppargparser::ArgType::FLOAT, "");
    parser->addArgument("-bool", false, false, "A boolean argument", cppargparser::ArgType::BOOL, "false"); // when dealing with bool arguments, the default value must be either "true" or "false"
    parser->addArgument("-char", false, false, "A char argument", cppargparser::ArgType::CHAR, "");
    parser->parse(argc, argv);

    delete parser; // Use this at the end of your code to free the memory allocated by the parser
    return EXIT_SUCCESS;
}
```

To access the arguments and flags, you must use the `getTypedArg` template function, which takes as input the name of the argument or flag and the type of the argument or flag. For example:

```cpp
#include "cppargparser.hpp"

int main(int argc, const char* argv[]) {
    cppargparser::ArgParser *parser = new cppargparser::ArgParser();
    parser->addArgument("-string", false, false, "A string argument", cppargparser::ArgType::STRING, "");
    parser->addArgument("-int", false, false, "An integer argument", cppargparser::ArgType::INT, "");
    parser->addArgument("-float", false, false, "A float argument", cppargparser::ArgType::FLOAT, "");
    parser->addArgument("-bool", false, false, "A boolean argument", cppargparser::ArgType::BOOL, "false");
    parser->addArgument("-char", false, false, "A char argument", cppargparser::ArgType::CHAR, "");
    parser->parse(argc, argv);

    std::string string_arg = parser->getTypedArg<std::string>("-string");
    int int_arg = parser->getTypedArg<int>("-int");
    float float_arg = parser->getTypedArg<float>("-float");
    bool bool_arg = parser->getTypedArg<bool>("-bool");
    char char_arg = parser->getTypedArg<char>("-char");

    delete parser;
    return EXIT_SUCCESS;
}
```

If you want to give positional arguments, you must set the `positional` argument to `true` in the `addArgument` function. For example:

```cpp
#include "cppargparser.hpp"

int main(int argc, const char* argv[]) {
    cppargparser::ArgParser *parser = new cppargparser::ArgParser();
    parser->addArgument("string", true, false, "A string argument", cppargparser::ArgType::STRING, "");
    parser->parse(argc, argv);

    std::string string_arg = parser->getTypedArg<std::string>("string");

    delete parser;
    return EXIT_SUCCESS;
}
```

# Known issues

When passing multiple positional args, the parser will only parse the first one. This is a known issue and will be fixed in future versions.
