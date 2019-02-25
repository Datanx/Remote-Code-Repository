///////////////////////////////////////////////////////////////////////
// Process.cpp - class used to start process                         //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include "Process.h"
#include <iostream>

#ifdef TEST_PROCESS

int main()
{
	std::cout << "\n  Demonstrating code pop-up in notepad";
	std::cout << "\n ======================================";

	Process p;
	p.title("test application");
	std::string appPath = "c:/windows/system32/notepad.exe";
	p.application(appPath);

	std::string cmdLine = "/A ../Process/Process.h";
	p.commandLine(cmdLine);

	std::cout << "\n  starting process: \"" << appPath << "\"";
	std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
	p.create();

	CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
	p.setCallBackProcessing(callback);
	p.registerCallback();

	std::cout << "\n  after OnExit";
	std::cout.flush();
	char ch;
	std::cin.read(&ch, 1);
	return 0;
}

#endif
