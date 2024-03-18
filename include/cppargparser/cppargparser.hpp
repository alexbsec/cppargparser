#ifndef CPPARGPARSER_HPP
#define CPPARGPARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

namespace cppargparser {
	class ArgParser {
	public:
		ArgParser() = default;
	
		void addArgument(const std::string& name, bool required = false, const std::string& help = "") {
			arguments[name] = "";
			required_args[name] = required;
			help_messages[name] = help;	
		}

		void parse_args(int argc, const char *argv[]) {
			std::vector<std::string> args(argv + 1, argv + argc);
			for (size_t i = 0; i < args.size(); ++i) {
				// Check for optional args
    				if (args[i][0] == '-') {
        				std::string key = args[i];
        				// Check if there is a next argument and it is not another option (does not start with '-')
        				// Also, ensure we check if the key exists in the map to avoid inserting new keys inadvertently
        				if (i + 1 < args.size() && args[i + 1][0] != '-' && arguments.find(key) != arguments.end()) {
            					// optional arg with value
            					arguments[key] = args[i + 1];
            					i++; // Skip the next argument since it's consumed as the value for the current option
        				} else if (arguments.find(key) != arguments.end()) {
            					// optional arg without value, or next argument is another option
            					arguments[key] = "true";
        				} 
    				} else if (!positional_args.empty()) {
        				// Assuming arg should be args[i] based on your previous loop
        				arguments[positional_args.front()] = args[i];
        				positional_args.pop();
    				}
			}

		}
	
		std::string getArg(const std::string& name) {
			if (arguments.find(name) != arguments.end()) {
				return arguments[name];
			} else {
				return "";
			}
		}

	private:
		std::map<std::string, std::string> arguments;
		std::map<std::string, bool> required_args;
		std::map<std::string, std::string> help_messages;
		std::queue<std::string> positional_args;
	};
}

#endif
