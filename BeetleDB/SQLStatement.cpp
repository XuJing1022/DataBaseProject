//Implemented by Jin Xin
#include"SQLStatement.h"

#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>
using namespace std;

#pragma region class ʵ�֣�SQL
/*sql���������Ĺ��캯��*/
SQL::SQL()
{
	sql_type_ = -1;
}

/*sql������int sql_type�Ĺ��캯��*/
SQL::SQL(int sql_type)
{
	sql_type_ = sql_type;
}

/*��ȡsql��������*/
int SQL::get_sql_type()
{
	return sql_type_;
}

/*����sql��������*/
void SQL::set_sql_type(int sql_type)
{
	sql_type_ = sql_type;
}

/*����SQL������ֶε���������*/
int SQL::ParseDataType(vector<string> sql_vector, Attribute &attr, unsigned int pos)
{
	boost::algorithm::to_lower(sql_vector[pos]);/*��ȡСд���ں����ж�*/
	if (sql_vector[pos] == "int")/*�����int���ͣ�����attr����������ΪT_INT,attr�ֶγ���Ϊ4*/
	{
		attr.set_data_type(T_INT);
		attr.set_length(4);
		pos++;
		if (sql_vector[pos] == ",") pos++;
	}
	else if (sql_vector[pos] == "float")/*�����float���ͣ�����attr����������ΪT_FLOAT,attr�ֶγ���Ϊ4*/
	{
		attr.set_data_type(T_FLOAT);
		attr.set_length(4);
		pos++;
		if (sql_vector[pos] == ",") pos++;
	}
	else if (sql_vector[pos] == "char" || sql_vector[pos] == "varchar")/*�����char����varchar���ͣ�����attr����������ΪT_CHAR,attr�ֶγ���Ϊ�û����ó���*/
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

#pragma region class ʵ�֣�SQLCreateDatabase
/*SQLCreateDatabase�Ĺ��캯��*/
SQLCreateDatabase::SQLCreateDatabase(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡdatabase������*/
string SQLCreateDatabase::get_database_name()
{
	return database_name_;
}

/*����database������*/
void SQLCreateDatabase::set_database_name(string dbname)
{
	database_name_ = dbname;
}

/*����sql��ȡdatabase������*/
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

#pragma region class ʵ�֣�SQLCreateTable
/*SQLCreateTable�Ĺ��캯��*/
SQLCreateTable::SQLCreateTable(vector<string> sql_vector)
{
	Parse(sql_vector);
}
/*��ȡtable������*/
string SQLCreateTable::get_table_name()
{
	return table_name_;
}
/*����table������*/
void SQLCreateTable::set_table_name(string dbname)
{
	table_name_ = dbname;
}
/*��ȡtable������*/
vector<Attribute> SQLCreateTable::get_attributes()
{
	return attributes_;
}
/*����table������*/
void SQLCreateTable::SetAttributes(vector<Attribute> attr)
{
	attributes_ = attr;
}
/*����sql��ȡtable�����֡�table���ԡ�����create table student (name char(100), id int, primary key(id));*/
void SQLCreateTable::Parse(vector<string> sql_vector)
{
	sql_type_ = 22;
	unsigned int pos = 2;
	if (sql_vector.size() <= pos) throw SyntaxErrorException();/*���ֻ��create table �������*/

	table_name_ = sql_vector[pos];/*��ȡtable name*/
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
			attr.set_attribute_name(sql_vector[pos]);/*��ȡ�ֶ�����*/
			pos++;
			pos = ParseDataType(sql_vector, attr, pos);/*��ȡ�ֶε���������*/
			attributes_.push_back(attr);/*���ֶ�vector������ֶ���Ϣ*/
			if (sql_vector[pos] != ")") if_attr = true;/*����ⲻ�����һ���ֶ�*/
		}
		else/* primary key(id))*/
		{
			if (if_primary_key) throw SyntaxErrorException();/*����Ѿ����������ش���*/
			pos++;
			if (sql_vector[pos] != "key") throw SyntaxErrorException();/*�����ʽ��Ϊ primary key�򷵻ش���*/
			pos++;
			if (sql_vector[pos] != "(") throw SyntaxErrorException();/*�����ʽ��Ϊ primary key( �򷵻ش���*/
			pos++;
			bool if_arrt_exist = false;
			for (auto att = attributes_.begin(); att != attributes_.end(); att++)/*��ȡ֮ǰ�����������ֶΣ������Ƿ���primary key�еĸ�id�ֶ�*/
			{
				if ((*att).get_attr_name() == sql_vector[pos])
				{
					(*att).set_attribute_type(1);
					if_arrt_exist = true;
				}
			}
			if (if_arrt_exist == false) throw SyntaxErrorException();/*���primary key ���ֶ���֮ǰ�������ֶ��в������򷵻ش���*/
			pos++;
			if (sql_vector[pos] != ")") throw SyntaxErrorException();/*�����ʽ��Ϊ primary key(id) �򷵻ش���*/
			if_primary_key = true;
		}
	}
}
#pragma endregion

#pragma region class ʵ�֣�SQLCreateIndex
/*SQLCreateIndex�Ĺ��캯��*/
SQLCreateIndex::SQLCreateIndex(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
string SQLCreateIndex::get_tb_name()
{
	return table_name_;
}

/*��ȡ����������*/
string SQLCreateIndex::get_index_name()
{
	return index_name_;
}

/*��ȡ���������ֶε�����*/
string SQLCreateIndex::get_column_name()
{
	return col_name_;
}

/*����sql��ȡtable�����֡����������֡����������ֶε����� ���磺create index i1 on student(id); */
void SQLCreateIndex::Parse(vector<string> sql_vector)
{
	sql_type_ = 23;
	unsigned int pos = 2;
	if (sql_vector.size() <= pos) throw SyntaxErrorException();/*���sql����Ϊ create index�򷵻��������*/

	index_name_ = sql_vector[pos];/*��ȡindex������*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "on") throw SyntaxErrorException();/*���sql��Ϊcreate index i1 on �򷵻ش���*/
	pos++;

	table_name_ = sql_vector[pos];/*��ȡtable ������*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "(") throw SyntaxErrorException();/*���sql��Ϊcreate index i1 on t1( �򷵻ش���*/
	pos++;

	col_name_ = sql_vector[pos];/*��ȡ�����������ֶ���*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != ")") throw SyntaxErrorException();/*���sql��Ϊcreate index i1 on t1(id) �򷵻ش���*/
	pos++;
}
#pragma endregion

#pragma region class ʵ�֣�SQLDropDatabase
/*SQLDropDatabase�Ĺ��캯��*/
SQLDropDatabase::SQLDropDatabase(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡ���ݿ������*/
string SQLDropDatabase::get_database_name()
{
	return database_name_;
}

/*�������ݿ������*/
void SQLDropDatabase::set_database_name(string dbname)
{
	database_name_ = dbname;
}

/*����sql��ȡ���ݿ����� ���磺drop database university;*/
void SQLDropDatabase::Parse(vector<string> sql_vector)
{
	sql_type_ = 41;
	if (sql_vector.size() <= 2 || sql_vector.size()>3) throw SyntaxErrorException();/*���sql��䲻Ϊdrop database university���ش���*/
	else
	{
		database_name_ = sql_vector[2];/*��ȡdatabase������*/
	}
}
#pragma endregion

#pragma region class ʵ�֣�SQLDropTable
/*SQLDropTable�Ĺ��캯��*/
SQLDropTable::SQLDropTable(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
string SQLDropTable::get_table_name()
{
	return table_name_;
}

/*����table������*/
void SQLDropTable::set_table_name(string tbname)
{
	table_name_ = tbname;
}

/*����sql��ȡtable������ ���磺drop table student;*/
void SQLDropTable::Parse(vector<string> sql_vector)
{
	sql_type_ = 42;
	if (sql_vector.size() <= 2 || sql_vector.size()>3) throw SyntaxErrorException();/*���sql��䲻Ϊdrop table student;���ش���*/
	else table_name_ = sql_vector[2];
}
#pragma endregion

#pragma region class ʵ�֣�SQLDropIndex
/*SQLDropIndex�Ĺ��캯��*/
SQLDropIndex::SQLDropIndex(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡindex������*/
string SQLDropIndex::get_index_name()
{
	return index_name_;
}

/*����index������*/
void SQLDropIndex::set_index_name(string idxname)
{
	index_name_ = idxname;
}

/*����sql��ȡindex������ drop index i1;*/
void SQLDropIndex::Parse(vector<string> sql_vector)
{
	sql_type_ = 52;
	if (sql_vector.size() <= 2 || sql_vector.size()>3) throw SyntaxErrorException();/*���sql��䲻Ϊ drop index i1;���ش���*/
	else index_name_ = sql_vector[2];
}
#pragma endregion

#pragma region class ʵ�֣�SQLUse
/*SQLUse�Ĺ��캯��*/
SQLUse::SQLUse(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡ���ݿ������*/
string SQLUse::get_database_name()
{
	return database_name_;
}

/*�������ݿ������*/
void SQLUse::set_database_name(string dbname)
{
	database_name_ = dbname;
}

/*����sql��ȡ���ݿ������use university��*/
void SQLUse::Parse(vector<string> sql_vector)
{
	sql_type_ = 91;
	if (sql_vector.size() <= 1 || sql_vector.size()>2) throw SyntaxErrorException();/*���sql���Ͳ�Ϊuse university;�򷵻ش���*/
	else
	{
		database_name_ = sql_vector[1];
	}
}
#pragma endregion

#pragma region class ʵ�֣�SQLInsert
/*SQLInsert�Ĺ��캯��*/
SQLInsert::SQLInsert(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
string SQLInsert::get_tb_name()
{
	return table_name_;
}

/*��ȡSQLValue��ֵ*/
vector<SQLValue>& SQLInsert::GetValues()
{
	return values_;
}

/*����sql��ȡtable�����ֺ�SQLValue��ֵ  insert into student values('Tom',1);*/
void SQLInsert::Parse(vector<string> sql_vector)
{
	sql_type_ = 51;
	unsigned int pos = 1;
	bool if_attr = true;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "into") throw SyntaxErrorException();/*���sql��Ϊ insert into �򷵻ش���*/
	pos++;

	table_name_ = sql_vector[pos];/*��ȡ����ı������*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "values") throw SyntaxErrorException();/*���sql ��Ϊ insert into student values�򷵻ش���*/
	pos++;

	if (sql_vector[pos] != "(") throw SyntaxErrorException();/*���sql ��Ϊ insert into student values(�򷵻ش���*/
	pos++;

	while (if_attr)/*'Tom',1*/
	{
		if_attr = false;
		SQLValue sql_value;
		string val = sql_vector[pos];
		if (val[0] == '\'' || val[0] == '\"')/*'Tom'*/
		{
			val.assign(val, 1, val.length() - 2);/*��Tom��ֵ��val*/
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
		if (sql_vector[pos] != ")")/*���滹��value*/
		{
			if_attr = true;
		}
		pos++;
	}
}
#pragma endregion

#pragma region class ʵ�֣�SQLSelect
/*SQLSelect�Ĺ��캯��*/
SQLSelect::SQLSelect(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
string SQLSelect::get_tb_name()
{
	return table_name_;
}

/*��ȡSQLWhere��ֵ����ѯ����*/
vector<SQLWhere>& SQLSelect::GetWheres()
{
	return wheres_;
}

/*��ȡ����select_attribute_*/
vector<string>& SQLSelect::get_select_attribute()
{
	return select_attribute_;
}

/*����sql��ȡtable�����ֺ�SQLWhere��ֵ select * from student where name = 'Tom';��ʱֻ֧��select**/
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

	table_name_ = sql_vector[pos];/*��ȡtable����*/
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

		if (sql_vector.size() == pos) break; /*where ��䵽�˽���*/
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*select * from student where id=1 and name ='Tom'*/
		pos++;
	}
}
#pragma endregion

#pragma region  class ʵ�֣�SQLJoinSelect
SQLJoinSelect::SQLJoinSelect(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
vector<string>  SQLJoinSelect::get_table_names()
{
	return table_names_;
}

/*��ȡSQLWhere��ֵ����ѯ����*/
vector<SQLWhere>& SQLJoinSelect::get_wheres()
{
	return wheres_;
}
/*����sql��ȡtable�����ֺ�SQLWhere��ֵ select * from student join study where name = 'Tom';��ʱֻ֧��select**/
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
		table_names_[table_count++] = sql_vector[pos];/*��ȡtable����*/
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

		if (sql_vector.size() == pos) break; /*where ��䵽�˽���*/
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*select * from student where id=1 and name ='Tom'*/
		pos++;
	}
}
#pragma endregion

#pragma region class ʵ�֣�SQLDelete
/*SQLDelete�Ĺ��캯��*/
SQLDelete::SQLDelete(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
string SQLDelete::get_tb_name()
{
	return table_name_;
}

/*��ȡSQLWhere��ֵ����ѯ����*/
vector<SQLWhere>& SQLDelete::GetWheres()
{
	return wheres_;
}

/*����sql��ȡtable�����ֺ�SQLWhere��ֵ delete from student where name='Tom';*/
void SQLDelete::Parse(vector<string> sql_vector)
{
	sql_type_ = 71;
	unsigned int pos = 1;
	if (sql_vector.size() <= 1) throw SyntaxErrorException();/*delete*/
	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "from") throw SyntaxErrorException();/*delete from*/
	pos++;

	table_name_ = sql_vector[pos];/*��ȡtable������*/
	pos++;

	if (sql_vector.size() == pos) return; /*delete from student;���Ѹı�����ȫ��ɾ��*/
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

		if (sql_vector.size() == pos) break; /*where �������˽��� */
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*delete  from student where id=1 and name ='Tom'*/
		pos++;
	}

}
#pragma endregion

#pragma region class ʵ�֣�SQLUpdate
/*SQLUpdate�Ĺ��캯��*/
SQLUpdate::SQLUpdate(vector<string> sql_vector)
{
	Parse(sql_vector);
}

/*��ȡtable������*/
string SQLUpdate::get_tb_name()
{
	return table_name_;
}

/*��ȡSQLWhere��ֵ����ѯ����*/
vector<SQLWhere>& SQLUpdate::GetWheres()
{
	return wheres_;
}

/*��ȡSQLKeyValue��ֵ�������ֶ�*/
vector<SQLKeyValue>& SQLUpdate::GetKeyValues()
{
	return keyvalues_;
}

/*����sql��ȡtable�����ֺ�SQLWhere��ֵ��SQLKeyValue��ֵ update student set name ='Tim' where name='Tom';*/
void SQLUpdate::Parse(vector<string> sql_vector)
{
	sql_type_ = 81;
	unsigned int pos = 1;
	if (sql_vector.size() <= 1) throw SyntaxErrorException();/*update*/

	table_name_ = sql_vector[pos];/*��ȡtable������*/
	pos++;

	if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "set") throw SyntaxErrorException();/*update student set*/
	pos++;

	while (true)/*��ȡset�ֶ����Ӧ�ֶ�ֵ*/
	{
		SQLKeyValue sql_kv;
		sql_kv.key = sql_vector[pos];/*��ȡset�ֶ�*/
		pos++;

		if (sql_vector[pos] != "=") throw SyntaxErrorException();/*set name=*/
		pos++;

		sql_kv.value = sql_vector[pos];/*��ȡ��Ӧ�ֶ�ֵ*/
		if (sql_kv.value[0] == '\'' || sql_kv.value[0] == '\"') sql_kv.value.assign(sql_kv.value, 1, sql_kv.value.length() - 2);/*'Tom'��ΪTom*/
		keyvalues_.push_back(sql_kv);
		pos++;

		if (sql_vector[pos] == ",") pos++;/*����еڶ���set�ֶ������*/
		else if (boost::algorithm::to_lower_copy(sql_vector[pos]) == "where") break;/*���������where�ֶ�������ѭ����ʼ��ȡwhere�ֶ�ֵ*/
		else throw SyntaxErrorException();/*�������*/
	}
	if (sql_vector[pos] != "where") throw SyntaxErrorException();/*update student set name = 'Tim' where*/
	pos++;

	while (true)/*��ȡwhere�ֶ�ֵ*/
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

		if (sql_vector.size() == pos) break; /*where�ֶε��˽��� */
		if (boost::algorithm::to_lower_copy(sql_vector[pos]) != "and") throw SyntaxErrorException();/*update  student set name='Tim' where id=1 and name ='Tom'*/
		pos++;
	}
}
#pragma endregion

#pragma region class ʵ�֣�TKey
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