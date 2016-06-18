#pragma once
#pragma region 
//简介：查询解析器
//Implemented by Jin Xin
//作用：负责解析用户输入的查询语句并调用对应的API类里的函数
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
	 QueryParser();/*QueryParser构造函数*/
	~ QueryParser();/*QueryParser的析构函数*/
	void ExecuteSQL(string sql);/*对外接口，根据sql来执行相应的操作*/
private:
	API* api;
	string sql_;
	vector<string> sql_vector_;
	vector<string> SplitSQL(string sql, string seg);/*拆分sql语句，将每个单词放入vector中*/
	int sql_type_;
	void FormateSQL();/*规范化用户输入的sql，便于后续拆分*/
	void GetSqlType();/*获取sql语句的类型*/
	void InvokeAPI();/*根据sql类型调用对应的api*/
};
#endif // !_QUERYPARSER_H_
