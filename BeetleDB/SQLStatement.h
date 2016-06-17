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
	SQL();/*sql���������Ĺ��캯��*/
	SQL(int sql_type);/*sql������int sql_type�Ĺ��캯��*/
	int get_sql_type();/*��ȡsql��������*/
	void set_sql_type(int sql_type);/*����sql��������*/
	int ParseDataType(vector<string> sql_vector, Attribute &attribute, unsigned int pos);/*����SQL������ֶε���������*/
	virtual void Parse(vector<string> sql_vector) = 0;/*�麯��Ҫ��̳и�����������д*/
protected:
	int sql_type_;//sql��������
};
#pragma endregion

#pragma region class: SQLCreateDatabase ���磺create database university;
class SQLCreateDatabase : public SQL
{
public:
	SQLCreateDatabase(vector<string> sql_vector);/*SQLCreateDatabase�Ĺ��캯��*/
	string get_database_name();/*��ȡdatabase������*/
	void set_database_name(string database_name);/*����database������*/
	void Parse(vector<string> sql_vector);/*����sql��ȡdatabase������*/
private:
	string database_name_;//database������
};
#pragma endregion

#pragma region class SQLCreateTable ���磺create table student (name char(100), id int, primary key(id));
class SQLCreateTable : public SQL
{
public:
	SQLCreateTable(vector<string> sql_vector);/*SQLCreateTable�Ĺ��캯��*/
	string get_table_name();/*��ȡtable������*/
	void set_table_name(string table_name);/*����table������*/
	vector<Attribute> get_attributes();/*��ȡtable������*/
	void SetAttributes(vector<Attribute> attribute);/*����table������*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable�����֡�table����*/
private:
	string table_name_;//table������
	vector<Attribute> attributes_;//table������
};
#pragma endregion

#pragma region class SQLCreateIndex ���磺create index i1 on student(id); 
class SQLCreateIndex : public SQL
{
public:
	SQLCreateIndex(vector<string> sql_vector);/*SQLCreateIndex�Ĺ��캯��*/
	string get_tb_name();/*��ȡtable������*/
	string get_index_name();/*��ȡ����������*/
	string get_column_name();/*��ȡ���������ֶε�����*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable�����֡����������֡����������ֶε�����*/
private:
	string index_name_;//����������
	string table_name_;//table������
	string col_name_;//���������ֶε�����
};
#pragma endregion

#pragma region class SQLDropDatabase ���磺drop database university;
class SQLDropDatabase : public SQL
{
public:
	SQLDropDatabase(vector<string> sql_vector);/*SQLDropDatabase�Ĺ��캯��*/
	string get_database_name();/*��ȡ���ݿ������*/
	void set_database_name(string dbname);/*�������ݿ������*/
	void Parse(vector<string> sql_vector);/*����sql��ȡ���ݿ�����*/
private:
	string database_name_;//���ݿ�����
};
#pragma endregion

#pragma region class SQLDropTable ���磺drop table student;
class SQLDropTable : public SQL
{
public:
	SQLDropTable(vector<string> sql_vector);/*SQLDropTable�Ĺ��캯��*/
	string get_table_name();/*��ȡtable������*/
	void set_table_name(string table_name);/*����table������*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable������*/
private:
	string table_name_;//table������
};
#pragma endregion

#pragma region class SQLDropIndex ���磺drop index i1;
class SQLDropIndex : public SQL
{
public:
	SQLDropIndex(vector<string> sql_vector);/*SQLDropIndex�Ĺ��캯��*/
	string get_index_name();/*��ȡindex������*/
	void set_index_name(string  index_name);/*����index������*/
	void Parse(vector<string> sql_vector);/*����sql��ȡindex������*/
private:
	string index_name_;//����������
};
#pragma endregion

#pragma region class SQLUse ���磺use university��
class SQLUse : public SQL
{
public:
	SQLUse(vector<string> sql_vector);/*SQLUse�Ĺ��캯��*/
	string get_database_name();/*��ȡ���ݿ������*/
	void set_database_name(string database_name);/*�������ݿ������*/
	void Parse(vector<string> sql_vector);/*����sql��ȡ���ݿ������*/
private:
	string database_name_;//���ݿ������
};
#pragma endregion

#pragma region struct SQLValue ���磺name|Tom �����ֶ������ֶ�ֵ
typedef	struct
{
	int data_type;
	string value;
}SQLValue;
#pragma endregion

#pragma region struct SQLWhere ���� where name='Tom'
typedef struct
{
	int op_type;
	string key;
	string value;
}SQLWhere;
#pragma endregion

#pragma region struct SQLKeyValue ���� set name='Tom'
typedef struct
{
	string key;
	string value;
}SQLKeyValue;
#pragma endregion


#pragma region class SQLInsert ���磺 insert into student values('Tom',1);
class SQLInsert : public SQL
{
public:
	SQLInsert(vector<string> sql_vector);/*SQLInsert�Ĺ��캯��*/
	string get_tb_name();/*��ȡtable������*/
	vector<SQLValue>& GetValues();  /*��ȡSQLValue��ֵ*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable�����ֺ�SQLValue��ֵ*/
private:
	string table_name_;//table ������
	vector<SQLValue> values_;//SQLValue��ֵ value��ֵ
};
#pragma endregion

#pragma region class SQLSelect ���磺select name from student where name = 'Tom';
class SQLSelect : public SQL
{
public:
	SQLSelect(vector<string> sql_vector);/*SQLSelect�Ĺ��캯��*/
	string get_tb_name();/*��ȡtable������*/
	vector<SQLWhere>& GetWheres();/*��ȡSQLWhere��ֵ����ѯ����*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable�����ֺ�SQLWhere��ֵ*/
	vector<string>&  get_select_attribute();/*��ȡ����select_attribute_*/
private:
	string table_name_;//table������
	vector<SQLWhere> wheres_;//SQLWhere��ֵ��where��ѯ����
	vector<string> select_attribute_;//�洢Ҫ��ѯ���ֶεı���
};
#pragma endregion

#pragma region class SQLJoinSelect ���磺select name from student join takes where name ='Tom';
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

#pragma region class SQLDelete ���磺delete from student where name='Tom';
class SQLDelete : public SQL
{
public:
	SQLDelete(vector<string> sql_vector);/*SQLDelete�Ĺ��캯��*/
	string get_tb_name();/*��ȡtable������*/
	vector<SQLWhere>& GetWheres();/*��ȡSQLWhere��ֵ����ѯ����*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable�����ֺ�SQLWhere��ֵ*/
private:
	string table_name_;//table������
	vector<SQLWhere> wheres_;//SQLWhere��ֵ��where��ѯ����
};
#pragma endregion

#pragma region class SQLUpdate ���磺update student set name ='Tim' where name='Tom';
class SQLUpdate : public SQL
{
public:
	SQLUpdate(vector<string> sql_vector);/*SQLUpdate�Ĺ��캯��*/
	string get_tb_name();/*��ȡtable������*/
	vector<SQLWhere>& GetWheres();/*��ȡSQLWhere��ֵ����ѯ����*/
	vector<SQLKeyValue>& GetKeyValues();/*��ȡSQLKeyValue��ֵ�������ֶ�*/
	void Parse(vector<string> sql_vector);/*����sql��ȡtable�����ֺ�SQLWhere��ֵ��SQLKeyValue��ֵ*/
private:
	string table_name_;//table������
	vector<SQLWhere> wheres_;//SQLWhere��ֵ��where��ѯ����
	vector<SQLKeyValue> keyvalues_;//SQLKeyValue��ֵ��set���ֶ���ֵ
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
