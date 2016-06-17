//Implemented by Jin Xin
#include"API.h"
#include"Exceptions.h"
#include"RecordManager.h"
#include<iostream>
#include <boost/filesystem.hpp>
#include<iomanip>

using namespace std;

/*API���캯��*/
API::API(string path) :path_(path)
{
	catalog_manager_ = new CatalogManager(path);
}

/*API��������*/
API::~API(void)
{
	delete buffer_manager_;
	delete catalog_manager_;
}

/*��ʾ��������*/
void API::help()
{
	cout << setiosflags(ios::left) << endl;
	cout << "����Ϊ���ݿ����п��õ�sql���" << endl;
	cout << "ע�����е�������Ҫ��һ�м��룬����';'�ֺŽ�����" << endl << endl;
	cout << "-------------------------------------------------------------" << endl;
	cout << setw(16) << "help" << setw(2) << "|" << "��ʾ�İ������档����help;" << endl;
	cout << setw(16) << "show databases" << setw(2) << "|" << "��ʾ�������ݿ⡣����show databases;" << endl;
	cout << setw(16) << "show tables" << setw(2) << "|" << "��ʾ��ǰ���ݿ����ݱ�����show tables;" << endl;
	cout << setw(16) << "use" << setw(2) << "|" << "ѡ����һ�����ݿ⡣����use university;" << endl;
	cout << setw(16) << "create database" << setw(2) << "|" << "����һ�����ݿ⡣����create database university;" << endl;
	cout << setw(16) << "create table" << setw(2) << "|" << "�ڵ�ǰ���ݿⴴ��һ�����ݱ�����create table student(id int,name char(20),primary key(id));" << endl;
	cout << setw(16) << "create index" << setw(2) << "|" << "��һ�ű��ϴ�������������create index on student(id);" << endl;
	cout << setw(16) << "drop database" << setw(2) << "|" << "ɾ�����ݿ⡣����drop database university;" << endl;
	cout << setw(16) << "drop table" << setw(2) << "|" << "ɾ����ǰ���ݿ��һ�����ݱ�����drop table student;" << endl;
	cout << setw(16) << "drop index" << setw(2) << "|" << "ɾ����������� drop index i1;" << endl;
	cout << setw(16) << "select" << setw(2) << "|" << "��ѯ���ݡ�����selete * from student where id=1 and name='Tom';" << endl;
	cout << setw(16) << "insert" << setw(2) << "|" << "�������ݡ�����insert into student values(2,'Tim');" << endl;
	cout << setw(16) << "delete" << setw(2) << "|" << "ɾ�����ݡ�����delete from student where id=2;" << endl;
	cout << setw(16) << "update" << setw(2) << "|" << "�������ݡ�����update student set name='Tom' where id='2';" << endl;
	cout << "-------------------------------------------------------------" << endl;
}

/*�������ݿ⣬����boost�������Ӧ�ļ�������catalog Managerʵ�����ݿⴴ��*/
void API::CreateDatabase(SQLCreateDatabase& sql_statement)
{
	cout << "�������ݿ�: " << sql_statement.get_database_name() << endl;
	string folder_name(path_ + sql_statement.get_database_name());/*��ȡ��databaseӦ�ô������ļ���ַ*/
	boost::filesystem::path folder_path(folder_name);/*��file system������boost��Ϊ��database�����ļ�*/

	if (catalog_manager_->GetDB(sql_statement.get_database_name()) != NULL) throw DatabaseAlreadyExistsException();/*��catalog manager���ж�catalog���Ƿ��Ѿ��и�database������з����쳣�����û�м���ִ�С�*/
	if (boost::filesystem::exists(folder_path))/*��file system������boost���жϸ��ļ���ַ�Ƿ��Ѵ��ڡ�������ɾ���������������ִ��*/
	{
		boost::filesystem::remove_all(folder_path);/*��file system������boost��ɾ�����ļ���ַ��Ӧ�ļ�*/
		cout << "���ݿ��ļ����Ѵ��ڣ�����ɾ����" << endl;
	}
	boost::filesystem::create_directories(folder_path);/*��file system������boost�ⴴ�����ļ���ַ��Ӧ�ļ�*/
	cout << "���ݿ��ļ����Ѵ�����" << endl;
	catalog_manager_->CreateDatabase(sql_statement.get_database_name());/*��catalog manager����catalog�д��������ݿ�*/
	cout << "Ŀ¼������д�롣" << endl << endl;
	catalog_manager_->WriteArchiveFile();/*��catalog manager����catalog��д�ĵ�*/

										 /*cout << "�������ݿ�:" << sql_statement.get_database_name() << endl;*/
}

/*�������ݱ� �ҳ�Ҫ�������ݱ�����ݿ�;������Ӧ�ļ�;����catalog Managerʵ�ֶ�Ӧ���ݱ�Ĵ���*/
void API::CreateTable(SQLCreateTable& sql_statement)
{
	cout << "������: " << sql_statement.get_table_name() << endl;
	if (current_database_.length() == 0) throw NoDatabaseSelectedException();/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL) throw DatabaseNotExistException();/*��catalog Manager���жϵ�ǰ���ݿ��Ƿ����*/
	if (db->GetTable(sql_statement.get_table_name()) != NULL) throw TableAlreadyExistsException();/*��catalog Manager���жϵ�ǰ���ݿ��Ƿ����и����ݱ�*/

	string file_name(path_ + current_database_ + "/" + sql_statement.get_table_name() + ".records");
	boost::filesystem::path folder_path(file_name);/*��file System������boost��Ϊ��table�����ļ�*/

	if (boost::filesystem::exists(folder_path))/*��file System������boost���жϸ��ļ���ַ�Ƿ��Ѵ��ڡ�������ɾ���������������ִ��*/
	{
		boost::filesystem::remove_all(folder_path);/*��file System������boost��ɾ�����ļ���ַ��Ӧ�ļ�*/
		cout << "���ݱ��ļ��Ѵ��ڣ�����ɾ����" << endl;
	}
	ofstream ofs(file_name);/*��file System���������ļ���ַ��Ӧ�ļ�*/
	ofs.close();
	cout << "���ݱ��ļ��Ѵ�����" << endl;

	db->CreateTable(sql_statement);/*��catalog Manager���ڸ�database�д��������ݱ�*/

	cout << "Ŀ¼������д��" << endl << endl;
	//��catalog Manager����catalog��д�ĵ�
	catalog_manager_->WriteArchiveFile();

	//cout << "�ڵ�ǰ���ݿ��д������ݱ�" + sql_statement.get_table_name()<< endl;
}

/*�������� �ҳ�Ҫ�������������ݿ�;������Ӧ�ļ�;����index managerʵ�ֶ�Ӧindex�Ĵ���*/
void API::CreateIndex(SQLCreateIndex& sql_statement)
{
	cout << "��������: " << sql_statement.get_index_name() << endl;
	if (current_database_.length() == 0) throw NoDatabaseSelectedException();

	Database *db = catalog_manager_->GetDB(current_database_);
	if (db->GetTable(sql_statement.get_tb_name()) == NULL) throw TableNotExistException();
	if (db->CheckIfIndexExists(sql_statement.get_index_name())) throw IndexAlreadyExistsException();

	IndexManager *im = new IndexManager(catalog_manager_, buffer_manager_, current_database_);
	im->CreateIndex(sql_statement);
	delete im;
	cout << "�� " + sql_statement.get_tb_name() + " �д������� " + sql_statement.get_index_name() << endl;
}

/*��ʾ���ݿ�*/
void API::ShowDatabases()
{
	vector<Database> dbs = catalog_manager_->GetDBs();/*��catalog manager����ȡ�������ݿ�*/
	if (dbs.size() == 0)/*������������ݿ�*/
	{
		cout << "��ǰ���������ݿ⣬�봴����" << endl;
		return;
	}
	cout << setiosflags(ios::left) << endl;
	cout << "������������������������" << endl;
	cout << "�� " << setw(18) << "���ݿ�" << " ��" << endl;
	cout << "�ǩ���������������������" << endl;
	for (auto db = dbs.begin(); db != dbs.end(); db++)
		cout << "�� " << setw(18) << (*db).get_database_name() << " ��" << endl;
	cout << "������������������������" << endl;
	cout << "���У� " << dbs.size() << " �����ݿ⡣" << endl;
	//cout << "��ʾ���ݿ⣺" << endl;
}

/*��ʾ���ݱ�*/
void API::ShowTables()
{
	if (current_database_.size() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);/*��ȡ��ǰ���ݿ�*/
	if (db == NULL)/*��catalog manager���жϵ�ǰ���ݿⲻ����*/
	{
		throw DatabaseNotExistException();
	}
	if (db->get_tables().size() == 0)/*������������ݱ�*/
	{
		cout << "���ݿ⣺" + db->get_database_name() + " �в��������ݱ��봴����" << endl;
		return;
	}
	cout << setiosflags(ios::left) << endl;
	cout << "��������������������������" << endl;
	cout << "�� " << setw(20) << current_database_ + "�е����ݱ�" << " ��" << endl;
	cout << "�ǩ�����������������������" << endl;
	for (auto tb = db->get_tables().begin(); tb != db->get_tables().end(); tb++)
		cout << "�� " << setw(20) << (*tb).get_tb_name() << " ��" << endl;
	cout << "��������������������������" << endl;
	//cout << "��ʾ���ݱ�";
}

/*ɾ�����ݿ�*/
void API::DropDatabase(SQLDropDatabase& sql_statement)
{
	cout << "ɾ�����ݿ�: " << sql_statement.get_database_name() << endl;
	bool isExist = false;

	vector<Database> dbs = catalog_manager_->GetDBs();/*��catalog manager����ȡ�������ݿ�*/
	for (auto db = dbs.begin(); db != dbs.end(); db++)
	{
		if (db->get_database_name() == sql_statement.get_database_name())
		{
			isExist = true;
			break;
		}
	}
	if (isExist == false)/*�жϸ����ݿ��Ƿ����*/
	{
		throw DatabaseNotExistException();
	}
	string folder_name(path_ + sql_statement.get_database_name());/*��ȡ�����ݿ���ļ���ַ*/
	boost::filesystem::path folder_path(folder_name);/*��file system������boost���ȡ�����ݿ���ļ���ַ*/

	if (!boost::filesystem::exists(folder_path))/*�жϸ��ļ���ַ�ļ��Ƿ����*/
	{
		cout << sql_statement.get_database_name() + " ���ݿ��ļ������ڡ�" << endl;
	}
	else
	{
		boost::filesystem::remove_all(folder_path);
		cout << sql_statement.get_database_name() + " ���ݿ��ļ���ɾ����" << endl;
	}

	catalog_manager_->DeleteDatabase(sql_statement.get_database_name());/*��catalog manager����Ŀ¼��������ɾ�������ݿ�*/
	cout << sql_statement.get_database_name() + " ���ݿ��Ŀ¼��ɾ��" << endl;
	catalog_manager_->WriteArchiveFile();/*��catalog manager����catalog��д�ĵ�*/

	if (sql_statement.get_database_name() == current_database_)/*�����ǰѡ�������ݿ��Ǹ����ݿ⣬��ѡ��ʧЧ*/
	{
		current_database_ = "";
		delete buffer_manager_;
	}
	//cout << "ɾ�����ݿ⡣" << endl;
}

/*ɾ�����ݱ�*/
void API::DropTable(SQLDropTable& sql_statement)
{
	cout << "Droping table: " << sql_statement.get_table_name() << endl;
	if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}

	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*�жϸõ�ǰ���ݿ��Ƿ����*/
	{
		throw DatabaseNotExistException();
	}

	Table *tb = db->GetTable(sql_statement.get_table_name());
	if (tb == NULL)/*�жϸ����ݿ��Ƿ�������ݱ�*/
	{
		throw TableNotExistException();
	}
	string file_name(path_ + current_database_ + "/" + sql_statement.get_table_name() + ".records");/*��ȡ�����ݱ���ļ���ַ*/

	if (!boost::filesystem::exists(file_name))/*��file system������boost���жϸ��ļ��Ƿ����*/
	{
		cout << sql_statement.get_table_name() + " ���ݱ��ļ������ڡ�" << endl;
	}
	else
	{
		boost::filesystem::remove(file_name);/*��file system������boost��ɾ�����ļ�*/
		cout << sql_statement.get_table_name() + "���ݱ��ļ���ɾ����" << endl;
	}

	cout << "ɾ�����ݱ��������ļ���" << endl;
	for (unsigned int i = 0; i < tb->GetIndexNum(); i++)
	{
		string file_name(path_ + current_database_ + "/" + tb->GetIndex(i)->get_name() + ".index");/*��ȡ�����ݱ���ļ���ַ*/
		if (!boost::filesystem::exists(file_name))/*��file system������boost���жϸ��ļ��Ƿ����*/
		{
			cout << "�����ļ������ڡ�" << endl;
		}
		else
		{
			boost::filesystem::remove(file_name);
			cout << "�����ļ���ɾ��" << endl;
		}
	}

	db->DropTable(sql_statement);/*��catalog manager����Ŀ¼��ɾ�������ݱ�*/
	cout << "Ŀ¼�ļ���д�롣" << endl;
	catalog_manager_->WriteArchiveFile();/*��catalog manager����catalog��д�ĵ�*/
										 //cout << "ɾ�����ݱ�" << endl;
}

/*ɾ������*/
void API::DropIndex(SQLDropIndex& sql_statement)
{
	if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*�жϵ�ǰ���ݿ��Ƿ����*/
	{
		throw DatabaseNotExistException();
	}
	if (!db->CheckIfIndexExists(sql_statement.get_index_name()))/*��catalog manager���ж�index�Ƿ����*/
	{
		throw IndexNotExistException();
	}
	string file_name(path_ + current_database_ + "/" + sql_statement.get_index_name() + ".index");/*��file system����ȡ�ļ���ַ*/
	if (!boost::filesystem::exists(file_name))/*��file system������boost���жϵ�ǰ�ļ��Ƿ����*/
	{
		cout << "�����ļ�������" << endl;
		return;
	}
	boost::filesystem::remove(file_name);/*��file system������boost��ɾ���ļ���ַ��Ӧ�ļ�*/
	cout << "�����ļ���ɾ����" << endl;

	db->DropIndex(sql_statement);/*��catalog manager��Ŀ¼�ṹ��ɾ������*/
	cout << "Ŀ¼�ļ���д�롣" << endl;
	catalog_manager_->WriteArchiveFile();/*��catalog manager����catalog��д�ĵ�*/
										 //cout << "ɾ��������" << endl;
}

/*ѡ����ǰ���ݿ�*/
void API::Use(SQLUse& sql_statement)
{
	cout << "���� " + sql_statement.get_database_name() + " ���ݿ��С�" << endl;
	Database *db = catalog_manager_->GetDB(sql_statement.get_database_name());
	if (db == NULL)/*��ǰ���ݿⲻ����*/
	{
		throw DatabaseNotExistException();
	}
	if (current_database_.length() != 0)/*ѡ���µ����ݿ�֮ǰ��ѡ�������ݿ�*/
	{
		cout << "�ر���ѡ�����ݿ⣺" << current_database_ << endl;
		catalog_manager_->WriteArchiveFile();/*��catalog manager����catalog��д�ĵ�*/
		delete buffer_manager_;
	}
	current_database_ = sql_statement.get_database_name();/*���µ�ǰ���ݿ�*/
	buffer_manager_ = new BufferManager(path_);/*���»��������*/
	cout << endl << "���ݿ�" + sql_statement.get_database_name() + "�ѽ��롣" << endl;
	cout << "ѡ�����ݿ�" << endl << endl;
}

/*��������*/
void API::Insert(SQLInsert& sql_statement)
{
	if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*�жϵ�ǰ���ݿ��Ƿ����*/
	{
		throw DatabaseNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Insert(sql_statement);
	delete rm;
	//cout << "�������ݡ�";
}

/*��ѯ����*/
void API::Select(SQLSelect& sql_statement)
{
	if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}
	Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_tb_name());
	if (tb == NULL)/*�жϵ�ǰ���ݿ��Ƿ����*/
	{
		throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Select(sql_statement);
	delete rm;
	//cout << "��ѯ����" << endl;
}

/*join��ѯ*/
void API::JoinSelect(SQLJoinSelect& sql_statement)
{
	//if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	//{
	//	throw NoDatabaseSelectedException();
	//}
	//Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_tb_name());
	//if (tb == NULL)/*�жϵ�ǰ���ݿ��Ƿ����*/
	//{
	//	throw TableNotExistException();
	//}
	//RecordManager *rm = new RecordManager(catalog_manager_, catalog_manager_, current_database_);
	//rm->JoinSelect(sql_statement);
	//delete rm;
}

/*ɾ������*/
void API::Delete(SQLDelete& sql_statement)
{
	if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}
	Table *tb = catalog_manager_->GetDB(current_database_)->GetTable(sql_statement.get_tb_name());
	if (tb == NULL)/*�жϵ�ǰ���ݿ��Ƿ����*/
	{
		throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Delete(sql_statement);
	delete rm;
	//cout << "ɾ������" << endl;
}

/*��������*/
void API::Update(SQLUpdate& sql_statement)
{
	if (current_database_.length() == 0)/*�жϵ�ǰ���ݿ��Ƿ�ѡ��*/
	{
		throw NoDatabaseSelectedException();
	}
	Database *db = catalog_manager_->GetDB(current_database_);
	if (db == NULL)/*�жϵ�ǰ���ݿ��Ƿ����*/
	{
		throw DatabaseNotExistException();
	}
	Table *tb = db->GetTable(sql_statement.get_tb_name());
	if (tb == NULL)/*�ж����ݱ��Ƿ�����Ƿ����*/
	{
		throw TableNotExistException();
	}
	RecordManager *rm = new RecordManager(catalog_manager_, buffer_manager_, current_database_);
	rm->Update(sql_statement);
	delete rm;
	//cout << "��������" << endl;
}