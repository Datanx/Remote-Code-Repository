#ifndef BROWSE_H
#define BROWSE_H
///////////////////////////////////////////////////////////////////////
// Browse.h -  provides the capability to locate files               //  
//             and view their contents.                              //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines a single Browse class that:
*  - accepts a DbCore<P> and filename instance when constructed
*  - provides facilities to display package descriptions and contents.
*
*  Required Files:
*  ---------------
*  Browse.h, Browse.cpp
*  DbCore.h, DbCore.cpp
*  PayLoad.h, PayLoad.cpp
*  Version.h, Version.cpp
*  Query.h, Query.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 6 March 2018
*  - first release
*/

#include "../Executive/NoSqlDb.h"
#include "../PayLoad/PayLoad.h"
#include "../Query/Query.h"
#include "../Process/Process.h"

namespace NoSqlDb {

	class Browse {
	public:
		Browse() {}
		Browse(DbCore<PayLoad>& db, const std::string& filename, const std::string& num) : db_(db), filename_(filename), versionum_(num) {}

		// methods to get and set Browse member data

		DbCore<PayLoad>& db() { return db_; }
		DbCore<PayLoad> db() const { return db_; }
		void db(const DbCore<PayLoad>& db) { db_ = db; }

		std::string& filename() { return filename_; }
		std::string filename() const { return filename_; }
		void filename(const std::string& filename) { filename_ = filename; }

		std::string& storepath() { return storepath_; }
		std::string storepath() const { return storepath_; }
		void storepath(const std::string& path) { storepath_ = path; }

		std::string& versionum() { return versionum_; }
		std::string versionum() const { return versionum_; }
		void versionum(const std::string& num) { versionum_ = num; }

		//method to browse

		bool browse();

	private:

		std::string filename_;
		DbCore<PayLoad> db_;
		std::string storepath_;
		std::string versionum_;
	};

	//----< browsing of one package by displaying package descriptions and contents>---------------------------

	bool Browse::browse()
	{
		if (db_.contains(filename_))
		{
			std::cout << "\n\n  Displaying file record:" << std::endl;
			NoSqlDb::showRecord(filename_, db_[filename_]);
			std::cout << "\n";
			std::cout << "\n  Demonstrating file codes";
			std::cout << "\n ======================================";

			Process p;
			std::string appPath = "c:/windows/system32/notepad.exe";
			p.application(appPath);

			std::string cmdLine = "/A " + storepath_ + filename_ + versionum_;
			p.commandLine(cmdLine);

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
		else
		{
			std::cout << " The file to be browsed does not exist." << std::endl;
			return false;
		}
	}

}

#endif


