///////////////////////////////////////////////////////////////////////
// CheckIn.h - Check in package files                                //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef TEST_CHECKIN

#include "CheckIn.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
using namespace NoSqlDb;

int main()
{
	Utilities::Title("Demonstrating Check In Files");

	DbCore<PayLoad> db;
	DbElement<PayLoad> demoElem;

	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(PayLoad("../TestFolder/src/"));
	demoElem.children().push_back("GraphWalk.cpp");

	std::cout << "\n  Check in the first version of \"GraphWalk.h\"." << std::endl;
	CheckIn ck1(db, "GraphWalk.h");
	ck1.elem(demoElem);
	ck1.storepath("../TestFolder/repo/");
	ck1.checkin();
	db = ck1.db();
	std::cout << "\n";
	std::cout << "\n  Close file \"GraphWalk.h\" by the above notification." << std::endl;
	PayLoad p = db["GraphWalk.h"].payLoad();
	p.status(false);
	db["GraphWalk.h"].payLoad(p);

	std::cout << "\n  Check in a new version \"GraphWalk.h\"" << std::endl;
	CheckIn ck2(db, "GraphWalk.h");
	ck2.elem(demoElem);
	ck2.storepath("../TestFolder/repo/");
	ck2.checkin();
	db = ck2.db();

	Utilities::putline();
	std::cout << "\n  showing all the database elements:";
	Utilities::putline();
	showDb(db);
	Utilities::putline();

	std::cout << "\n  database keys are: ";
	Utilities::putline();
	showKeys(db);

	Utilities::putline();

	std::cout << "\n  Check in the first version of \"GraphWalk.cpp\"." << std::endl;
	demoElem.name("Datan");
	demoElem.descrip("Graph traversal");
	demoElem.payLoad(PayLoad("../TestFolder/src/"));
	demoElem.children().pop_back();
	CheckIn ck3(db, "GraphWalk.cpp");
	ck3.elem(demoElem);
	ck3.storepath("../TestFolder/repo/");
	ck3.checkin();
	db = ck3.db();

	Utilities::putline();
	std::cout << "\n  showing all the database elements:";
	Utilities::putline();
	showDb(db);
	Utilities::putline();

	std::cout << "\n  database keys are: ";
	Utilities::putline();
	showKeys(db);

	Utilities::putline();

	system("pause");
}
#endif