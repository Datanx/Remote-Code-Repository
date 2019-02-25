///////////////////////////////////////////////////////////////////////
// Version.h - Manages version numbering                             //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef TEST_VERSION

#include "Version.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
using namespace NoSqlDb;

int main()
{
	Utilities::Title("Demonstrating Version Numbering");

	DbCore<PayLoad> db;
	DbElement<PayLoad> demoElem;
	std::vector<int> version;
	version.push_back(1);
	version.push_back(2);

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(PayLoad("../TestFolder/source/"));
	demoElem.version(version);
	db["Test1.h"] = demoElem;

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.payLoad(PayLoad("../TestFolder/source/"));
	db["Test1.cpp"] = demoElem;

	db["Test1.h"].children().push_back("Test1.cpp");

	Utilities::putline();
	std::cout << "\n  showing all the database elements:";
	Utilities::putline();
	showDb(db);
	
	Utilities::putline();
	std::cout << "\n  Convert \"Test1.h\"'s latest int version number to string version.";
	Utilities::putline();

	Version vers(db, "Test1.h");
	std::cout << "  \"Test1.h\"'s latest int version number is: " << db["Test1.h"].version().back();
	std::cout << "\n  Result: version number converted from " << db["Test1.h"].version().back() << " to " << vers.version().versnum() << std::endl;
	system("pause");
}
#endif