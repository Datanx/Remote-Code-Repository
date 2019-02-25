///////////////////////////////////////////////////////////////////////
// TestClass.cpp - defines all testing for NoSqlDb                   //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include "TestClass.h"
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Persist/Persist.h"
#include "../Edit/Edit.h"
#include "../Executive/Executive.h"
#include "../Browse/Browse.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
  Utilities::putline(n, out);
};

//----< R1 >-----------------------------------------------------------
/*
*  - Demonstrate that solution uses C++11
*/
bool TestDb::testR1()
{
  Utilities::title("Demonstrating Requirement #1");
  std::cout << "\n  " << typeid(std::function<bool()>).name()
    << ", declared in this function, "
    << "\n  is only valid for C++11 and later versions.";
  return true; // would not compile unless C++11
}
//----< R2 >-----------------------------------------------------------
/*
*  - Cite use of streams and operators new and delete
*/
bool TestDb::testR2()
{
  Utilities::title("Demonstrating Requirement #2");
  std::cout << "\n  A visual examination of all the submitted code "
    << "will show only\n  use of streams and operators new and delete.";
  return true;
}
//----< R3a >----------------------------------------------------------
/*
*  - Demonstrate check in the first version of a file
*/
bool TestDb::testR3a()
{
  Utilities::title("Demonstrating Requirement #3a - checking in the first version of a file");

  std::cout << "\n  Store the files in the designated location: ../TestFolder/repo/" << std::endl;

  // create a demo element

  DbElement<PayLoad> demoElem;

  demoElem.name("Datan");
  demoElem.descrip("Graph traversal");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad(PayLoad("../TestFolder/src/"));
  demoElem.children().push_back("GraphWalk.cpp");

  std::cout << "\n  Check in the first version of \"GraphWalk.h\"." << std::endl;
  CheckIn ck1(db_, "GraphWalk.h");
  ck1.elem(demoElem);
  ck1.storepath("../TestFolder/repo/");
  ck1.checkin();
  db_ = ck1.db();

  Utilities::putline();
  std::cout << "\n  showing all the database elements:";
  Utilities::putline();
  NoSqlDb::showDb(db_);
  Utilities::putline();

  std::cout << "\n  database keys are: ";
  Utilities::putline();
  NoSqlDb::showKeys(db_);

  std::cout << "\n\n  Please check the result in \"../TestFolder/repo/\".";
  return true;
}
//----< R3b >----------------------------------------------------------
/*
*  - Demonstrate check in the second version of a file and, the file is open
*/

bool TestDb::testR3b()
{
  Utilities::title("Demonstrating Requirement #3b - checking in the second version of a file and, the file is open");

  DbElement<PayLoad> demoElem;

  demoElem.name("Datan");
  demoElem.descrip("Graph traversal");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad(PayLoad("../TestFolder/src/"));
  demoElem.children().push_back("GraphWalk.cpp");

  std::cout << "\n  Check in the second version of \"GraphWalk.h\"." << std::endl;
  CheckIn ck2(db_, "GraphWalk.h");
  ck2.elem(demoElem);
  ck2.storepath("../TestFolder/repo/");
  ck2.checkin();
  db_ = ck2.db();

  Utilities::putline();
  std::cout << "\n  showing all the database elements:";
  Utilities::putline();
  NoSqlDb::showDb(db_);
  Utilities::putline();

  std::cout << "\n  database keys are: ";
  Utilities::putline();
  NoSqlDb::showKeys(db_);

  std::cout << "\n\n  Please check the result in \"../TestFolder/repo/\". The old version has been overrided. ";

  return true;
}
//----< R3c >-----------------------------------------------------------
/*
*  - Demonstrate check in the second version of a file but, the file is closed
*/
bool TestDb::testR3c()
{
  Utilities::title("Demonstrating Requirement #3c - checking in the second version of a file but, the file is closed");

  DbElement<PayLoad> demoElem;

  demoElem.name("Datan");
  demoElem.descrip("Graph traversal");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad(PayLoad("../TestFolder/src/"));
  demoElem.children().push_back("GraphWalk.cpp");

  std::cout << "\n  Close file \"GraphWalk.h\" in terms of the above notification, i.e. \"It's okay to close the file.\"" << std::endl;
  PayLoad p = db_["GraphWalk.h"].payLoad();
  p.status(false);
  db_["GraphWalk.h"].payLoad(p);

  std::cout << "\n  Check in a new version \"GraphWalk.h\"" << std::endl;
  CheckIn ck3(db_, "GraphWalk.h");
  ck3.elem(demoElem);
  ck3.storepath("../TestFolder/repo/");
  ck3.checkin();
  db_ = ck3.db();

  Utilities::putline();
  std::cout << "\n  showing all the database elements:";
  Utilities::putline();
  NoSqlDb::showDb(db_);
  Utilities::putline();

  std::cout << "\n  database keys are: ";
  Utilities::putline();
  NoSqlDb::showKeys(db_);

  std::cout << "\n\n  Please check the result in \"../TestFolder/repo/\". The second version has been created.";
  
  return true;
}
//----< R4 >----------------------------------------------------------
/*
*  - Demonstrate browsing of one or more packages by displaying package descriptions.
*/

bool TestDb::testR4()
{
  Utilities::title("Demonstrating Requirement #4 - demonstrating Files Browse");

  DbElement<PayLoad> demoElem;

  std::cout << "\n  Check in the first and second version of \"GraphWalk.cpp\" with closed status." << std::endl;
  demoElem.name("Datan");
  demoElem.descrip("Graph traversal");
  demoElem.payLoad(PayLoad("../TestFolder/src/"));

  CheckIn ck4(db_, "GraphWalk.cpp");
  ck4.elem(demoElem);
  ck4.storepath("../TestFolder/repo/");
  ck4.checkin();
  db_ = ck4.db();

  //close "GraphWalk.cpp"
  PayLoad p = db_["GraphWalk.cpp"].payLoad();
  p.status(false);
  db_["GraphWalk.cpp"].payLoad(p);

  CheckIn ck5(db_, "GraphWalk.cpp");
  ck5.elem(demoElem);
  ck5.storepath("../TestFolder/repo/");
  ck5.checkin();
  db_ = ck5.db();

  Utilities::putline();
  std::cout << "\n  showing all the database elements:";
  Utilities::putline();
  NoSqlDb::showDb(db_);
  Utilities::putline();

  std::cout << "\n  database keys are: ";
  Utilities::putline();
  NoSqlDb::showKeys(db_);

  std::cout << "\n\n  Browse \"GraphWalk.h.2\".";
  Browse browsefile(db_, "GraphWalk.h", ".2");
  browsefile.storepath("../TestFolder/repo/");
  browsefile.browse();

  
  return true;
}
//----< R5 >----------------------------------------------------------
/*
*  - Demonstrate check out files
*/
bool TestDb::testR5()
{
  Utilities::title("Demonstrating Requirement #5 - checking out files");

  std::cout << "\n Check out \"GraphWalk.h.2\" and, the filse belonging to the same package should be checked out automatically too.";
  CheckOut co(db_, "GraphWalk.h", ".2");
  co.storepath("../TestFolder/repo/");
  co.specpath("../TestFolder/spec/");
  co.checkout();

  std::cout << "\n\n  Please check the result in \"../TestFolder/spec/\". The package files have been checked out there.";
  return true;
}

//----< implement requirements testing >-------------------------------
#ifdef TEST_TESTCLASS
int main()
{
  std::cout << "\n  Software Repository Testbed ";
  std::cout << "\n =========================";

  NoSqlDb::DbCore<PayLoad> db;
  TestDb tdb(db);
  tdb.invoke(&TestDb::testR1);
  tdb.invoke(&TestDb::testR2);
  tdb.invoke(&TestDb::testR3a);
  tdb.invoke(&TestDb::testR3b);
  tdb.invoke(&TestDb::testR3c);
  tdb.invoke(&TestDb::testR4);
  tdb.invoke(&TestDb::testR5);
  
  std::cout << "\n\n";
  system("pause");
  return 0;
}
#endif