#ifndef CPPARGPARSER_HPP
#define CPPARGPARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <sstream>
#include <any>
#include <stdexcept>
#include <typeinfo>

namespace cppargparser {

	enum class ArgType {
		STRING,
		INT,
		FLOAT,
		DOUBLE,
		BOOL,
		CHAR,
		VECTOR,
		MAP
	};

	struct ArgTypeMatcher {
		std::any value;
		std::string expected;
		std::string actual;
	};

	ArgTypeMatcher stringToInt(const std::string& s) {
		try {
			int res = std::stoi(s);
			return ArgTypeMatcher{res, "int", "int"};
		} catch (const std::invalid_argument& e) {
			return ArgTypeMatcher{{}, "int", "string"};
		} catch (const std::out_of_range& e) {
			return ArgTypeMatcher{{}, "int", "out_of_range"};
		}
	}

	ArgTypeMatcher stringToFloat(const std::string& s) {
		try {
			float res = std::stof(s);
			return ArgTypeMatcher{res, "float", "float"};
		} catch (const std::invalid_argument& e) {
			return ArgTypeMatcher{{}, "float", "string"};
		} catch (const std::out_of_range& e) {
			return ArgTypeMatcher{{}, "float", "out_of_range"};
		}
	}

	ArgTypeMatcher stringToDouble(const std::string& s) {
		try {
			double res = std::stod(s);
			return ArgTypeMatcher{res, "double", "double"};
		} catch (const std::invalid_argument& e) {
			return ArgTypeMatcher{{}, "double", "string"};
		} catch (const std::out_of_range& e) {
			return ArgTypeMatcher{{}, "double", "out_of_range"};
		}
	}

	ArgTypeMatcher stringToBool(const std::string& s) {
		std::string lower = s;
		std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
		if (lower == "true" || lower == "1") {
			return ArgTypeMatcher{true, "bool", "bool"};
		} else if (lower == "false" || lower == "0") {
			return ArgTypeMatcher{false, "bool", "bool"};
		} else {
			return ArgTypeMatcher{{}, "bool", "string"};
		}
	}

	ArgTypeMatcher stringToChar(const std::string& s) {
		if (s.size() == 1) {
			return ArgTypeMatcher{s[0], "char", "char"};
		} else {
			return ArgTypeMatcher{{}, "char", "string"};
		}
	}


	class ArgParser {

		struct ArgumentInfo {
			std::string value;
			bool positional;
			bool required;
			std::string help;
			ArgType type;
		};

		std::map<std::string, ArgumentInfo> arguments;

	public:
		ArgParser(const std::string name = "program"){
			// Automatically load help message
			program_name = name;
			addArgument("-h", false, false, "Display this help message", ArgType::BOOL);
			addArgument("--help", false, false, "Display this help message", ArgType::BOOL);
		}


		template<typename T>
			T getTypedArg(const std::string &name) {
				if (arguments.find(name) != arguments.end()) {
					const auto &matcher = checkArg(arguments[name]);
					if (!matcher.value.has_value()) {
						throw std::runtime_error("No value present for argument: " + name);
					}
					try {
						return std::any_cast<T>(matcher.value);
					} catch (const std::bad_any_cast& e) {
						throw std::runtime_error("Invalid type for argument: " + name);
					}
				} else {
					throw std::runtime_error("Argument not found: " + name);
				}
			}
	


		ArgTypeMatcher checkArg(ArgumentInfo argument) {
			ArgTypeMatcher result = ArgTypeMatcher{"", "", ""};
			switch (argument.type) {
				case ArgType::INT:
				result = stringToInt(argument.value);
				break;
				case ArgType::FLOAT:
				result = stringToFloat(argument.value);
				break;
				case ArgType::DOUBLE:
				result = stringToDouble(argument.value);
				break;
				case ArgType::BOOL:
				result = stringToBool(argument.value);
				break;
				case ArgType::CHAR:
				result = stringToChar(argument.value);
				break;
				case ArgType::STRING:
				result = ArgTypeMatcher{argument.value, "string", "string"};
			}

			return result;
		}


		void addArgument(const std::string& name, bool positional, bool required = false, const std::string& help = "", ArgType type = ArgType::STRING, std::string value = "") {
			arguments[name] = ArgumentInfo{value, positional, required, help, type};
			if (positional) {
				positional_args.push(name);
			}
		}

		void parse_args(int argc, const char *argv[]) {
			std::vector<std::string> args(argv + 1, argv + argc);
			ArgTypeMatcher checker;
			for (size_t i = 0; i < args.size(); ++i) {
				// Check for optional args
				if (args[i] == "-h" || args[i] == "--help") {
					displayHelp();
					exit(0);
				}

    				if (args[i][0] == '-') {
        				std::string key = args[i];
        				// Check if there is a next argument and it is not another option (does not start with '-')
        				// Also, ensure we check if the key exists in the map to avoid inserting new keys inadvertently
        				if (i + 1 < args.size() && args[i + 1][0] != '-' && arguments.find(key) != arguments.end()) {
            					// optional arg with value
						// Debugging
						// std::cout << "Key: " << key << " Value: " << args[i + 1] << std::endl;
						arguments[key] = ArgumentInfo{args[i + 1], false, arguments[key].required, arguments[key].help, arguments[key].type};
						checker = checkArg(arguments[key]);
						if (checker.expected != checker.actual) {
							std::cerr << "Invalid argument type. Expected '" << checker.expected << "' got '" << checker.actual << "'" << std::endl;
							exit(1);
						}
						i++; // Skip the next argument since it's consumed as the value for the current option
        				} else if (arguments.find(key) != arguments.end()) {
            					// optional arg without value, or next argument is another option
            					arguments[key] = ArgumentInfo{"true", false, arguments[key].required, arguments[key].help, arguments[key].type};
						checker = checkArg(arguments[key]);
						if (checker.expected != checker.actual) {
							std::cerr << "Invalid argument type. Expected '" << checker.expected << "' got '" << checker.actual << "'" <<std::endl;
						}
        				} 
    				} else if (!positional_args.empty()) {
        				// Assuming arg should be args[i] based on your previous loop
        				arguments[positional_args.front()] = ArgumentInfo{args[i], true, arguments[positional_args.front()].required, arguments[positional_args.front()].help, arguments[positional_args.front()].type};
        				checker = checkArg(arguments[positional_args.front()]);
					if (checker.expected != checker.actual) {
						std::cerr << "Invalid argument type. Expected '" << checker.expected << "' got '" << checker.actual << "'" << std::endl;
						exit(1);
					}
					positional_args.pop();
    				} else {
					std::cerr << "Invalid argument: " << args[i] << std::endl;
					exit(1);
    				}
			}


		}
	

		std::string generateHelpMessage() {
			std::stringstream ss;
			ss << "Usage: ./"  << program_name << " <flags>"<< std::endl;
			for (auto& arg : arguments) { 
				ss << arg.first << " :\t " << arg.second.help << std::endl;
			}
			return ss.str();
		}


	private:

		void displayHelp() {
			std::cout << generateHelpMessage() << std::endl;
			exit(0);
		}

		std::queue<std::string> positional_args;
		std::string program_name;
		
	};
}

#endif
