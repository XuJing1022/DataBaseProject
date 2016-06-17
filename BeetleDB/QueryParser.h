#pragma once
#pragma region 
//��飺��ѯ������
//Implemented by Jin Xin
//���ã���������û�����Ĳ�ѯ��䲢���ö�Ӧ��API����ĺ���
#pragma endregion

#ifndef _QUERYPARSER_H_
#define _QUERYPARSER_H_

#include "API.h"
#include "SQLStatement.h"
#include <string>
#include <vector>
using namespace std;
class  QueryParser
{
public:
	QueryParser();/*QueryParser���캯��*/
	~QueryParser();/*QueryParser����������*/
	void ExecuteSQL(string sql);/*����ӿڣ�����sql��ִ����Ӧ�Ĳ���*/
private:
	API* api;
	string sql_;
	vector<string> sql_vector_;
	vector<string> SplitSQL(string sql, string seg);/*���sql��䣬��ÿ�����ʷ���vector��*/
	int sql_type_;
	void FormateSQL();/*�淶���û������sql�����ں������*/
	void GetSqlType();/*��ȡsql��������*/
	void InvokeAPI();/*����sql���͵��ö�Ӧ��api*/
};
#endif // !_QUERYPARSER_H_