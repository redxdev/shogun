#include <tclap/CmdLine.h>

#include <SVM_AsmWriter.h>
#include <SVM_VirtualMachine.h>
#include <ShogunVM.h>

#include <iostream>
#include <fstream>

void print(Shogun::VirtualMachine* vm)
{
	std::cout << vm->pop()->getString() << std::endl;
}

int main(int argc, char** argv)
{
	Shogun::String inputFileName;

	try
	{
		TCLAP::CmdLine cmd("ShogunVM assembler", ' ', Shogun::version_string(), true);

		TCLAP::UnlabeledValueArg<Shogun::String> inputArg("input", "input file", true, "", "filename", cmd, false);

		cmd.parse(argc, argv);

		inputFileName = inputArg.getValue();
	}
	catch (TCLAP::ArgException e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream inputFile(inputFileName.c_str(), std::ios::in | std::ios::binary);
	if (!inputFile.is_open())
	{
		std::cerr << "error: unable to open input file" << std::endl;
		return EXIT_FAILURE;
	}

	Shogun::Assembler::AsmReader reader;
	Shogun::Assembler::CompileList compile;
	try
	{
		compile = reader.read(inputFile);
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while reading assembly" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		return EXIT_FAILURE;
	}

	Shogun::Program program;
	program.insert(program.begin(), compile.begin(), compile.end());

	Shogun::VirtualMachine vm(0);
	vm.registerCallable("print", &print);

	try
	{
		vm.loadProgram(program);
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while loading program" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		vm.run();
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while running program" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}