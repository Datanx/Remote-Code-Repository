///////////////////////////////////////////////////////////////////////
// RepositoryCore.cpp                                                //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef TEST_REPOSITORYCORE
#include "RepositoryCore.h"

int main() {
	NoSqlDb::DbElement<NoSqlDb::PayLoad> demoElem;
	NoSqlDb::DbCore<NoSqlDb::PayLoad> db;
	Repository::RepositoryCore repoCore(db);

	// build demoElem
	demoElem.name("owner");
	demoElem.descrip("descript");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(NoSqlDb::PayLoad("../Storage/"));
	demoElem.nameSpace("namespace");

	if (repoCore.checkIn("file.h", demoElem))
	{
		std::cout << "\n  Check-In success! ";
	}
	return 0;
}
#endif