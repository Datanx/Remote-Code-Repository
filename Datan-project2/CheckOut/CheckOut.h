#ifndef CHECKOUT_H
#define CHECKOUT_H
///////////////////////////////////////////////////////////////////////
// CheckIn.h - Check out files                                       //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines a single Check-out class that:
*  - accepts a DbCore<P> instance when constructed
*  - provides facilities to retrieves package files, 
*  - removing version information from their filenames.
*  - Retrieved files will be copied to a specified directory.
*
*  Required Files:
*  ---------------
*  CheckOut.h, CheckOut.cpp
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
#include "../FileSystem/FileSystem.h"
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <locale>


namespace NoSqlDb {

	class CheckOut {
	public:
		CheckOut() {}
		CheckOut(DbCore<PayLoad>& db, const std::string& filename, const std::string& num) : db_(db), filename_(filename), versionum_(num) {}

		// methods to get and set CheckIn member data

		DbCore<PayLoad>& db() { return db_; }
		DbCore<PayLoad> db() const { return db_; }
		void db(const DbCore<PayLoad>& db) { db_ = db; }

		std::string& filename() { return filename_; }
		std::string filename() const { return filename_; }
		void filename(const std::string& filename) { filename_ = filename; }

		std::string& specpath() { return specpath_; }
		std::string specpath() const { return specpath_; }
		void specpath(const std::string& path) { specpath_ = path; }

		std::string& storepath() { return storepath_; }
		std::string storepath() const { return storepath_; }
		void storepath(const std::string& path) { storepath_ = path; }

		std::string& versionum() { return versionum_; }
		std::string versionum() const { return versionum_; }
		void versionum(const std::string& num) { versionum_ = num; }

		//methods to check out

		bool checkout();

	private:
		bool filecp(const std::string& src, const std::string& des);

		std::string filename_;
		DbCore<PayLoad> db_;
		std::string specpath_;
		std::string storepath_;
		std::string versionum_;
	};


	//----< copy binary file from one directory to another >---------------------------

	bool CheckOut::filecp(const std::string& src, const std::string& des)
	{
		FileSystem::File me(src);
		me.open(FileSystem::File::in, FileSystem::File::binary);
		std::cout << "\n copying " << me.name().c_str() << " to " << des;
		if (!me.isGood())
		{
			std::cout << "\n  can't open executable\n";
			return false;
		}
		else
		{
			FileSystem::File you(des);
			you.open(FileSystem::File::out, FileSystem::File::binary);
			if (you.isGood())
			{
				while (me.isGood())
				{
					FileSystem::Block b = me.getBlock(1024);
					you.putBlock(b);
				}
			}
			return true;
		}
	}

	//----< check out package files at a time >---------------------------

	bool CheckOut::checkout()
	{
		if (db_.contains(filename_))
		{
			std::string src = storepath_ + filename_ + versionum_;
			std::string des = specpath_ + filename_;
			filecp(src, des);
			if (!db_[filename_].children().empty())
			{
				std::vector<std::string> children = db_[filename_].children();
				for (auto child : children)
				{
					std::string src1 = storepath_ + child + versionum_;
					std::string des1 = specpath_ + child;
					filecp(src1, des1);
				}
			}
			if (!db_.parents(filename_).empty())
			{
				std::vector<std::string> parents = db_.parents(filename_);
				for (auto item : parents)
				{
					std::string src2 = storepath_ + item + versionum_;
					std::string des2 = specpath_ + item;
					filecp(src2, des2);
				}
			}
			return true;
		}
		else
		{
			std::cout << " The package files don't exist." << std::endl;
			return false;
		}
	}

}

#endif