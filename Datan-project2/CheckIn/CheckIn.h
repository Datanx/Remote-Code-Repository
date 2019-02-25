#ifndef CHECKIN_H
#define CHECKIN_H
///////////////////////////////////////////////////////////////////////
// CheckIn.h - Check in files                                        //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines a single Check-in class that:
*  - accepts a DbCore<P> instance when constructed
*  - provides facilities to start a new package version by accepting files, 
*  - appending version numbers to their filenames, providing dependency and 
*  - category information, creating metadata, and storing the files in a designated location.
*
*  Required Files:
*  ---------------
*  CheckIn.h, CheckIn.cpp
*  DbCore.h, DbCore.cpp
*  PayLoad.h, PayLoad.cpp
*  Version.h, Version.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 6 March 2018
*  - first release
*/

#include "../Executive/NoSqlDb.h"
#include "../PayLoad/PayLoad.h"
#include "../FileSystem/FileSystem.h"
#include "../Version/Version.h"
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <locale>


namespace NoSqlDb {
	class CheckIn {
	public:
		CheckIn(){}
		CheckIn(DbCore<PayLoad>& db, const std::string& filename) : db_(db), filename_(filename) {}

		// methods to get and set CheckIn member data

		DbCore<PayLoad>& db() { return db_; }
		DbCore<PayLoad> db() const { return db_; }
		void db(const DbCore<PayLoad>& db) { db_ = db; }

		DbElement<PayLoad>& elem() { return elem_; }
		DbElement<PayLoad> elem() const { return elem_; }
		void elem(const DbElement<PayLoad>& elem) { elem_ = elem; }

		std::string& filename() { return filename_; }
		std::string filename() const { return filename_; }
		void filename(const std::string& filename) { filename_ = filename; }

		std::string& storepath() { return storepath_; }
		std::string storepath() const { return storepath_; }
		void storepath(const std::string& path) { storepath_ = path; }

		//methods to check in

		bool checkin();
		bool flag = false;

	private:
		bool filecp(const std::string& src, const std::string& des);
		bool depsclosed();
		void update(const DbElement<PayLoad>& elem2);

		std::string filename_;
		DbCore<PayLoad>  db_;
		DbElement<PayLoad> elem_;
		std::string storepath_;
	};

	//----< check if all the dependensies are closed >---------------------------
	bool CheckIn::depsclosed()
	{
		if (!db_.parents(filename_).empty())
		{
			std::vector<std::string> parents = db_.parents(filename_);
			for (auto item : parents)
			{
				if (!db_[item].payLoad().status())
					return false;
			}
			flag = true;
			std::cout << "\n  It's okay to close the file." << std::endl;

			return true;
		}
		else
		{
			flag = true;
			std::cout << "\n  It's okay to close the file." << std::endl;
			return true;
		}
	}

	//----< copy binary file from one directory to another >---------------------------

	bool CheckIn::filecp(const std::string& src, const std::string& des)
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

	//----< update dbElement >---------------------------

	void CheckIn::update(const DbElement<PayLoad>& elem2)
	{
		db_[filename_].children(elem2.children());
		db_[filename_].dateTime(elem2.dateTime());
		db_[filename_].descrip(elem2.descrip());
		db_[filename_].nameSpace(elem2.nameSpace());
		db_[filename_].payLoad(elem2.payLoad());
	}

	//----< check in one file at a time >---------------------------

	bool CheckIn::checkin()
	{
		if (db_.contains(filename_))
		{
			if (db_[filename_].name() != elem_.name())
			{
				std::cout << "\n  Different file owners. Check-in failed." << std::endl;
				return false;
			}
			else
			{
				std::string src = "../TempForServer/" + filename_;
				if (db_[filename_].payLoad().status() == true)
				{
					update(elem_);
					Version ver(db_, filename_);
					std::string des1 = storepath_ + filename_ + ver.version().versnum();
					depsclosed();
					if (filecp(src, des1))
						return true;
					else
						return false;
				}
				else
				{
					std::vector<int> temp = db_[filename_].version();
					int num = temp.back() + 1;
					temp.push_back(num);
					db_[filename_].version(temp);
					update(elem_);
					Version ver2(db_, filename_);
					std::string des2 = storepath_ + filename_ + ver2.version().versnum();
					depsclosed();
					if (filecp(src, des2))
					{
						PayLoad p;
						p = db_[filename_].payLoad();
						p.status(true);
						db_[filename_].payLoad(p);
						return true;
					}
					else
						return false;
				}
			}
		}
		else
		{
			std::vector<int> vers;
			vers.push_back(1);
			elem_.version(vers);
			db_.addRecord(filename_, elem_);
			depsclosed();
			filecp("../TempForServer/" + filename_, storepath_ + filename_ + ".1");
			return true;
		}
	}

}
#endif