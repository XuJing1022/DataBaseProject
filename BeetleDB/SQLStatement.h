//Implemented by Jin Xin
#pragma once
#ifndef _SQLSTATEMENT_H_
#define _SQLSTATEMENT_H_
#include"CatalogManager.h"
#include"Exceptions.h"
#include"ConstValue.h"
#include <vector>
#include <string>
using namespace std;

class CatalogManager;
class Database;
class Table;
class Attribute;
class Index;

#pragma region base class:SQL 
class SQL
{
public:
	SQL();/*sql不带参数的构造函数*/
	SQL(int sql_type);/*sql带参数int sql_type的构造函数*/
	int get_sql_type();/*获取sql语句的类型*/
	void set_sql_type(int sql_type);/*设置sql语句的类型*/
	int ParseDataType(vector<string> sql_vector, Attribute &attribute, unsigned int pos);/*解析SQL语句中字段的数据类型*/
	virtual void Parse(vector<string> sql_vector) = 0;/*虚函数要求继承该类的类进行重写*/
protected:
	int sql_type_;//sql语句的类型
};
#pragma endregion

#pragma region class: SQLCreateDatabase 例如：create database university;
class SQLCreateDatabase : public SQL
{
public:
	SQLCreateDatabase(vector<string> sql_vector);/*SQLCreateDatabase的构造函数*/
	string get_database_name();/*获取database的名字*/
	void set_database_name(string database_name);/*设置database的名字*/
	void Parse(vector<string> sql_vector);/*解析sql获取database的名字*/
private:
	string database_name_;//database的名字
};
#pragma endregion

#pragma region class SQLCreateTable 例如：create table student (name char(100), id int, primary key(id));
class SQLCreateTable : public SQL
{
public:
	SQLCreateTable(vector<string> sql_vector);/*SQLCreateTable的构造函数*/
	string get_table_name();/*获取table的名字*/
	void set_table_name(string table_name);/*设置table的名字*/
	vector<Attribute> get_attributes();/*获取table的属性*/
	void SetAttributes(vector<Attribute> attribute);/*设置table的属性*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字、table属性*/
private:
	string table_name_;//table的名字
	vector<Attribute> attributes_;//table的属性
};
#pragma endregion

#pragma region class SQLCreateIndex 例如：create index i1 on student(id); 
class SQLCreateIndex : public SQL
{
public:
	SQLCreateIndex(vector<string> sql_vector);/*SQLCreateIndex的构造函数*/
	string get_tb_name();/*获取table的名字*/
	string get_index_name();/*获取索引的名字*/
	string get_column_name();/*获取建立索引字段的名字*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字、索引的名字、建立索引字段的名字*/
private:
	string index_name_;//索引的名字
	string table_name_;//table的名字
	string col_name_;//建立索引字段的名字
};
#pragma endregion

#pragma region class SQLDropDatabase 例如：drop database university;
class SQLDropDatabase : public SQL
{
public:
	SQLDropDatabase(vector<string> sql_vector);/*SQLDropDatabase的构造函数*/
	string get_database_name();/*获取数据库的名字*/
	void set_database_name(string dbname);/*设置数据库的名字*/
	void Parse(vector<string> sql_vector);/*解析sql获取数据库名字*/
private:
	string database_name_;//数据库名字
};
#pragma endregion

#pragma region class SQLDropTable 例如：drop table student;
class SQLDropTable : public SQL
{
public:
	SQLDropTable(vector<string> sql_vector);/*SQLDropTable的构造函数*/
	string get_table_name();/*获取table的名字*/
	void set_table_name(string table_name);/*设置table的名字*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字*/
private:
	string table_name_;//table的名字
};
#pragma endregion

#pragma region class SQLDropIndex 例如：drop index i1;
class SQLDropIndex : public SQL
{
public:
	SQLDropIndex(vector<string> sql_vector);/*SQLDropIndex的构造函数*/
	string get_index_name();/*获取index的名字*/
	void set_index_name(string  index_name);/*设置index的名字*/
	void Parse(vector<string> sql_vector);/*解析sql获取index的名字*/
private:
	string index_name_;//索引的名字
};
#pragma endregion

#pragma region class SQLUse 例如：use university：
class SQLUse : public SQL
{
public:
	SQLUse(vector<string> sql_vector);/*SQLUse的构造函数*/
	string get_database_name();/*获取数据库的名字*/
	void set_database_name(string database_name);/*设置数据库的名字*/
	void Parse(vector<string> sql_vector);/*解析sql获取数据库的名字*/
private:
	string database_name_;//数据库的名字
};
#pragma endregion

#pragma region struct SQLValue 例如：name|Tom 包括字段名和字段值
typedef	struct
{
	int data_type;
	string value;
}SQLValue;
#pragma endregion

#pragma region struct SQLWhere 例如 where name='Tom'
typedef struct
{
	int op_type;
	string key;
	string value;
}SQLWhere;
#pragma endregion

#pragma region struct SQLKeyValue 例如 set name='Tom'
typedef struct
{
	string key;
	string value;
}SQLKeyValue;
#pragma endregion


#pragma region class SQLInsert 例如： insert into student values('Tom',1);
class SQLInsert : public SQL
{
public:
	SQLInsert(vector<string> sql_vector);/*SQLInsert的构造函数*/
	string get_tb_name();/*获取table的名字*/
	vector<SQLValue>& GetValues();  /*获取SQLValue的值*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字和SQLValue的值*/
private:
	string table_name_;//table 的名字
	vector<SQLValue> values_;//SQLValue的值 value的值
};
#pragma endregion

#pragma region class SQLSelect 例如：select name from student where name = 'Tom';
class SQLSelect : public SQL
{
public:
	SQLSelect(vector<string> sql_vector);/*SQLSelect的构造函数*/
	string get_tb_name();/*获取table的名字*/
	vector<SQLWhere>& GetWheres();/*获取SQLWhere的值即查询条件*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字和SQLWhere的值*/
	vector<string>&  get_select_attribute();/*获取变量select_attribute_*/
private:
	string table_name_;//table的名字
	vector<SQLWhere> wheres_;//SQLWhere的值即where查询条件
	vector<string> select_attribute_;//存储要查询的字段的变量
};
#pragma endregion

#pragma region class SQLJoinSelect 例如：select name from student join takes where name ='Tom';
class SQLJoinSelect :public SQL
{
public:
	SQLJoinSelect(vector<string>sql_vector);
	vector<string> get_table_names();
	vector<SQLWhere>& get_wheres();
	void Parse(vector<string>sql_vector);
private:
	vector<string> table_names_;
	vector<SQLWhere> wheres_;
};
#pragma endregion

#pragma region class SQLDelete 例如：delete from student where name='Tom';
class SQLDelete : public SQL
{
public:
	SQLDelete(vector<string> sql_vector);/*SQLDelete的构造函数*/
	string get_tb_name();/*获取table的名字*/
	vector<SQLWhere>& GetWheres();/*获取SQLWhere的值即查询条件*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字和SQLWhere的值*/
private:
	string table_name_;//table的名字
	vector<SQLWhere> wheres_;//SQLWhere的值即where查询条件
};
#pragma endregion

#pragma region class SQLUpdate 例如：update student set name ='Tim' where name='Tom';
class SQLUpdate : public SQL
{
public:
	SQLUpdate(vector<string> sql_vector);/*SQLUpdate的构造函数*/
	string get_tb_name();/*获取table的名字*/
	vector<SQLWhere>& GetWheres();/*获取SQLWhere的值即查询条件*/
	vector<SQLKeyValue>& GetKeyValues();/*获取SQLKeyValue的值即更新字段*/
	void Parse(vector<string> sql_vector);/*解析sql获取table的名字和SQLWhere的值和SQLKeyValue的值*/
private:
	string table_name_;//table的名字
	vector<SQLWhere> wheres_;//SQLWhere的值即where查询条件
	vector<SQLKeyValue> keyvalues_;//SQLKeyValue的值即set的字段与值
};
#pragma endregion

#endif // !_SQLSTATEMENT_H_

class TKey
{
public:
	TKey(int keytype, int length);
	TKey(const TKey& t1);
	~TKey();
	void ReadValue(const char *content);
	void ReadValue(string content);

	int get_key_type();
	char* get_key();
	int get_length();

	friend std::ostream & operator<<(std::ostream& out, const TKey& object);
	bool operator<(const TKey t1);
	bool operator>(const TKey t1);
	bool operator<=(const TKey t1);
	bool operator>=(const TKey t1);
	bool operator==(const TKey t1);
	bool operator!=(const TKey t1);

	//xj0616 FOR aggregation
	TKey* operator+=(const TKey t1);
	TKey* operator/=(const TKey t1);
	TKey* TKey::operator/=(const int t1);

private:
	int key_type_;
	char *key_;
	int length_;
};
