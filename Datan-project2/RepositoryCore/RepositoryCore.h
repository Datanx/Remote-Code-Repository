#ifndef REPOSITORY_H
#define REPOSITORY_H
/////////////////////////////////////////////////////////////////////
// RepositoryCore.h   - Check in, check out, browse, version       //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2017                            //
// Platform:    Macbook pro, Windows 10 Home                       //
// Application: Spring 2018 CSE687 Project#2                       //
// Author:      Datan Xu, dxu103@syr.edu                           //
// Referrence:  Jim Fawcett, CSE687                                //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* -This package provides all of the functions needed to check in,
*  check out, version and browse.
*
* Required Files:
* ---------------
* NoSqlDb.h
* Version.h
* PayLoad.h
* CheckIn.h
* Browse.h
* CheckOut.h
*
* Maintenance History:
* --------------------
* ver 2.0 : 17 April 2018
*/

#include "../Executive/NoSqlDb.h"
#include "../Version/Version.h"
#include "../PayLoad/PayLoad.h"
#include "../CheckIn/CheckIn.h"
#include "../Browse/Browse.h" 
#include "../CheckOut/CheckOut.h"
#include "../Query/Query.h"

namespace Repository {
	using namespace NoSqlDb;

	class RepositoryCore {
	public:
		RepositoryCore(DbCore<PayLoad>& db) : db_(db){}
		bool checkIn(const std::string& filename, const DbElement<PayLoad>& demoElem);
		bool checkOut(const std::string& filename, const std::string& num);
		bool close(const std::string& filename);
		

		DbCore<PayLoad>& dbCore() { return db_; }
		DbCore<PayLoad> dbCore() const { return db_; }
		void dbCore(const DbCore<PayLoad>& dbCore) { db_ = dbCore; }

		bool& flag() { return flag_; }
		bool flag() const { return flag_; }
		void flag(const bool& flag) { flag_ = flag; }

	private:
		DbCore<PayLoad> db_;
		bool flag_ = false;
	};
	//----< check in one file at a time >-----------------------------------------------

	inline bool RepositoryCore::checkIn(const std::string& filename, const DbElement<PayLoad>& demoElem) {
		CheckIn checkin_(db_, filename);
		checkin_.elem(demoElem);
		checkin_.storepath("../Storage/");
		
		if (checkin_.checkin()) {

			flag_ = checkin_.flag;
			db_ = checkin_.db();
			std::cout << "  check-in success!\n";
			return true;
		}
		std::cout << "  check-in fail!\n";
		return false;
	}
	//----< check out file >-------------------------------------------------

	inline bool RepositoryCore::checkOut(const std::string& filename, const std::string& num) {
		CheckOut checkout_(db_, filename, num);
		checkout_.storepath("../Storage/");
		checkout_.specpath("../TempForServer/");
		if (checkout_.checkout()) {
			std::cout << "\n  check-out success!";
			return true;
		}
		std::cout << "\n  check-out fail!";
		return false;
	}
	//----< close element >--------------------------------------------------

	inline bool RepositoryCore::close(const std::string& filename) {
		PayLoad p = db_[filename].payLoad();
		p.status(false);
		db_[filename].payLoad(p);
		return true;
	}
}

#endif