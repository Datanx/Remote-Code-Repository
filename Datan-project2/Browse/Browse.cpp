///////////////////////////////////////////////////////////////////////
// Browse.h - Browse package files                                   //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef TEST_BROWSE

#include "Browse.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
using namespace NoSqlDb;

int main()
{
	Utilities::Title("Demonstrating Files Browse");

	DbCore<PayLoad> db;
	DbElement<PayLoad> demoElem;

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(PayLoad("../TestFolder/src/"));
	db["GraphWalk.h"] = demoElem;

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.payLoad(PayLoad("../TestFolder/src/"));
	db["GraphWalk.cpp"] = demoElem;

	db["GraphWalk.h"].children().push_back("GraphWalk.cpp");

	Utilities::putline();
	std::cout << "\n  showing all the database elements:";
	Utilities::putline();
	showDb(db);
	Utilities::putline();

	std::cout << "\n  database keys are: ";
	Utilities::putline();
	showKeys(db);

	Utilities::putline();

	Browse browsefile(db, "GraphWalk.h", ".1");
	browsefile.storepath("../TestFolder/repo/");
	browsefile.browse();
	system("pause");
}
#endif