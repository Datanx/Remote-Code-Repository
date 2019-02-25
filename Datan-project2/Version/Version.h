#ifndef VERSION_H
#define VERSION_H
///////////////////////////////////////////////////////////////////////
// Version.h - manages version numbering                             //
//             for all files held in the Repository.                 //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines a single Version class that:
*  - accepts a DbCore<P> and key(filename) instance when constructed
*  - provides facilities to convert a int version to string version
*
*  Required Files:
*  ---------------
*  Version.h, Version.cpp
*  DbCore.h, DbCore.cpp
*  PayLoad.h, PayLoad.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 6 March 2018
*  - first release
*/

#include "../Executive/NoSqlDb.h"
#include "../PayLoad/PayLoad.h"


namespace NoSqlDb {
	class Version {
	public:
		Version() {}
		Version(const DbCore<PayLoad>& db, const std::string& filename) : db_(db), filename_(filename) {}

		// methods to get and set Version member data

		DbCore<PayLoad>& db() { return db_; }
		DbCore<PayLoad> db() const { return db_; }
		void db(const DbCore<PayLoad>& db) { db_ = db; }

		std::string& filename() { return filename_; }
		std::string filename() const { return filename_; }
		void filename(const std::string& filename) { filename_ = filename; }

		std::string& versnum() { return versnum_; }
		std::string versnum() const { return versnum_; }

		// method to convert a int version to string version
		
		Version& version();

	private:
		std::string filename_;
		DbCore<PayLoad>  db_;
		std::string versnum_;
	};

	//----< convert a int version to string version >---------------------------
	Version& Version::version()
	{
		versnum_ = "." + std::to_string(db_[filename_].version().back());
		return *this;
	}

}

#endif
