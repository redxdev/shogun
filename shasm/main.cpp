#include "SVM_Lexer.h"
#include "SVM_Parser.h"
#include "SVM_AsmWriter.h"
#include "ShogunVM.h"

#include <tclap/CmdLine.h>

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	Shogun::String inputFileName;
	Shogun::String outputFileName;

	try
	{
		TCLAP::CmdLine cmd("ShogunVM assembler", ' ', Shogun::version_string(), true);

		TCLAP::ValueArg<Shogun::String> inputArg("i", "input", "input file", true, "", "filename", cmd);
		TCLAP::ValueArg<Shogun::String> outputArg("o", "output", "output file", true, "", "filename", cmd);

		cmd.parse(argc, argv);

		inputFileName = inputArg.getValue();
		outputFileName = outputArg.getValue();
	}
	catch (TCLAP::ArgException e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Starting lexer..." << std::endl;

	std::ifstream inputFile(inputFileName.c_str(), std::ios::in | std::ios::binary);
	if (!inputFile.is_open())
	{
		std::cerr << "error: unable to open input file" << std::endl;
		return EXIT_FAILURE;
	}

	Shogun::Assembler::Lexer lexer;
	Shogun::Assembler::TokenStream tokens;

	try
	{
		tokens = lexer.tokenize(inputFile);
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while tokenizing stream" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		return EXIT_FAILURE;
	}

	inputFile.close();

	std::cout << "Lexer completed: " << tokens.size() << " tokens." << std::endl;
	std::cout << "Starting parser..." << std::endl;

	Shogun::Assembler::Parser parser;
	Shogun::Assembler::NodeList nodes;

	try
	{
		nodes = parser.parse(tokens);
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while parsing token stream" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Parser completed: " << nodes.size() << " parse nodes." << std::endl;
	std::cout << "Starting writer..." << std::endl;

	Shogun::Assembler::AsmWriter writer;
	std::ofstream outputFile(outputFileName.c_str(), std::ios::out | std::ios::ate | std::ios::binary);

	if (!outputFile.is_open())
	{
		std::cerr << "error: unable to open output file" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		writer.write(outputFile, nodes);
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while compiling node tree" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		return EXIT_FAILURE;
	}

	outputFile.close();

	std::cout << "Writer completed." << std::endl;
	std::cout << "shasm has finished." << std::endl;
	return EXIT_SUCCESS;
}