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
	cppargparser::ArgParser* parser;
	parser->addArgument("--test", false, "A test argument");
	std::string expected = "";
	ASSERT_EQ(parser->getArg("--test"), expected);
}

TEST_F(ArgParserTest, ParseMultipleOptionalArguments) {
	int argc = 5;
	const char* argv[] = {"./test", "--test", "test", "--test2", "test2"};
	parser->addArgument("--test", false, "A test argument");
	parser->addArgument("--test2", false, "A test argument");
	parser->parse_args(argc, argv);

	ASSERT_EQ(parser->getArg("--test"), "test");
	ASSERT_EQ(parser->getArg("--test2"), "test2");
}


