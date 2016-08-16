#include <cstdlib>
#include <iostream>
#include <string>
#include "os.h"
#include "compiler.h"

using namespace std;

struct parsedCli {
	string mainFile;
	string outFile;
	bool isLibrary; // Executable or library?
	bool includeDebugSymbols;
};

parsedCli cli(int, char* []);

int main(int argc, char* argv[]) {
	parsedCli args = cli(argc, argv);

	compileFile(args.mainFile);

	return 0;
}

parsedCli cli(int argc, char* argv[]) {
	parsedCli args;
	args.isLibrary = false;
	args.includeDebugSymbols = false;

	char* mainFile = NULL;

	for (int i = 1; i < argc; i++) {
		if (!mainFile) {
			mainFile = argv[i];
		} else {
			printf("Please specify only one main file\n");
			exit(1);
		}
	}

	if (!mainFile) {
		printf("Please specify a main file\n");
		exit(1);
	}

	args.mainFile = mainFile;

	// decide output filename
#ifdef OS_WIN
	int index = args.mainFile.rfind("\\");
#else
	int index = args.mainFile.rfind("/");
#endif

	args.outFile = index < 0 ? args.mainFile : args.mainFile.substr(index + 1, args.mainFile.length() - index - 1);
	index = args.outFile.rfind(".");
	args.outFile = index < 0 ? args.outFile + '_' : args.outFile.substr(0, index);

#ifdef OS_WIN
	args.outFile += ".exe";
#endif

	return args;
}
