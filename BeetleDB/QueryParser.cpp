#pragma region
//��飺��ѯ������
//Implemented by Jin Xin
//���ã���������û�����Ĳ�ѯ��䲢���ö�Ӧ��API����ĺ���
#pragma endregion

#include "QueryParser.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include<iostream>
using namespace std;

/*QueryParser���캯��*/
QueryParser::QueryParser()
{
	sql_type_ = -1;/*Ĭ�����ñ���sql_type_Ϊ-1*/
	string path = boost::filesystem::initial_path<boost::filesystem::path>().string() + "/DATABASEData/"; /*��ȡ��ǰ�ļ�exe���õ�ַ*/
	if (!boost::filesystem::exists(path))/*�ж��ļ��Ƿ���ڣ������ھʹ����ļ�*/
	{
		boost::filesystem::create_directory(path);
	}
	api = new API(path);/*����api����*/
}

/*QueryParser����������*/
QueryParser::~QueryParser(void)
{
	delete api;
}

/*����ӿڣ�����sql��ִ����Ӧ�Ĳ���*/
void QueryParser::ExecuteSQL(string sql)
{
	try
	{
		sql_ = sql;/*��ȡ�û������SQL���*/
		FormateSQL();/*�淶���û������sql*/
		GetSqlType();/*��ȡSQL��������*/
		InvokeAPI();/*�������͵��ö�Ӧ��API����*/
	}
	catch (exception e)
	{
		cout << e.what();
	}
}

/*�淶���û������sql�����ں������*/
void QueryParser::FormateSQL()
{
	//boost::regex reg("[\r\n\t]");
	//sql_ = boost::regex_replace(sql_, reg, " ");/*���С�tab���ÿհ׷�" "����*/
	//reg = ";.*$";
	//sql_ = boost::regex_replace(sql_, reg, "");/*ȥ���ֺ�֮�������*/
	//reg = "(^ +)|( +$)";
	//sql_ = boost::regex_replace(sql_, reg, "");/*ȥ����ǰ�����Ŀհ׷�*/
	//reg = " +";
	//sql_ = boost::regex_replace(sql_, reg, " ");/*ȥ���ظ��Ŀհ׷�*/
	//reg = " ?(\\(|\\)|,|=|(<>)|<|>) ?";
	//sql_ = boost::regex_replace(sql_, reg, " $1 ");
	//reg = "< *>";
	//sql_ = boost::regex_replace(sql_, reg, "<>");
	//reg = "< *=";
	//sql_ = boost::regex_replace(sql_, reg, "<=");
	//reg = "> *=";
	//sql_ = boost::regex_replace(sql_, reg, ">=");/*��( ) , = <> < >��Щ�ַ�֮ǰ֮����ӿհ׷�" "*/

	//sql_vector_ = SplitSQL(sql_, " ");
	// remove newlines, tabs
	boost::regex reg("[\r\n\t]");
	//string newstr(" ");
	sql_ = boost::regex_replace(sql_, reg, " ");
	// remove ; and chars after ;
	reg = ";.*$";
	//string newstr(" ");
	sql_ = boost::regex_replace(sql_, reg, "");
	// remove leading spaces and trailing spaces
	reg = "(^ +)|( +$)";
	sql_ = boost::regex_replace(sql_, reg, "");
	// remove duplicate spaces
	reg = " +";
	sql_ = boost::regex_replace(sql_, reg, " ");
	// insert space before or after ( ) , = <> < >
	reg = " ?(\\(|\\)|,|=|(<>)|<|>) ?";
	sql_ = boost::regex_replace(sql_, reg, " $1 ");
	reg = "< *>";
	sql_ = boost::regex_replace(sql_, reg, "<>");
	reg = "< *=";
	sql_ = boost::regex_replace(sql_, reg, "<=");
	reg = "> *=";
	sql_ = boost::regex_replace(sql_, reg, ">=");

	// split sql_statement_
	sql_vector_ = SplitSQL(sql_, " ");
}

/*���sql��䣬��ÿ�����ʷ���vector��*/
vector<string> QueryParser::SplitSQL(string sql, string seg)
{
	//int loc = sql.find(seg);
	//vector<string> arr;
	//while (loc != -1)
	//{
	//	string tmp = sql.substr(0, loc);
	//	arr.push_back(tmp);
	//	sql = sql.substr(loc + 1,strlen(sql.c_str())- 1 - loc);
	//	loc= sql.find(seg);
	//}
	//arr.push_back(sql);
	//return arr;

	char* cstr = const_cast<char*>(sql.c_str());
	char* current;
	vector<string> arr;
	current = strtok(cstr, seg.c_str());
	while (current != NULL)
	{
		arr.push_back(current);
		current = strtok(NULL, seg.c_str());
	}
	return arr;
}

/*��ȡsql��������*/
void  QueryParser::GetSqlType()
{
	if (sql_vector_.size() == 0)
	{
		sql_type_ = -1;
		cout << "sql�������δ֪" << endl;
		return;
	}
	boost::algorithm::to_lower(sql_vector_[0]);/*ʹsql��һ����Сд���ں����ж�*/
	if (sql_vector_[0] == "help")
	{
		sql_type_ = 11;
	}
	else if (sql_vector_[0] == "create")
	{
		if (sql_vector_.size() <= 1)
		{
			sql_type_ = -1;
			cout << "createʧ��,����������help�鿴��ȷcreate����" << endl;
			return;
		}
		boost::algorithm::to_lower(sql_vector_[1]);/*ʹsql�ڶ�����Сд���ں����ж�*/
		if (sql_vector_[1] == "database")/*sql�������Ϊ���½����ݿ� Code:21*/
		{
			sql_type_ = 21;
		}
		else if (sql_vector_[1] == "table")/*sql�������Ϊ���½����ݱ� Code:22*/
		{
			sql_type_ = 22;
		}
		else if (sql_vector_[1] == "index")/*sql�������Ϊ���½����� Code:23*/
		{
			sql_type_ = 23;
		}
		else
		{
			sql_type_ = -1;
			cout << "createʧ��,����������help�鿴��ȷcreate����" << endl;
			return;
		}
	}
	else if (sql_vector_[0] == "show")
	{
		if (sql_vector_.size() <= 1)
		{
			sql_type_ = -1;
			cout << "showʧ��,����������help�鿴��ȷshow����" << endl;
			return;
		}
		boost::algorithm::to_lower(sql_vector_[1]);/*ʹsql�ڶ�����Сд���ں����ж�*/
		if (sql_vector_[1] == "databases") /*sql�������Ϊ���鿴���ݿ� Code:31*/
		{
			sql_type_ = 31;
		}
		else if (sql_vector_[1] == "tables") /*sql�������Ϊ���鿴���ݱ� Code:32*/
		{
			sql_type_ = 32;
		}
		else
		{
			sql_type_ = -1;
			cout << "showʧ��,����������help�鿴��ȷshow����" << endl;
			return;
		}
	}
	else if (sql_vector_[0] == "drop")
	{
		if (sql_vector_.size() <= 1)
		{
			sql_type_ = -1;
			cout << "dropʧ��,����������help�鿴��ȷdrop����" << endl;
			return;
		}
		boost::algorithm::to_lower(sql_vector_[1]);/*ʹsql�ڶ�����Сд���ں����ж�*/
		if (sql_vector_[1] == "database") /*sql�������Ϊ��ɾ�����ݿ� Code:41*/
		{
			sql_type_ = 41;
		}
		else if (sql_vector_[1] == "table") /*sql�������Ϊ��ɾ�����ݱ� Code:42*/
		{
			sql_type_ = 42;
		}
		else if (sql_vector_[1] == "index") /*sql�������Ϊ��ɾ�����ݱ� Code:42*/
		{
			sql_type_ = 43;
		}
		else
		{
			sql_type_ = -1;
			cout << "dropʧ��,����������help�鿴��ȷdrop����" << endl;
			return;
		}
	}
	else if (sql_vector_[0] == "insert")  /*sql�������Ϊ���������� Code:51*/
	{
		sql_type_ = 51;
	}
	else if (sql_vector_[0] == "select")  /*sql�������Ϊ����ѯ���� Code:61*/
	{

		if (sql_vector_.size()>4 && sql_vector_[4] == "join")
		{
			sql_type_ = 62;
		}
		else
		{
			sql_type_ = 61;
		}
	}
	else if (sql_vector_[0] == "delete") /*sql�������Ϊ��ɾ������ Code:71*/
	{
		sql_type_ = 71;
	}
	else if (sql_vector_[0] == "update")  /*sql�������Ϊ���������� Code:81*/
	{
		sql_type_ = 81;
	}
	else if (sql_vector_[0] == "use")
	{
		sql_type_ = 91;
	}
	else
	{
		sql_type_ = -1;
		cout << "sql�������δ֪" << endl;
	}
}

/*����sql���͵��ö�Ӧ��api*/
void QueryParser::InvokeAPI()
{
	try
	{
		switch (sql_type_)
		{
		case -1:break;
		case 11:
		{
			api->help();
		}
		break;
		case 21:
		{
			SQLCreateDatabase *sd = new SQLCreateDatabase(sql_vector_);
			api->CreateDatabase(*sd);
			delete sd;
		}
		break;
		case 22:
		{
			SQLCreateTable *st = new SQLCreateTable(sql_vector_);
			api->CreateTable(*st);
			delete st;
		}
		break;
		case 23:
		{
			SQLCreateIndex *si = new SQLCreateIndex(sql_vector_);
			api->CreateIndex(*si);
			delete si;
		}
		break;
		case 31:
		{
			api->ShowDatabases();
		}
		break;
		case 32:
		{
			api->ShowTables();
		}
		break;
		case 41:
		{
			SQLDropDatabase *sdd = new SQLDropDatabase(sql_vector_);
			api->DropDatabase(*sdd);
			delete sdd;
		}
		break;
		case 42:
		{
			SQLDropTable *sdt = new SQLDropTable(sql_vector_);
			api->DropTable(*sdt);
			delete sdt;
		}
		break;
		case 43:
		{
			SQLDropIndex *sdi = new SQLDropIndex(sql_vector_);
			api->DropIndex(*sdi);
			delete sdi;
		}
		break;
		case 51:
		{
			SQLInsert *si = new SQLInsert(sql_vector_);
			api->Insert(*si);
			delete si;
		}
		break;
		case 61:
		{
			SQLSelect *ss = new SQLSelect(sql_vector_);
			api->Select(*ss);
			delete ss;
		}
		break;
		case 62:
		{
			SQLJoinSelect *sjs = new SQLJoinSelect(sql_vector_);
			api->JoinSelect(*sjs);
			delete sjs;
		}
		break;
		case 71:
		{
			SQLDelete *sd = new SQLDelete(sql_vector_);
			api->Delete(*sd);
			delete sd;
		}
		break;
		case 81:
		{
			SQLUpdate *su = new SQLUpdate(sql_vector_);
			api->Update(*su);
			delete su;
		}
		break;
		case 91:
		{
			SQLUse *suse = new SQLUse(sql_vector_);
			api->Use(*suse);
			delete suse;
		}
		break;
		default:
			break;
		}
	}
	catch (SyntaxErrorException& e) { cerr << "Error: �������!" << endl; }
	catch (NoDatabaseSelectedException& e) { cerr << "Error:δѡ����ǰ���ݿ⣡" << endl; }
	catch (DatabaseNotExistException& e) { cerr << "Error:���ݿⲻ����!" << endl; }
	catch (DatabaseAlreadyExistsException& e) { cerr << "Error:�����ݿ��Ѵ���!" << endl; }
	catch (TableNotExistException& e) { cerr << "Error:���ݱ�����!" << endl; }
	catch (TableAlreadyExistsException& e) { cerr << "Error:�����ݱ��Ѵ���!" << endl; }
	catch (IndexAlreadyExistsException& e) { cerr << "Error:�������Ѵ���!" << endl; }
	catch (IndexNotExistException& e) { cerr << "Error:����������!" << endl; }
	catch (OneIndexEachTableException& e) { cerr << "Error: ÿһ����ֻ����һ������!" << endl; }
	catch (BPlusTreeException& e) { cerr << "Error: B++������!" << endl; }
	catch (IndexMustBeCreatedOnPrimaryKeyException& e) { cerr << "Error: �������뽨����������!" << endl; }
	catch (PrimaryKeyConflictException& e) { cerr << "Error: ������ͻ!" << endl; }
}