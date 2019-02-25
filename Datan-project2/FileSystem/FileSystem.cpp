/////////////////////////////////////////////////////////////////////////////
// FileSystem.cpp - Support file and directory operations                  //
// ver 2.6                                                                 //
// ----------------------------------------------------------------------- //
// copyright ?Jim Fawcett, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <locale>
#include "FileSystem.h"

using namespace FileSystem;


//----< test stub >--------------------------------------------------------

#ifdef TEST_FILESYSTEM

void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}
int main(int argc, char* argv[])
{
	title("Demonstrate Path Class");

	std::string fs = Path::fileSpec(".", "temp.txt");
	std::cout << "\n  Path::fileSpec(\".\",\"temp.txt\") = " << fs;

	std::string path = Path::getPath(fs);
	std::cout << "\n  Path::getPath(\"" + fs + "\") = " << path;

	std::string ffs = Path::getFullFileSpec(fs);
	std::cout << "\n  Path::getFullFileSpec(\"" + fs + "\") = " << ffs;

	std::string name = Path::getName(fs);
	std::cout << "\n  Path::getName(\"" + fs + "\") = " << name;

	std::string ext = Path::getExt(fs);
	std::cout << "\n  Path::getExt(\"" + fs + "\") = " << ext;

	std::string upper = Path::toUpper("temp.txt");
	std::cout << "\n  Path::toUpper(\"temp.txt\") = " << upper;

	std::string lower = Path::toLower("Temp.Txt");
	std::cout << "\n  Path::toLower(\"Temp.Txt\") = " << lower;
	std::cout << std::endl;
	/*
	title("Demonstrate FileSystemSearch class");

	const size_t PathSetSize = 2;
	std::string home = ::getenv("HOMEDRIVE");
	std::string pathSet[] = { home+"\\\\", "." };
	FileSystemSearch fss;
	for(size_t i = 0; i<PathSetSize; ++i)
	{
	std::cout << "\n  searching for files on \"" << pathSet[i] << "\"";
	std::cout << "\n " << std::string(27 + pathSet[i].size(), '-');
	std::string searchPath = pathSet[i];
	std::string fileName = fss.firstFile(searchPath);
	if(fileName.size() > 0)
	std::cout << "\n  " << fileName;
	else
	std::cout << "\n  no files match search";
	while(true) {
	fileName = fss.nextFile();
	if(fileName.size() > 0)
	std::cout << "\n  " << fileName;
	else
	break;
	}
	std::cout << std::endl;

	std::cout << "\n  searching for directories on \"" << pathSet[i] << "\"";
	std::cout << "\n " << std::string(33 + pathSet[i].size(), '-');
	std::string dirName = fss.firstDirectory(searchPath);
	if(dirName.size() > 0)
	std::cout << "\n  " << dirName;
	else
	std::cout << "\n  no directories match search";
	while(true)
	{
	dirName = fss.nextDirectory();
	if(dirName.size() > 0)
	std::cout << "\n  " << dirName;
	else
	break;
	}
	std::cout << std::endl;
	}
	*/
	title("Demonstrate Directory class");

	// Display contents of current directory

	std::cout << "\n  current directory is:\n    " << Directory::getCurrentDirectory();
	std::cout << "\n  It contains files:";
	std::vector<std::string> currfiles = Directory::getFiles();
	///////////////////////////////////////////////////////
	// This works too
	// std::vector<std::string> currfiles = d.getFiles();  
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i].c_str();
	std::cout << "\n  and contains directories:";
	std::vector<std::string> currdirs = Directory::getDirectories();
	for (size_t i = 0; i<currdirs.size(); ++i)
		std::cout << "\n    " << currdirs[i].c_str();
	std::cout << "\n";

	// Display contents of non-current directory

	std::cout << "\n  .txt files residing in C:/temp are:";
	currfiles = Directory::getFiles("c:/temp/", "*.txt");

	// if we want fully qualified file names, we have to
	// set the current directory to the path on which the files
	// reside, if it isn't already so set

	std::string currDir = Directory::getCurrentDirectory();
	Directory::setCurrentDirectory("c:/temp/");
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << Path::getFullFileSpec(currfiles[i]).c_str();
	Directory::setCurrentDirectory(currDir);

	// we have to restore the current directory so the
	// remaining tests work

	// it's probably easier just to use Path::fileSpec(path, filename)
	// like this:

	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << Path::fileSpec("c:\\temp\\", currfiles[i]).c_str();

	std::cout << "\n";
	std::cout << "\n  directories residing in C:/temp are:";
	currdirs = Directory::getDirectories("c:/temp/");
	for (size_t i = 0; i<currdirs.size(); ++i)
		std::cout << "\n    " << currdirs[i].c_str();
	std::cout << "\n";

	// Create directory

	title("Demonstrate FileInfo Class Operations", '=');
	std::cout << "\n";

	Directory::setCurrentDirectory(".");
	std::cout << "\n  current path is \"" << Directory::getCurrentDirectory();

	std::string fn1;
	if (argc > 1)
		fn1 = argv[1];
	else
		fn1 = "c:\\temp\\test.txt";
	FileInfo fi(fn1);

	if (fi.good())
	{
		std::cout << "\n  name: " << "\t" << fi.name();
		std::cout << "\n  date: " << "\t" << fi.date();
		std::cout << "\n  date: " << "\t" << fi.date(FileInfo::dateformat);
		std::cout << "\n  date: " << "\t" << fi.date(FileInfo::timeformat);
		std::cout << "\n  size: " << "\t" << fi.size() << " bytes";
		if (fi.isArchive())
			std::cout << "\n  is archive";
		else
			std::cout << "\n  is not archive";
		if (fi.isCompressed())
			std::cout << "\n  is compressed";
		else
			std::cout << "\n  is not compressed";
		if (fi.isDirectory())
			std::cout << "\n  is directory";
		else
			std::cout << "\n  is not directory";
		if (fi.isEncrypted())
			std::cout << "\n  is encrypted";
		else
			std::cout << "\n  is not encrypted";
		if (fi.isHidden())
			std::cout << "\n  is hidden";
		else
			std::cout << "\n  is not hidden";
		if (fi.isNormal())
			std::cout << "\n  is normal";
		else
			std::cout << "\n  is not normal";
		if (fi.isOffLine())
			std::cout << "\n  is offline";
		else
			std::cout << "\n  is not offline";
		if (fi.isReadOnly())
			std::cout << "\n  is readonly";
		else
			std::cout << "\n  is not readonly";
		if (fi.isSystem())
			std::cout << "\n  is system";
		else
			std::cout << "\n  is not system";
		if (fi.isTemporary())
			std::cout << "\n  is temporary";
		else
			std::cout << "\n  is not temporary";
	}
	else
		std::cout << "\n  filename " << fn1 << " is not valid in this context\n";

	std::string fn2;
	if (argc > 2)
	{
		fn1 = argv[1];
		fn2 = argv[2];
	}
	else
	{
		fn1 = "FileSystem.h";
		fn2 = "FileSystem.cpp";
	}
	FileInfo fi1(fn1);
	FileInfo fi2(fn2);
	if (fi1.good() && fi2.good())
	{
		if (fi1 == fi1)
			std::cout << "\n  " << fi1.name() << " == " << fi1.name();
		else
			std::cout << "\n  " << fi1.name() << " != " << fi1.name();
		if (fi1 < fi1)
			std::cout << "\n  " << fi1.name() << " < " << fi1.name();
		else
			std::cout << "\n  " << fi1.name() << " >= " << fi1.name();
		if (fi1 == fi2)
			std::cout << "\n  " << fi1.name() << " == " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " != " << fi2.name();
		if (fi1 < fi2)
			std::cout << "\n  " << fi1.name() << " < " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " >= " << fi2.name();
		if (fi1.smaller(fi2))
			std::cout << "\n  " << fi1.name() << " is smaller than " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " is not smaller than " << fi2.name();
		if (fi1.earlier(fi2))
			std::cout << "\n  " << fi1.name() << " is earlier than " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " is not earlier than " << fi2.name();
		std::cout << std::endl;
	}
	else
		std::cout << "\n  files " << fn1 << " and " << fn2 << " are not valid in this context\n";

	title("Demonstrate File class operations", '=');
	std::cout << "\n";

	// copy binary file from one directory to another

	File me("../debug/filesystemdemo.exe");
	me.open(File::in, File::binary);
	std::cout << "\n  copying " << me.name().c_str() << " to c:/temp";
	if (!me.isGood())
	{
		std::cout << "\n  can't open executable\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << Path::getFullFileSpec(me.name()) << "\n";
	}
	else
	{
		File you("c:/temp/fileSystemdemo.exe");
		you.open(File::out, File::binary);
		if (you.isGood())
		{
			while (me.isGood())
			{
				static size_t count = 0;
				Block b = me.getBlock(1024);
				you.putBlock(b);
				if (++count < 10)
				{
					std::cout << "\n    reading block of " << b.size() << " bytes";
					std::cout << "\n    writing block of " << b.size() << " bytes";
				}
				if (b.size() < 1024)
				{
					std::cout << "\n\n    omitted " << count - 10 << " blocks from display\n\n";
					std::cout << "\n    reading block of " << b.size() << " bytes";
					std::cout << "\n    writing block of " << b.size() << " bytes";
				}
			}
			std::cout << "\n";
		}
	}

	// save some filespecs of text files in a vector for File demonstrations

	std::vector<std::string> files;
	if (argc == 1)
	{
		std::cout << "\n\n  Enter, on the command line, an additional filename to process.\n";
	}

	for (int i = 1; i<argc; ++i)
	{
		files.push_back(argv[i]);
	}
	files.push_back("FileSystem.cpp");                     // file not on current path
	files.push_back("../FileSystemDemo/FileSystem.cpp");   // file from project directory
	files.push_back("../FileSystemTest.txt");              // file in solution directory
	files.push_back("foobar");                             // doesn't exist

														   // open each file and display a few lines of text

	for (size_t i = 0; i<files.size(); ++i)
	{
		File file(files[i]);
		file.open(File::in);
		if (!file.isGood())
		{
			std::cout << "\n  Can't open file " << file.name();
			std::cout << "\n  Here's what the program can't find:\n  " << Path::getFullFileSpec(file.name());
			continue;
		}
		std::string temp = std::string("Processing file ") + files[i];
		title(temp, '-');
		for (int j = 0; j<10; ++j)
		{
			if (!file.isGood())
				break;
			std::cout << "\n  -- " << file.getLine().c_str();
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	// read all lines of text file into string

	title("testing File::readAll()", '-');
	std::cout << "\n";
	File testAll("../FileSystemTest.txt");
	testAll.open(File::in);
	if (testAll.isGood())
	{
		std::string all = testAll.readAll();
		std::cout << all << "\n";
	}
	testAll.close();

	title("testing File::readAll(true)", '-');
	std::cout << "\n";
	File testAllTrue("../FileSystemTest.txt");
	testAllTrue.open(File::in);
	if (testAllTrue.isGood())
	{
		std::string all = testAllTrue.readAll(true);
		std::cout << all << "\n";
	}
	testAllTrue.close();

	// test reading non-text files

	title("test reading non-text files", '-');
	std::cout << "\n";
	std::string testPath = "./debug";  // run from project directory (what Visual Studio does)
	if (!Directory::exists(testPath))
		testPath = ".";                  // run from solution debug directory
	std::vector<std::string> testFiles = Directory::getFiles(testPath);
	for (auto file : testFiles)
	{
		try
		{
			std::string ext = Path::getExt(file);
			if (ext == "exe" || ext == "dll" || file == "run.dat")
			{
				/* reading binary file works, but generates a lot of garbage */
				/* I use run.dat to capture this output so including will make output confusing */

				std::cout << "  skipping binary file " << file << "\n";
				continue;
			}
			File test(file);
			test.open(File::in);  // open as text file
			std::cout << "\n  processing \"" << file << "\"\n";
			if (test.isGood())
			{
				std::string text = test.readAll(true);
				std::cout << text << "\n";
			}
			else
			{
				std::cout << "\n  open failed\n";
			}
		}
		catch (std::exception& ex)
		{
			std::cout << "\n  Exception: " << ex.what() << "\n";
		}
	}

	// read text file and write to another text file

	title("writing to c:/temp/test.txt", '-');
	File in("../FileSystemDemo/FileSystem.h");
	in.open(File::in, File::text);
	File out("c:/temp/test.txt");
	out.open(File::out, File::text);
	while (in.isGood())
	{
		std::string temp = in.getLine();
		//std::cout << "\n  " << temp.c_str();
		out.putLine(temp);
		out.putLine("\n");
	}
	std::cout << "\n  check c:/temp/test.txt to validate";
	std::cout << "\n\n";

	// read and write buffers

	title("reading and writing buffers");
	std::cout << "\n  " << FileSystem::Directory::getCurrentDirectory();
	std::string fileIn = "../TestFileSystem/UnitTest.h";
	std::string fileOut = "../TestFileSystem/CopyOfUnitTest.h";
	File bufferIn(fileIn);
	bufferIn.open(File::in, File::binary);
	if (!bufferIn.isGood())
	{
		std::cout << "\n  could not open \"" << fileIn << "\" for reading";
		return 1;
	}
	else
	{
		std::cout << "\n  opening: \"" << fileIn << "\" for reading";
	}
	File bufferOut(fileOut);
	bufferOut.open(File::out, File::binary);
	if (!bufferOut.isGood())
	{
		std::cout << "\n  could not open \"" << fileOut << "\" for writing\n\n";
		return 1;
	}
	else
	{
		std::cout << "\n  opening: \"" << fileOut << "\" for writing";
	}
	std::cout << "\n";
	const size_t bufLen = 124;
	File::byte buffer[bufLen];
	while (true)
	{
		size_t resultSize = bufferIn.getBuffer(bufLen, buffer);
		std::string temp1(buffer, resultSize);
		std::cout << temp1;
		bufferOut.putBuffer(resultSize, buffer);
		if (resultSize < bufLen || !bufferIn.isGood() || !bufferOut.isGood())
		{
			bufferIn.close();
			bufferOut.close();
			break;
		}
	}
	std::cout << "\n\n";
}
#endif
