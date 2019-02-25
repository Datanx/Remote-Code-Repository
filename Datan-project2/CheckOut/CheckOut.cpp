///////////////////////////////////////////////////////////////////////
// CheckOut.h - Check out package files                              //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef TEST_CHECKOUT

#include "CheckOut.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
using namespace NoSqlDb;

int main()
{
	Utilities::Title("Demonstrating Check Out Files");

	DbCore<PayLoad> db;
	DbElement<PayLoad> demoElem;

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(PayLoad("../src/"));
	db["GraphWalk.h"] = demoElem;

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.payLoad(PayLoad("../src/"));
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

	CheckOut co(db, "GraphWalk.h", ".1");
	co.storepath("../TestFolder/repo/");
	co.specpath("../TestFolder/spec/");
	co.checkout();

	std::cout << "\n";

	system("pause");
}
#endif