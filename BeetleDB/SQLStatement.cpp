//Implemented by Jin Xin
#include"SQLStatement.h"

#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>
using namespace std;

#pragma region class 实现：SQL
/*sql不带参数的构造函数*/
SQL::SQL()
{
	sql_type_ = -1;
}

/*sql带参数int sql_type的构造函数*/
SQL::SQL(int sql_type)
{
	sql_type_ = sql_type;
}

/*获取sql语句的类型*/
int SQL::get_sql_type()
{
	return sql_type_;
}

/*设置sql语句的类型*/
void SQL::set_sql_type(int sql_type)
{
	sql_type_ = sql_type;
}

/*解析SQL语句中字段的数据类型*/
int SQL::ParseDataType(vector<string> sql_vector, Attribute &attr, unsigned int pos)
{
	boost::algorithm::to_lower(sql_vector[pos]);/*获取小写便于后续判断*/
	if (sql_vector[pos] == "int")/*如果是int类型，设置attr的数据类型为T_INT,attr字段长度为4*/
	{
		attr.set_data_type(T_INT);
		attr.set_length(4);
		pos++;
		if (sql_vector[pos] == ",") pos++;
	}
	else if (sql_vector[pos] == "float")/*如果是float类型，设置attr的数据类型为T_FLOAT,attr字段长度为4*/
	{
		attr.set_data_type(T_FLOAT);
		attr.set_length(4);
		pos++;
		if (sql_vector[pos] == ",") pos++;
	}
	else if (sql_vector[pos] == "char" || sql_vector[pos] == "varchar")/*如果是char或者varchar类型，设置attr的数据类型为T_CHAR,attr字段长度为用户设置长度*/
	{
		attr.set_data_type(T_CHAR);
		pos++;
		if (sql_vector[pos] == "(") pos++;
		attr.set_length(atoi(sql_vector[pos].c_str()));
		pos++;
		if (sql_vector[pos] == ")") pos++;
		if (sql_vector[pos] == ",") pos++;
	}
	else throw SyntaxErrorException();
	return pos;
}
#pragma endregion

#pragma region class 实现：SQLCreateDatabase
/*SQLCreateDatabase的构造函数*/
SQLCreateDatabase::SQLCreateDatabase(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取database的名字*/
string SQLCreateDatabase::get_database_name()
{
	return database_name_;
}

/*设置database的名字*/
void SQLCreateDatabase::set_database_name(string dbname)
{
	database_name_ = dbname;
}

/*解析sql获取database的名字*/
void SQLCreateDatabase::Parse(vector<string> sql_vector)
{
	sql_type_ = 21;
	if (sql_vector.size() <= 2) throw SyntaxErrorException();
	else
	{
		database_name_ = sql_vector[2];
	}
}
#pragma endregion	

#pragma region class 实现：SQLCreateTable
/*SQLCreateTable的构造函数*/
SQLCreateTable::SQLCreateTable(vector<string> sql_vector)
{
	Parse(sql_vector);
}
/*获取table的名字*/
string SQLCreateTable::get_table_name()
{
	return table_name_;
}
/*设置table的名字*/
void SQLCreateTable::set_table_name(string dbname)
{
	table_name_ = dbname;
}
/*获取table的属性*/
vector<Attribute> SQLCreateTable::get_attributes()
{
	return attributes_;
}
/*设置table的属性*/
void SQLCreateTable::SetAttributes(vector<Attribute> attr)
{
	attributes_ = attr;
}
/*解析sql获取table的名字、table属性。例如create table student (name char(100), id int, primary key(id));*/
void SQLCreateTable::Parse(vector<string> sql_vector)
{
	sql_type_ = 22;
	unsigned int pos = 2;
	if (sql_vector.size() <= pos) throw SyntaxErrorException();/*如果只有create table 语义错误*/

	table_name_ = sql_vector[pos];/*获取table name*/
	pos++;

	if (sql_vector[pos] != "(") throw SyntaxErrorException();
	pos++;

	bool if_attr = true;
	bool if_primary_key = false;

	while (if_attr)
	{
		if_attr = false;
		if (sql_vector[pos] != "primary")/*name char(100)*/
		{
			Attribute attr;
			attr.set_attribute_name(sql_vector[pos]);/*获取字段名称*/
			pos++;
			pos = ParseDataType(sql_vector, attr, pos);/*获取字段的数据类型*/
			attributes_.push_back(attr);/*向字段vector中添加字段信息*/
			if (sql_vector[pos] != ")") if_attr = true;/*如果这不是最后一个字段*/
		}
		else/* primary key(id))*/
		{
			if (if_primary_key) throw SyntaxErrorException();/*如果已经有主键返回错误*/
			pos++;
			if (sql_vector[pos] != "key") throw SyntaxErrorException();/*如果格式不为 primary key则返回错误*/
			pos++;
			if (sql_vector[pos] != "(") throw SyntaxErrorException();/*如果格式不为 primary key( 则返回错误*/
			pos++;
			bool if_arrt_exist = false;
			for (auto att = attributes_.begin(); att != attributes_.end(); att++)/*获取之前解析的所有字段，查找是否有primary key中的该id字段*/
			{
				if ((*att).get_attr_name() == sql_vector[pos])
				{
					(*att).set_attribute_type(1);
					if_arrt_exist = true;
				}
			}
			if (if_arrt_exist == false) throw SyntaxErrorException();/*如果primary key 的字段在之前解析的字段中不存在则返回错误*/
			pos++;
			if (sql_vector[pos] != ")") throw SyntaxErrorException();/*如果格式不为 primary key(id) 则返回错误*/
			if_primary_key = true;
		}
	}
}
#pragma endregion

#pragma region class 实现：SQLCreateIndex
/*SQLCreateIndex的构造函数*/
SQLCreateIndex::SQLCreateIndex(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
string SQLCreateIndex::get_tb_name()
{
	return table_name_;
}

/*获取索引的名字*/
string SQLCreateIndex::get_index_name()
{
	return index_name_;
}

/*获取建立索引字段的名字*/
string SQLCreateIndex::get_column_name()
{
	return col_name_;
}

/*解析sql获取table的名字、索引的名字、建立索引字段的名字 例如：create index i1 on student(id); */
void SQLCreateIndex::Parse(vector<string> sql_vector)
{
	sql_type_ = 23;
	unsigned int pos = 2;
	if (sql_vector.size() <= pos) throw SyntaxErrorException();/*如果sql类型为 create index则返回语义错误*/

	index_name_ = sql_vector[pos];/*获取index的名字*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "on") throw SyntaxErrorException();/*如果sql不为create index i1 on 则返回错误*/
	pos++;

	table_name_ = sql_vector[pos];/*获取table 的名字*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "(") throw SyntaxErrorException();/*如果sql不为create index i1 on t1( 则返回错误*/
	pos++;

	col_name_ = sql_vector[pos];/*获取建立索引的字段名*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != ")") throw SyntaxErrorException();/*如果sql不为create index i1 on t1(id) 则返回错误*/
	pos++;
}
#pragma endregion

#pragma region class 实现：SQLDropDatabase
/*SQLDropDatabase的构造函数*/
SQLDropDatabase::SQLDropDatabase(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取数据库的名字*/
string SQLDropDatabase::get_database_name()
{
	return database_name_;
}

/*设置数据库的名字*/
void SQLDropDatabase::set_database_name(string dbname)
{
	database_name_ = dbname;
}

/*解析sql获取数据库名字 例如：drop database university;*/
void SQLDropDatabase::Parse(vector<string> sql_vector)
{
	sql_type_ = 41;
	if (sql_vector.size() <= 2 || sql_vector.size()>3) throw SyntaxErrorException();/*如果sql语句不为drop database university返回错误*/
	else
	{
		database_name_ = sql_vector[2];/*获取database的名字*/
	}
}
#pragma endregion

#pragma region class 实现：SQLDropTable
/*SQLDropTable的构造函数*/
SQLDropTable::SQLDropTable(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
string SQLDropTable::get_table_name()
{
	return table_name_;
}

/*设置table的名字*/
void SQLDropTable::set_table_name(string tbname)
{
	table_name_ = tbname;
}

/*解析sql获取table的名字 例如：drop table student;*/
void SQLDropTable::Parse(vector<string> sql_vector)
{
	sql_type_ = 42;
	if (sql_vector.size() <= 2 || sql_vector.size()>3) throw SyntaxErrorException();/*如果sql语句不为drop table student;返回错误*/
	else table_name_ = sql_vector[2];
}
#pragma endregion

#pragma region class 实现：SQLDropIndex
/*SQLDropIndex的构造函数*/
SQLDropIndex::SQLDropIndex(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取index的名字*/
string SQLDropIndex::get_index_name()
{
	return index_name_;
}

/*设置index的名字*/
void SQLDropIndex::set_index_name(string idxname)
{
	index_name_ = idxname;
}

/*解析sql获取index的名字 drop index i1;*/
void SQLDropIndex::Parse(vector<string> sql_vector)
{
	sql_type_ = 52;
	if (sql_vector.size() <= 2 || sql_vector.size()>3) throw SyntaxErrorException();/*如果sql语句不为 drop index i1;返回错误*/
	else index_name_ = sql_vector[2];
}
#pragma endregion

#pragma region class 实现：SQLUse
/*SQLUse的构造函数*/
SQLUse::SQLUse(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取数据库的名字*/
string SQLUse::get_database_name()
{
	return database_name_;
}

/*设置数据库的名字*/
void SQLUse::set_database_name(string dbname)
{
	database_name_ = dbname;
}

/*解析sql获取数据库的名字use university：*/
void SQLUse::Parse(vector<string> sql_vector)
{
	sql_type_ = 91;
	if (sql_vector.size() <= 1 || sql_vector.size()>2) throw SyntaxErrorException();/*如果sql类型不为use university;则返回错误*/
	else
	{
		database_name_ = sql_vector[1];
	}
}
#pragma endregion

#pragma region class 实现：SQLInsert
/*SQLInsert的构造函数*/
SQLInsert::SQLInsert(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
string SQLInsert::get_tb_name()
{
	return table_name_;
}

/*获取SQLValue的值*/
vector<SQLValue>& SQLInsert::GetValues()
{
	return values_;
}

/*解析sql获取table的名字和SQLValue的值  insert into student values('Tom',1);*/
void SQLInsert::Parse(vector<string> sql_vector)
{
	sql_type_ = 51;
	unsigned int pos = 1;
	bool if_attr = true;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "into") throw SyntaxErrorException();/*如果sql不为 insert into 则返回错误*/
	pos++;

	table_name_ = sql_vector[pos];/*获取插入的表的名称*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "values") throw SyntaxErrorException();/*如果sql 不为 insert into student values则返回错误*/
	pos++;

	if (sql_vector[pos] != "(") throw SyntaxErrorException();/*如果sql 不为 insert into student values(则返回错误*/
	pos++;

	while (if_attr)/*'Tom',1*/
	{
		if_attr = false;
		SQLValue sql_value;
		string val = sql_vector[pos];
		if (val[0] == '\'' || val[0] == '\"')/*'Tom'*/
		{
			val.assign(val, 1, val.length() - 2);/*用Tom赋值给val*/
			sql_value.data_type = 2;
		}
		else
		{
			if (val.find('.') == string::npos)/*2.3*/
			{
				sql_value.data_type = 0;
			}
			else
			{
				sql_value.data_type = 1;/*2*/
			}
		}
		sql_value.value = val;
		values_.push_back(sql_value);
		pos++;
		if (sql_vector[pos] != ")")/*后面还有value*/
		{
			if_attr = true;
		}
		pos++;
	}
}
#pragma endregion

#pragma region class 实现：SQLSelect
/*SQLSelect的构造函数*/
SQLSelect::SQLSelect(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
string SQLSelect::get_tb_name()
{
	return table_name_;
}

/*获取SQLWhere的值即查询条件*/
vector<SQLWhere>& SQLSelect::GetWheres()
{
	return wheres_;
}

/*获取变量select_attribute_*/
vector<string>& SQLSelect::get_select_attribute()
{
	return select_attribute_;
}

/*解析sql获取table的名字和SQLWhere的值 select * from student where name = 'Tom';暂时只支持select**/
void SQLSelect::Parse(vector<string> sql_vector)
{
	sql_type_ = 61;
	unsigned int pos = 1;
	if (sql_vector.size() <= 1) throw SyntaxErrorException();/*select*/
	if (sql_vector[pos] == "*")
	{
		select_attribute_.push_back(sql_vector[pos]);
		pos++;
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "from") throw SyntaxErrorException();/*select * from*/
		pos++;
	}
	else
	{
		while (boost::algorithm::to_lower_copy(sql_vector[pos]) != "from")
		{
			if (sql_vector.size() == pos)
			{
				throw SyntaxErrorException();
			}
			select_attribute_.push_back(sql_vector[pos]);
			pos++;
			if (sql_vector[pos] == ",")
			{
				pos++;
			}
			else if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "from")
			{
				throw SyntaxErrorException();
			}
		}

		pos++;
	}

	table_name_ = sql_vector[pos];/*获取table名字*/
	pos++;

	if (sql_vector.size() == pos) return;
	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "where") throw SyntaxErrorException();/*select * from student where*/
	pos++;

	while (true)
	{
		SQLWhere sql_where;
		sql_where.key = sql_vector[pos];
		pos++;

		if (sql_vector[pos] == "<") sql_where.op_type = SIGN_LT;
		else if (sql_vector[pos] == "=") sql_where.op_type = SIGN_EQ;
		else if (sql_vector[pos] == ">") sql_where.op_type = SIGN_GT;
		else if (sql_vector[pos] == "<=") sql_where.op_type = SIGN_LE;
		else if (sql_vector[pos] == ">=") sql_where.op_type = SIGN_GE;
		else if (sql_vector[pos] == "<>") sql_where.op_type = SIGN_NE;
		pos++;

		sql_where.value = sql_vector[pos];
		if (sql_where.value[0] == '\'' || sql_where.value[0] == '\"') sql_where.value.assign(sql_where.value, 1, sql_where.value.length() - 2);
		wheres_.push_back(sql_where);
		pos++;

		if (sql_vector.size() == pos) break; /*where 语句到此结束*/
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*select * from student where id=1 and name ='Tom'*/
		pos++;
	}
}
#pragma endregion

#pragma region  class 实现：SQLJoinSelect
SQLJoinSelect::SQLJoinSelect(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
vector<string>  SQLJoinSelect::get_table_names()
{
	return table_names_;
}

/*获取SQLWhere的值即查询条件*/
vector<SQLWhere>& SQLJoinSelect::get_wheres()
{
	return wheres_;
}
/*解析sql获取table的名字和SQLWhere的值 select * from student join study where name = 'Tom';暂时只支持select**/
void SQLJoinSelect::Parse(vector<string> sql_vector)
{
	sql_type_ = 62;
	unsigned int pos = 1;
	if (sql_vector.size() <= 1) throw SyntaxErrorException();/*select*/
	if (sql_vector[pos] != "*") throw SyntaxErrorException();/*select */
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "from") throw SyntaxErrorException();/*select * from*/
	pos++;

	int table_count = 0;
	while (sql_vector.size() != pos&&boost::algorithm::to_lower_copy(sql_vector[pos]) != "where")
	{
		table_names_[table_count++] = sql_vector[pos];/*获取table名字*/
		pos++;
		if (sql_vector.size() != pos&&sql_vector[pos] != "where")
		{
			if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "join")
			{
				throw SyntaxErrorException();
			}
			else
			{
				pos++;
			}
		}
	}


	if (sql_vector.size() == pos) return; /*select * from student; */
	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "where") throw SyntaxErrorException();/*select * from student where*/
	pos++;

	while (true)
	{
		SQLWhere sql_where;
		sql_where.key = sql_vector[pos];
		pos++;

		if (sql_vector[pos] == "<") sql_where.op_type = SIGN_LT;
		else if (sql_vector[pos] == "=") sql_where.op_type = SIGN_EQ;
		else if (sql_vector[pos] == ">") sql_where.op_type = SIGN_GT;
		else if (sql_vector[pos] == "<=") sql_where.op_type = SIGN_LE;
		else if (sql_vector[pos] == ">=") sql_where.op_type = SIGN_GE;
		else if (sql_vector[pos] == "<>") sql_where.op_type = SIGN_NE;
		pos++;

		sql_where.value = sql_vector[pos];
		if (sql_where.value[0] == '\'' || sql_where.value[0] == '\"') sql_where.value.assign(sql_where.value, 1, sql_where.value.length() - 2);
		wheres_.push_back(sql_where);
		pos++;

		if (sql_vector.size() == pos) break; /*where 语句到此结束*/
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*select * from student where id=1 and name ='Tom'*/
		pos++;
	}
}
#pragma endregion

#pragma region class 实现：SQLDelete
/*SQLDelete的构造函数*/
SQLDelete::SQLDelete(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
string SQLDelete::get_tb_name()
{
	return table_name_;
}

/*获取SQLWhere的值即查询条件*/
vector<SQLWhere>& SQLDelete::GetWheres()
{
	return wheres_;
}

/*解析sql获取table的名字和SQLWhere的值 delete from student where name='Tom';*/
void SQLDelete::Parse(vector<string> sql_vector)
{
	sql_type_ = 71;
	unsigned int pos = 1;
	if (sql_vector.size() <= 1) throw SyntaxErrorException();/*delete*/
	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "from") throw SyntaxErrorException();/*delete from*/
	pos++;

	table_name_ = sql_vector[pos];/*获取table的名字*/
	pos++;

	if (sql_vector.size() == pos) return; /*delete from student;即把改变数据全部删除*/
	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "where") throw SyntaxErrorException();/*delete from studetn where*/
	pos++;

	while (true)
	{
		SQLWhere sql_where;
		sql_where.key = sql_vector[pos];
		pos++;

		if (sql_vector[pos] == "<") sql_where.op_type = SIGN_LT;
		else if (sql_vector[pos] == "=") sql_where.op_type = SIGN_EQ;
		else if (sql_vector[pos] == ">") sql_where.op_type = SIGN_GT;
		else if (sql_vector[pos] == "<=") sql_where.op_type = SIGN_LE;
		else if (sql_vector[pos] == ">=") sql_where.op_type = SIGN_GE;
		else if (sql_vector[pos] == "<>") sql_where.op_type = SIGN_NE;
		pos++;

		sql_where.value = sql_vector[pos];
		if (sql_where.value[0] == '\'' || sql_where.value[0] == '\"') sql_where.value.assign(sql_where.value, 1, sql_where.value.length() - 2);
		wheres_.push_back(sql_where);
		pos++;

		if (sql_vector.size() == pos) break; /*where 条件到此结束 */
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*delete  from student where id=1 and name ='Tom'*/
		pos++;
	}

}
#pragma endregion

#pragma region class 实现：SQLUpdate
/*SQLUpdate的构造函数*/
SQLUpdate::SQLUpdate(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*获取table的名字*/
string SQLUpdate::get_tb_name()
{
	return table_name_;
}

/*获取SQLWhere的值即查询条件*/
vector<SQLWhere>& SQLUpdate::GetWheres()
{
	return wheres_;
}

/*获取SQLKeyValue的值即更新字段*/
vector<SQLKeyValue>& SQLUpdate::GetKeyValues()
{
	return keyvalues_;
}

/*解析sql获取table的名字和SQLWhere的值和SQLKeyValue的值 update student set name ='Tim' where name='Tom';*/
void SQLUpdate::Parse(vector<string> sql_vector)
{
	sql_type_ = 81;
	unsigned int pos = 1;
	if (sql_vector.size() <= 1) throw SyntaxErrorException();/*update*/

	table_name_ = sql_vector[pos];/*获取table的名字*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "set") throw SyntaxErrorException();/*update student set*/
	pos++;

	while (true)/*获取set字段与对应字段值*/
	{
		SQLKeyValue sql_kv;
		sql_kv.key = sql_vector[pos];/*获取set字段*/
		pos++;

		if (sql_vector[pos] != "=") throw SyntaxErrorException();/*set name=*/
		pos++;

		sql_kv.value = sql_vector[pos];/*获取对应字段值*/
		if (sql_kv.value[0] == '\'' || sql_kv.value[0] == '\"') sql_kv.value.assign(sql_kv.value, 1, sql_kv.value.length() - 2);/*'Tom'变为Tom*/
		keyvalues_.push_back(sql_kv);
		pos++;

		if (sql_vector[pos] == ",") pos++;/*如果有第二个set字段则继续*/
		else if (boost::algorithm::to_lower_copy(sql_vector[pos]) == "where") break;/*如果后面是where字段则跳出循环开始获取where字段值*/
		else throw SyntaxErrorException();/*语义错误*/
	}
	if (sql_vector[pos] != "where") throw SyntaxErrorException();/*update student set name = 'Tim' where*/
	pos++;

	while (true)/*获取where字段值*/
	{
		SQLWhere sql_where;
		sql_where.key = sql_vector[pos];
		pos++;

		if (sql_vector[pos] == "<") sql_where.op_type = SIGN_LT;
		else if (sql_vector[pos] == "=") sql_where.op_type = SIGN_EQ;
		else if (sql_vector[pos] == ">") sql_where.op_type = SIGN_GT;
		else if (sql_vector[pos] == "<=") sql_where.op_type = SIGN_LE;
		else if (sql_vector[pos] == ">=") sql_where.op_type = SIGN_GE;
		else if (sql_vector[pos] == "<>") sql_where.op_type = SIGN_NE;
		pos++;

		sql_where.value = sql_vector[pos];
		if (sql_where.value[0] == '\'' || sql_where.value[0] == '\"') sql_where.value.assign(sql_where.value, 1, sql_where.value.length() - 2);
		wheres_.push_back(sql_where);
		pos++;

		if (sql_vector.size() == pos) break; /*where字段到此结束 */
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*update  student set name='Tim' where id=1 and name ='Tom'*/
		pos++;
	}
}
#pragma endregion

#pragma region class 实现：TKey
/* ----------------- TKey -------------------- */
TKey::TKey(int keytype, int length)
{
	key_type_ = keytype;
	if (keytype == 2) length_ = length; //char(100)
	else length_ = 4;
	key_ = new char[length_];
}

TKey::TKey(const TKey& t1)
{
	key_type_ = t1.key_type_;
	length_ = t1.length_;
	key_ = new char[length_];
	memcpy(key_, t1.key_, length_);
}

TKey::~TKey()
{
	if (key_ != NULL)
		delete[]key_;
}

void TKey::ReadValue(const char *content)
{
	switch (key_type_)
	{
	case 0:
	{
		int a = atoi(content);
		memcpy(key_, &a, length_);
	}
	break;
	case 1:
	{
		float b = atof(content);
		memcpy(key_, &b, length_);
	}
	break;
	case 2:
		memcpy(key_, content, length_);
		break;
	}
}

void TKey::ReadValue(string content)
{
	switch (key_type_)
	{
	case 0:
	{
		int a = atoi(content.c_str());
		memcpy(key_, &a, length_);
	}
	break;
	case 1:
	{
		float b = atof(content.c_str());
		memcpy(key_, &b, length_);
	}
	break;
	case 2:
		memcpy(key_, content.c_str(), length_);
		break;
	}
}

int TKey::get_key_type() { return key_type_; }
char* TKey::get_key() { return key_; }
int TKey::get_length() { return length_; }

std::ostream & operator<<(std::ostream& out, const TKey& object)
{
	switch (object.key_type_)
	{
	case 0:
	{
		int a;
		memcpy(&a, object.key_, object.length_);
		cout << setw(9) << left << a;
	}
	break;
	case 1:
	{
		float a;
		memcpy(&a, object.key_, object.length_);
		cout << setw(9) << left << a;
	}
	break;
	case 2:
	{
		cout << setw(9) << left << object.key_;
	}
	break;
	}
	return out;
}

bool TKey::operator<(const TKey t1)
{
	switch (t1.key_type_)
	{
	case 0:
		return *(int*)key_ < *(int*)t1.key_;
	case 1:
		return *(float*)key_ < *(float*)t1.key_;
	case 2:
		return (strncmp(key_, t1.key_, length_) < 0);
	default:
		return false;
	}
}

bool TKey::operator>(const TKey t1)
{
	switch (t1.key_type_)
	{
	case 0:
		return *(int*)key_ > *(int*)t1.key_;
	case 1:
		return *(float*)key_ > *(float*)t1.key_;
	case 2:
		return (strncmp(key_, t1.key_, length_) > 0);
	default:
		return false;
	}
}

bool TKey::operator==(const TKey t1)
{
	switch (t1.key_type_)
	{
	case 0:
		return *(int*)key_ == *(int*)t1.key_;
	case 1:
		return *(float*)key_ == *(float*)t1.key_;
	case 2:
		return (strncmp(key_, t1.key_, length_) == 0);
	default:
		return false;
	}
}

bool TKey::operator<=(const TKey t1) { return !(operator>(t1)); }
bool TKey::operator>=(const TKey t1) { return !(operator<(t1)); }
bool TKey::operator!=(const TKey t1) { return !(operator==(t1)); }
//xj
TKey* TKey::operator+=(const TKey t1) {
	TKey * temp = new TKey(t1);
	switch (t1.key_type_)
	{
	case 0:
		*(int*)key_ += *(int*)(*temp).key_;
		return temp;
	case 1:
		*(float*)key_ += *(float*)(*temp).key_;
		return temp;
	case 2:
		key_ = "";
		return nullptr;
	default:
		return nullptr;
	}
}

TKey* TKey::operator/=(const TKey t1) {
	TKey * temp = new TKey(*this);//not support varchar
	switch (key_type_)
	{
	case 0:
	case 1:
		*(float*)key_ /= *(float*)(*temp).key_;
	case 2:
		key_ = "";
		return nullptr;
	default:
		return nullptr;
	}
	return temp;
}

TKey* TKey::operator/=(const int t1) {
	TKey * temp = new TKey(*this);//not support varchar
	switch (key_type_)
	{
	case 0:
	case 1:
		*(float*)key_ /= (float)t1;
		return temp;
	case 2:
		key_ = "";
		return nullptr;
	default:
		return nullptr;
	}
}
#pragma endregion
/* ----------------- SQLExec ----------------*/
//SQLExec::SQLExec(vector<string> sql_vector) { Parse(sql_vector); }
//
//string SQLExec::get_file_name() { return file_name_; }
//
//void SQLExec::Parse(vector<string> sql_vector)
//{
//	sql_type_ = 80;
//	if (sql_vector.size() <= 1) throw SyntaxErrorException();
//	else file_name_ = sql_vector[1];
//}