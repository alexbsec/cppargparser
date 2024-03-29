#include <gtest/gtest.h>
#include "../include/cppargparser/cppargparser.hpp"
#include <sstream>

class ArgParserTest : public ::testing::Test {
protected:
    void SetUp() override {
	parser = new cppargparser::ArgParser();
    }

    void TearDown() override {
	delete parser;
    }

    cppargparser::ArgParser* parser;
};


TEST_F(ArgParserTest, TestAddArgument) {
    parser->addArgument("--test", false, false, "A test argument", cppargparser::ArgType::STRING, "value");
    std::string expected = "value";
    std::string actual = parser->getTypedArg<std::string>("--test");
    ASSERT_EQ(actual, expected);
}


TEST_F(ArgParserTest, ParseMultipleOptionalArguments) {
	int argc = 5;
	const char* argv[] = {"./test", "--test", "test", "--test2", "test2"};
	parser->addArgument("--test", false, false, "test", cppargparser::ArgType::STRING);
	parser->addArgument("--test2", false, false, "test2", cppargparser::ArgType::STRING);
	parser->parse_args(argc, argv);
	
	std::string actual1 = parser->getTypedArg<std::string>("--test");
	std::string actual2 = parser->getTypedArg<std::string>("--test2");
	ASSERT_EQ(actual1, "test");
	ASSERT_EQ(actual2, "test2");
}

// test help message

TEST_F(ArgParserTest, GenerateHelpMessage) {
    // Setup
    parser->addArgument("--test", false, false, "This is a test argument", cppargparser::ArgType::STRING);
    parser->addArgument("--option", false, true, "This is a required option", cppargparser::ArgType::STRING);

    // Execute
    std::string helpMessage = parser->generateHelpMessage();

    // Verify
    std::string expected = "Usage: ./program <flags>\n--help :\t Display this help message\n"
                       "--option :\t This is a required option\n"
                       "--test :\t This is a test argument\n"
                       "-h :\t Display this help message\n";
    ASSERT_EQ(helpMessage, expected);

}

TEST_F(ArgParserTest, ValidateIntegerArgument) {
    int argc = 3;
    const char* argv[] = {"./test", "--number", "42"};
    parser->addArgument("--number", false, false, "A number", cppargparser::ArgType::INT);
    parser->parse_args(argc, argv);

    auto matcher = parser->checkArg({"42", false, false, "A number", cppargparser::ArgType::INT});
    ASSERT_EQ(std::any_cast<int>(matcher.value), 42);
    ASSERT_EQ(matcher.expected, "int");
    ASSERT_EQ(matcher.actual, "int");
}

TEST_F(ArgParserTest, ValidateFloatArgument) {
    int argc = 3;
    const char* argv[] = {"./test", "--number", "42.5"};
    parser->addArgument("--number", false, false, "A number", cppargparser::ArgType::FLOAT);
    parser->parse_args(argc, argv);

    auto matcher = parser->checkArg({"42.5", false, false, "A number", cppargparser::ArgType::FLOAT});
    ASSERT_FLOAT_EQ(std::any_cast<float>(matcher.value), 42.5f);
    ASSERT_EQ(matcher.expected, "float");
    ASSERT_EQ(matcher.actual, "float");
}


TEST_F(ArgParserTest, ValidateBooleanArgumentTrue) {
    int argc = 3;
    const char* argv[] = {"./test", "--flag", "true"};
    parser->addArgument("--flag", false, false, "A flag", cppargparser::ArgType::BOOL);
    parser->parse_args(argc, argv);

    auto matcher = parser->checkArg({"true", false, false, "A flag", cppargparser::ArgType::BOOL});
    ASSERT_EQ(std::any_cast<bool>(matcher.value), true);
}

TEST_F(ArgParserTest, ValidateBooleanArgumentFalse) {
    int argc = 3;
    const char* argv[] = {"./test", "--flag", "false"};
    parser->addArgument("--flag", false, false, "A flag", cppargparser::ArgType::BOOL);
    parser->parse_args(argc, argv);

    auto matcher = parser->checkArg({"0", false, false, "A flag", cppargparser::ArgType::BOOL});
    ASSERT_EQ(std::any_cast<bool>(matcher.value), false);
}

TEST_F(ArgParserTest, ValidateStringArgument) {
    int argc = 3;
    const char* argv[] = {"./test", "--name", "John"};
    parser->addArgument("--name", false, false, "A name", cppargparser::ArgType::STRING);
    parser->parse_args(argc, argv);

    auto matcher = parser->checkArg({"John", false, false, "A name", cppargparser::ArgType::STRING});
    ASSERT_EQ(std::any_cast<std::string>(matcher.value), "John");
}

// test char

TEST_F(ArgParserTest, ValidateCharArgument) {
    int argc = 3;
    const char* argv[] = {"./test", "--char", "a"};
    parser->addArgument("--char", false, false, "A char", cppargparser::ArgType::CHAR);
    parser->parse_args(argc, argv);

    auto matcher = parser->checkArg({"a", false, false, "A char", cppargparser::ArgType::CHAR});
    ASSERT_EQ(std::any_cast<char>(matcher.value), 'a');
}


TEST_F(ArgParserTest, ValidateGetTypedArg) {
	int argc = 4;
	const char* argv[] = {"./test", "--test", "12", "--test2"};
	parser->addArgument("--test", false, false, "test", cppargparser::ArgType::INT);
	parser->addArgument("--test2", false, false, "test2", cppargparser::ArgType::BOOL, "false");
	parser->parse_args(argc, argv);

	int actual = parser->getTypedArg<int>("--test");
	bool actual2 = parser->getTypedArg<bool>("--test2");

	ASSERT_EQ(actual, 12);
	ASSERT_EQ(actual2, true);

}

TEST_F(ArgParserTest, ValidatePositionalArg) {
	int argc = 2;
	const char* argv[] = {"./test", "12"};
	parser->addArgument("number", true, false, "A number", cppargparser::ArgType::INT);	
	parser->parse_args(argc, argv);

	int actual = parser->getTypedArg<int>("number");
	ASSERT_EQ(actual, 12);

}

TEST_F(ArgParserTest, ValidatePositionalArgWithOptionalArg) {
	int argc = 4;
	const char* argv[] = {"./test", "12", "--test", "test"};
	parser->addArgument("number", true, false, "A number", cppargparser::ArgType::INT);
	parser->addArgument("--test", false, false, "test", cppargparser::ArgType::STRING);
	parser->parse_args(argc, argv);

	int actual = parser->getTypedArg<int>("number");
	std::string actual2 = parser->getTypedArg<std::string>("--test");

	ASSERT_EQ(actual, 12);
	ASSERT_EQ(actual2, "test");

}
