#include <tclap/CmdLine.h>

#include <SVM_AsmWriter.h>
#include <SVM_VirtualMachine.h>
#include <SVM_LibConsole.h>
#include <ShogunVM.h>

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	Shogun::String inputFileName;
	Shogun::Bool dumpOnRun = false;
	Shogun::Bool dumpOnExit = false;

	try
	{
		TCLAP::CmdLine cmd("ShogunVM assembler", ' ', Shogun::version_string(), true);

		TCLAP::UnlabeledValueArg<Shogun::String> inputArg("input", "input file", true, "", "filename", cmd, false);
		TCLAP::SwitchArg dumpOnRunArg("d", "dump-on-run", "Output a VM dump before running the program", cmd, false);
		TCLAP::SwitchArg dumpOnExitArg("D", "dump-on-exit", "Output a VM dump when exiting the program", cmd, false);

		cmd.parse(argc, argv);

		inputFileName = inputArg.getValue();
		dumpOnRun = dumpOnRunArg.getValue();
		dumpOnExit = dumpOnExitArg.getValue();
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

	Shogun::Library::Console::register_library(&vm);

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

	if (dumpOnRun)
	{
		vm.dump(std::cout);
	}

	try
	{
		vm.run();
	}
	catch (Shogun::Exception& e)
	{
		std::cerr << "error: caught exception while running program" << std::endl;
		std::cerr << e.getMessage() << std::endl;
		vm.dump(std::cerr);
		return EXIT_FAILURE;
	}

	if (dumpOnExit)
	{
		vm.dump(std::cout);
	}

	return EXIT_SUCCESS;
}