#pragma region
//简介：查询解析器
//Implemented by Jin Xin
//作用：负责解析用户输入的查询语句并调用对应的API类里的函数
#pragma endregion

#include "QueryParser.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include<iostream>
using namespace std;

/*QueryParser构造函数*/
QueryParser::QueryParser()
{
	sql_type_ = -1;/*默认设置变量sql_type_为-1*/
	string path = boost::filesystem::initial_path<boost::filesystem::path>().string() + "/DATABASEData/"; /*获取当前文件exe设置地址*/
	if (!boost::filesystem::exists(path))/*判断文件是否存在，不存在就创建文件*/
	{
		boost::filesystem::create_directory(path);
	}
	api = new API(path);/*设置api变量*/
}

/*QueryParser的析构函数*/
QueryParser::~QueryParser(void)
{
	delete api;
}

/*对外接口，根据sql来执行相应的操作*/
void QueryParser::ExecuteSQL(string sql)
{
	try
	{
		sql_ = sql;/*获取用户输入的SQL语句*/
		FormateSQL();/*规范化用户输入的sql*/
		GetSqlType();/*获取SQL语句的类型*/
		InvokeAPI();/*根据类型调用对应的API函数*/
	}
	catch (exception e)
	{
		cout << e.what();
	}
}

/*规范化用户输入的sql，便于后续拆分*/
void QueryParser::FormateSQL()
{
	//boost::regex reg("[\r\n\t]");
	//sql_ = boost::regex_replace(sql_, reg, " ");/*换行、tab键用空白符" "代替*/
	//reg = ";.*$";
	//sql_ = boost::regex_replace(sql_, reg, "");/*去除分号之后的内容*/
	//reg = "(^ +)|( +$)";
	//sql_ = boost::regex_replace(sql_, reg, "");/*去除最前和最后的空白符*/
	//reg = " +";
	//sql_ = boost::regex_replace(sql_, reg, " ");/*去除重复的空白符*/
	//reg = " ?(\\(|\\)|,|=|(<>)|<|>) ?";
	//sql_ = boost::regex_replace(sql_, reg, " $1 ");
	//reg = "< *>";
	//sql_ = boost::regex_replace(sql_, reg, "<>");
	//reg = "< *=";
	//sql_ = boost::regex_replace(sql_, reg, "<=");
	//reg = "> *=";
	//sql_ = boost::regex_replace(sql_, reg, ">=");/*在( ) , = <> < >这些字符之前之后添加空白符" "*/

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

/*拆分sql语句，将每个单词放入vector中*/
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

/*获取sql语句的类型*/
void  QueryParser::GetSqlType()
{
	if (sql_vector_.size() == 0)
	{
		sql_type_ = -1;
		cout << "sql语句类型未知" << endl;
		return;
	}
	boost::algorithm::to_lower(sql_vector_[0]);/*使sql第一个词小写便于后续判断*/
	if (sql_vector_[0] == "help")
	{
		sql_type_ = 11;
	}
	else if (sql_vector_[0] == "create")
	{
		if (sql_vector_.size() <= 1)
		{
			sql_type_ = -1;
			cout << "create失败,请利用命令help查看正确create语义" << endl;
			return;
		}
		boost::algorithm::to_lower(sql_vector_[1]);/*使sql第二个词小写便于后续判断*/
		if (sql_vector_[1] == "database")/*sql语句类型为：新建数据库 Code:21*/
		{
			sql_type_ = 21;
		}
		else if (sql_vector_[1] == "table")/*sql语句类型为：新建数据表 Code:22*/
		{
			sql_type_ = 22;
		}
		else if (sql_vector_[1] == "index")/*sql语句类型为：新建索引 Code:23*/
		{
			sql_type_ = 23;
		}
		else
		{
			sql_type_ = -1;
			cout << "create失败,请利用命令help查看正确create语义" << endl;
			return;
		}
	}
	else if (sql_vector_[0] == "show")
	{
		if (sql_vector_.size() <= 1)
		{
			sql_type_ = -1;
			cout << "show失败,请利用命令help查看正确show语义" << endl;
			return;
		}
		boost::algorithm::to_lower(sql_vector_[1]);/*使sql第二个词小写便于后续判断*/
		if (sql_vector_[1] == "databases") /*sql语句类型为：查看数据库 Code:31*/
		{
			sql_type_ = 31;
		}
		else if (sql_vector_[1] == "tables") /*sql语句类型为：查看数据表 Code:32*/
		{
			sql_type_ = 32;
		}
		else
		{
			sql_type_ = -1;
			cout << "show失败,请利用命令help查看正确show语义" << endl;
			return;
		}
	}
	else if (sql_vector_[0] == "drop")
	{
		if (sql_vector_.size() <= 1)
		{
			sql_type_ = -1;
			cout << "drop失败,请利用命令help查看正确drop语义" << endl;
			return;
		}
		boost::algorithm::to_lower(sql_vector_[1]);/*使sql第二个词小写便于后续判断*/
		if (sql_vector_[1] == "database") /*sql语句类型为：删除数据库 Code:41*/
		{
			sql_type_ = 41;
		}
		else if (sql_vector_[1] == "table") /*sql语句类型为：删除数据表 Code:42*/
		{
			sql_type_ = 42;
		}
		else if (sql_vector_[1] == "index") /*sql语句类型为：删除数据表 Code:42*/
		{
			sql_type_ = 43;
		}
		else
		{
			sql_type_ = -1;
			cout << "drop失败,请利用命令help查看正确drop语义" << endl;
			return;
		}
	}
	else if (sql_vector_[0] == "insert")  /*sql语句类型为：插入数据 Code:51*/
	{
		sql_type_ = 51;
	}
	else if (sql_vector_[0] == "select")  /*sql语句类型为：查询数据 Code:61*/
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
	else if (sql_vector_[0] == "delete") /*sql语句类型为：删除数据 Code:71*/
	{
		sql_type_ = 71;
	}
	else if (sql_vector_[0] == "update")  /*sql语句类型为：更新数据 Code:81*/
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
		cout << "sql语句类型未知" << endl;
	}
}

/*根据sql类型调用对应的api*/
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
	catch (SyntaxErrorException& e) { cerr << "Error: 语义错误!" << endl; }
	catch (NoDatabaseSelectedException& e) { cerr << "Error:未选定当前数据库！" << endl; }
	catch (DatabaseNotExistException& e) { cerr << "Error:数据库不存在!" << endl; }
	catch (DatabaseAlreadyExistsException& e) { cerr << "Error:该数据库已存在!" << endl; }
	catch (TableNotExistException& e) { cerr << "Error:数据表不存在!" << endl; }
	catch (TableAlreadyExistsException& e) { cerr << "Error:该数据表已存在!" << endl; }
	catch (IndexAlreadyExistsException& e) { cerr << "Error:该索引已存在!" << endl; }
	catch (IndexNotExistException& e) { cerr << "Error:索引不存在!" << endl; }
	catch (OneIndexEachTableException& e) { cerr << "Error: 每一个表只能由一个索引!" << endl; }
	catch (BPlusTreeException& e) { cerr << "Error: B++树错误!" << endl; }
	catch (IndexMustBeCreatedOnPrimaryKeyException& e) { cerr << "Error: 索引必须建立在主键上!" << endl; }
	catch (PrimaryKeyConflictException& e) { cerr << "Error: 主键冲突!" << endl; }
}