//Implemented by Jin Xin
#pragma once
#include"API.h"
#include"Exceptions.h"
#include"RecordManager.h"
#include<iostream>
#include <boost/filesystem.hpp>
#include<iomanip>

using namespace std;

/*API构造函数*/
API::API(string path) :path_(path)
{
	catalog_manager_ = new CatalogManager(path);
}

/*API析构函数*/
API::~API(void)
{
	delete buffer_manager_;
	delete catalog_manager_;
}

/*显示各种命令*/
void API::help()
{
	cout << setiosflags(ios::left) << endl;
	cout << "下列为数据库所有可用的sql命令：" << endl;
	cout << "注意所有的命令需要在一行键入，并以';'分号结束。" << endl<<endl;
	cout << "-------------------------------------------------------------" << endl;
	cout << setw(16) << "help" << setw(2) << "|" << "显示改帮助界面。例：help;" << endl;
	cout << setw(16) << "show databases" << setw(2) << "|" << "显示所有数据库。例：show databases;" << endl;
	cout << setw(16) << "show tables" << setw(2) << "|" << "显示当前数据库数据表。例：show tables;" << endl;
	cout << setw(16) << "use" << setw(2) << "|" << "选定另一个数据库。例：use university;" << endl;
	cout << setw(16) << "create database" << setw(2) << "|" << "创建一个数据库。例：create database university;" << endl;
	cout << setw(16) << "create table" << setw(2) << "|" << "在当前数据库创建一张数据表。例：create table student(id int,name char(20),primary key(id));" << endl;
	cout << setw(16) << "create index" << setw(2) << "|" << "在一张表上创建索引。例：create index on student(id);" << endl;
	cout << setw(16) << "drop database" << setw(2) << "|" << "删除数据库。例：drop database university;" << endl;
	cout << setw(16) << "drop table" << setw(2) << "|" << "删除当前数据库的一张数据表。例：drop table student;" << endl;
	cout << setw(16) << "drop index" << setw(2) << "|" << "删除表的索引。 drop index i1;" << endl;
	cout << setw(16) << "select" << setw(2) << "|" << "查询数据。例：selete * from student where id=1 and name='Tom';" << endl;
	cout << setw(16) << "insert" << setw(2) << "|" << "插入数据。例：insert into student values(2,'Tim');" << endl;
	cout << setw(16) << "delete" << setw(2) << "|" << "删除数据。例：delete from student where id=2;" << endl;
	cout << setw(16) << "update" << setw(2) << "|" << "更新数据。例：update student set name='Tom' where id='2';" << endl;
	cout << "-------------------------------------------------------------" << endl;
}

/*创建数据库，调用boost库操作对应文件；调用catalog Manager实现数据库创建*/
void API::CreateDatabase(SQLCreateDatabase& sql_statement)
{
	cout << "创建数据库: " << sql_statement.get_database_name() << endl;	
	string folder_name(path_ + sql_statement.get_database_name());/*获取该database应该创建的文件地址*/
	boost::filesystem::path folder_path(folder_name);/*【file system】利用boost库为该database创建文件*/

	if (catalog_manager_->GetDB(sql_statement.get_database_name()) != NULL) throw DatabaseAlreadyExistsException();/*【catalog manager】判断catalog中是否已经有该database。如果有返回异常，如果没有继续执行。*/
	if (boost::filesystem::exists(folder_path))/*【file system】利用boost库判断该文件地址是否已存在。存在则删除。不存在则继续执行*/
	{
		boost::filesystem::remove_all(folder_path);/*【file system】利用boost库删除该文件地址对应文件*/
		cout << "数据库文件夹已存在，并已删除。" << endl;
	}
	boost::filesystem::create_directories(folder_path);/*【file system】利用boost库创建该文件地址对应文件*/
	cout << "数据库文件夹已创建。" << endl;
	catalog_manager_->CreateDatabase(sql_statement.get_database_name());/*【catalog manager】在catalog中创建该数据库*/
	cout << "目录管理已写入。" << endl<<endl;
	catalog_manager_->WriteArchiveFile();/*【catalog manager】在catalog中写文档*/

	/*cout << "创建数据库:" << sql_statement.get_database_name() << endl;*/
}

/*创建数据表 找出要创建数据表的数据库;操作对应文件;利用catalog Manager实现对应数据表的创建*/
void API::CreateTable(SQLCreateTable& sql_statement)
{
	cout << "创建表: " << sql_statement.get_table_name() << endl;
	if (current_database_.length() == 0) throw NoDatabaseSelectedException();/*判断当前数据库是否选中*/
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL) throw DatabaseNotExistException();/*【catalog Manager】判断当前数据库是否存在*/
	if (db->GetTable(sql_statement.get_table_name()) != NULL) throw TableAlreadyExistsException();/*【catalog Manager】判断当前数据库是否已有该数据表*/

	string file_name(path_ + current_database_ + "/" + sql_statement.get_table_name() + ".records");
	boost::filesystem::path folder_path(file_name);/*【file System】利用boost库为该table创建文件*/

	if (boost::filesystem::exists(folder_path))/*【file System】利用boost库判断该文件地址是否已存在。存在则删除。不存在则继续执行*/
	{
		boost::filesystem::remove_all(folder_path);/*【file System】利用boost库删除该文件地址对应文件*/
		cout << "数据表文件已存在，并已删除。" << endl;
	}
	ofstream ofs(file_name);/*【file System】创建该文件地址对应文件*/
	ofs.close();
	cout << "数据表文件已创建。" << endl;

	db->CreateTable(sql_statement);/*【catalog Manager】在该database中创建该数据表*/

	cout << "目录管理已写入" << endl<<endl;
	//【catalog Manager】在catalog中写文档
	catalog_manager_->WriteArchiveFile();

	//cout << "在当前数据库中创建数据表" + sql_statement.get_table_name()<< endl;
}

/*创建索引 找出要创建索引的数据库;操作对应文件;利用index manager实现对应index的创建*/
void API::CreateIndex(SQLCreateIndex& sql_statement)
{
	cout << "创建索引: " << sql_statement.get_index_name() << endl;
	if (current_database_.length() == 0) throw NoDatabaseSelectedException();

	Database *db = catalog_manager_->GetDB(current_database_);
	if (db->GetTable(sql_statement.get_tb_name()) == NULL) throw TableNotExistException();
	if (db->CheckIfIndexExists(sql_statement.get_index_name())) throw IndexAlreadyExistsException();

	IndexManager *im = new IndexManager(catalog_manager_, buffer_manager_, current_database_);
	im->CreateIndex(sql_statement);
	delete im;
	cout << "在 "+ sql_statement.get_tb_name()+" 中创建索引 "+ sql_statement.get_index_name()<<endl;
}

/*显示数据库*/
void API::ShowDatabases()
{
	vector<Database> dbs = catalog_manager_->GetDBs();/*【catalog manager】获取所有数据库*/
	if (dbs.size() == 0)/*如果不存在数据库*/
	{
		cout << "当前不存在数据库，请创建。" << endl;
		return;
	}
	cout << setiosflags(ios::left) << endl;
	cout << "┏──────────┓" << endl;
	cout << "│ " << setw(18) << "数据库" << " │" << endl;
	cout << "┣──────────┫" << endl;
	for (auto db = dbs.begin(); db != dbs.end(); db++)
		cout << "│ " << setw(18) << (*db).get_database_name() << " │" << endl;
	cout << "┕──────────┛" << endl;
	cout << "共有： " << dbs.size() << " 个数据库。" << endl;
	//cout << "显示数据库：" << endl;
}

/*显示数据表*/
void API::ShowTables()
{
	if (current_database_.size() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);/*获取当前数据库*/
	if (db == NULL)/*【catalog manager】判断当前数据库不存在*/
	{
		throw DatabaseNotExistException();
	}
	if (db->get_tables().size() == 0)/*如果不存在数据表*/
	{
		cout <<"数据库：" +db->get_database_name()+ " 中不存在数据表。请创建。" << endl;
		return;
	}
	cout << setiosflags(ios::left) << endl;
	cout << "┏───────────┓" << endl;
	cout << "│ " << setw(20) << current_database_+"中的数据表" << " │" << endl;
	cout << "┣───────────┫" << endl;
	for (auto tb = db->get_tables().begin(); tb != db->get_tables().end(); tb++)
		cout << "│ " << setw(20) << (*tb).get_tb_name() << " │" << endl;
	cout << "┕───────────┛" << endl;
	//cout << "显示数据表";
}

/*删除数据库*/
void API::DropDatabase(SQLDropDatabase& sql_statement)
{
	cout << "删除数据库: " << sql_statement.get_database_name() << endl;
	bool isExist = false;

	vector<Database> dbs = catalog_manager_->GetDBs();/*【catalog manager】获取所有数据库*/
	for (auto db = dbs.begin(); db != dbs.end(); db++)
	{
		if (db->get_database_name() == sql_statement.get_database_name())
		{
			isExist = true;
			break;
		}
	}
	if (isExist == false)/*判断该数据库是否存在*/
	{
		throw DatabaseNotExistException();
	}
	string folder_name(path_ + sql_statement.get_database_name());/*获取该数据库的文件地址*/
	boost::filesystem::path folder_path(folder_name);/*【file system】利用boost库获取该数据库的文件地址*/

	if (!boost::filesystem::exists(folder_path))/*判断该文件地址文件是否存在*/
	{
		cout << sql_statement.get_database_name()+" 数据库文件不存在。" << endl;
	}
	else
	{
		boost::filesystem::remove_all(folder_path);
		cout << sql_statement.get_database_name()+" 数据库文件已删除。" << endl;
	}

	catalog_manager_->DeleteDatabase(sql_statement.get_database_name());/*【catalog manager】在目录管理器内删除该数据库*/
	cout << sql_statement.get_database_name()+ " 数据库从目录中删除" << endl;
	catalog_manager_->WriteArchiveFile();/*【catalog manager】在catalog中写文档*/

	if (sql_statement.get_database_name() == current_database_)/*如果当前选定的数据库是该数据库，则选定失效*/
	{
		current_database_ = "";
		delete buffer_manager_;
	}
	//cout << "删除数据库。" << endl;
}

/*删除数据表*/
void API::DropTable(SQLDropTable& sql_statement)
{
	cout << "Droping table: " << sql_statement.get_table_name() << endl;
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}

	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*判断该当前数据库是否存在*/
	{
		throw DatabaseNotExistException();
	}

	Table *tb = db->GetTable(sql_statement.get_table_name());
	if (tb == NULL)/*判断该数据库是否存在数据表*/
	{
		throw TableNotExistException();
	}
	string file_name(path_ + current_database_ + "/" + sql_statement.get_table_name() + ".records");/*获取该数据表的文件地址*/

	if (!boost::filesystem::exists(file_name))/*【file system】利用boost库判断该文件是否存在*/
	{
		cout << sql_statement.get_table_name() +" 数据表文件不存在。" << endl;
	}
	else
	{
		boost::filesystem::remove(file_name);/*【file system】利用boost库删除该文件*/
		cout << sql_statement.get_table_name() + "数据表文件已删除。" << endl;
	}

	cout << "删除数据表下索引文件。" << endl;
	for (unsigned int i = 0; i < tb->GetIndexNum(); i++)
	{
		string file_name(path_ + current_database_ + "/" + tb->GetIndex(i)->get_name() + ".index");/*获取该数据表的文件地址*/
		if (!boost::filesystem::exists(file_name))/*【file system】利用boost库判断该文件是否存在*/
		{
			cout << "索引文件不存在。" << endl;
		}
		else
		{
			boost::filesystem::remove(file_name);
			cout << "索引文件已删除" << endl;
		}
	}

	db->DropTable(sql_statement);/*【catalog manager】在目录中删除该数据表*/
	cout << "目录文件已写入。" << endl;
	catalog_manager_->WriteArchiveFile();/*【catalog manager】在catalog中写文档*/
	//cout << "删除数据表。" << endl;
}

/*删除索引*/
void API::DropIndex(SQLDropIndex& sql_statement)
{
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*判断当前数据库是否存在*/
	{
		throw DatabaseNotExistException();
	}
	if (!db->CheckIfIndexExists(sql_statement.get_index_name()))/*【catalog manager】判断index是否存在*/
	{
		throw IndexNotExistException();
	}
	string file_name(path_ + current_database_ + "/" + sql_statement.get_index_name() + ".index");/*【file system】获取文件地址*/
	if (!boost::filesystem::exists(file_name))/*【file system】利用boost库判断当前文件是否存在*/
	{
		cout << "索引文件不存在" << endl;
		return;
	}
	boost::filesystem::remove(file_name);/*【file system】利用boost库删除文件地址对应文件*/
	cout << "索引文件已删除。" << endl;

	db->DropIndex(sql_statement);/*【catalog manager】目录结构中删除索引*/
	cout << "目录文件已写入。" << endl;
	catalog_manager_->WriteArchiveFile();/*【catalog manager】在catalog中写文档*/
	//cout << "删除索引。" << endl;
}

/*选定当前数据库*/
void API::Use(SQLUse& sql_statement)
{
	cout << "进入 "+ sql_statement .get_database_name()+" 数据库中。"<< endl;
	Database *db = catalog_manager_->GetDB(sql_statement.get_database_name());
	if (db == NULL)/*当前数据库不存在*/
	{
		throw DatabaseNotExistException();
	}
	if (current_database_.length() != 0)/*选定新的数据库之前已选定旧数据库*/
	{
		cout << "关闭已选定数据库：" << current_database_ << endl;
		catalog_manager_->WriteArchiveFile();/*【catalog manager】在catalog中写文档*/
		delete buffer_manager_;
	}
	current_database_ = sql_statement.get_database_name();/*更新当前数据库*/
	buffer_manager_ = new BufferManager(path_);/*更新缓存管理器*/
	cout << endl << "数据库" + sql_statement.get_database_name() + "已进入。" << endl;
	cout << "选择数据库" << endl<<endl;
}

/*插入数据*/
void API::Insert(SQLInsert& sql_statement)
{
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*判断当前数据库是否存在*/
	{
		throw DatabaseNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Insert(sql_statement);
	delete rm;
	//cout << "插入数据。";
}

/*查询数据*/
void API::Select(SQLSelect& sql_statement)
{
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_tb_name());
	if (tb == NULL)/*判断当前表是否存在*/
	{
		throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	vector<vector<TKey>>result;
	result = rm->Select(sql_statement);
	delete rm;
	//cout << "查询数据" << endl;
}

//查询数据
//void API::Select(SQLSelect& sql_statement)
//{
//	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
//	{
//		throw NoDatabaseSelectedException();
//	}
//	Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_tb_name());
//	if (tb == NULL)/*判断当前表是否存在*/
//	{
//		throw TableNotExistException();
//	}
//	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
//	rm->Select(sql_statement);
//	delete rm;
//	//cout << "查询数据" << endl;
//}

/*join查询*/
void API::JoinSelect(SQLJoinSelect& sql_statement)
{
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	int table_count = sql_statement.get_table_names().size();
	for (int i = 0; i < table_count; i++)
	{
		Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_table_names()[i]);
		if (tb == NULL)
			throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->JoinSelect(sql_statement);
	delete rm;
}

/*删除数据*/
void API::Delete(SQLDelete& sql_statement)
{
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_tb_name());
	if (tb == NULL)/*判断当前数据库是否存在*/
	{
		throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Delete(sql_statement);
	delete rm;
	//cout << "删除数据" << endl;
}

/*更新数据*/
void API::Update(SQLUpdate& sql_statement)
{
	if (current_database_.length() == 0)/*判断当前数据库是否选定*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*判断当前数据库是否存在*/
	{
		throw DatabaseNotExistException();
	}
	Table *tb = db->GetTable(sql_statement.get_tb_name());
	if (tb == NULL)/*判断数据表是否存在是否存在*/
	{
		throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Update(sql_statement);
	delete rm;
	//cout << "更新数据" << endl;
}
