#pragma once
#pragma region 
//API.h头文件
//Implemented by Jin Xin
//【catalog Manager】负责和catalog manager 交互
//【B Plus Tree】负责和BPlusTree 进行交互
//【console】负责控制台交互（控制台输出信息）
//【file System】负责文件系统交互
#pragma endregion

#ifndef  API_H_
#define API_H_

#include"SQLStatement.h"

#include"CatalogManager.h"
#include"BufferManager.h"
#include"IndexManager.h"

class API
{
public:
	API(string path);/*API构造函数*/
	~API(void);/*API析构函数*/
	void help();/*显示各种命令*/
	void CreateDatabase(SQLCreateDatabase& sql_statement);/*新建数据库*/
	void CreateTable(SQLCreateTable& sql_statement);/*新建数据表*/
	void CreateIndex(SQLCreateIndex& sql_statement);/*新建索引*/
	void ShowDatabases();/*显示数据库*/
	void ShowTables();/*显示数据表*/
	void DropDatabase(SQLDropDatabase& sql_statement);/*删除数据库*/
	void DropTable(SQLDropTable& sql_statement);/*删除数据表*/
	void DropIndex(SQLDropIndex& sql_statement);/*删除索引*/
	void Use(SQLUse& sql_statement);/*选定数据库*/
	void Insert(SQLInsert& sql_statement);/*插入数据*/
	void Select(SQLSelect& sql_statement);/*查询数据*/
	void JoinSelect(SQLJoinSelect& sql_statement);/*join查询*/
	void Delete(SQLDelete& sql_statement);/*删除数据*/
	void Update(SQLUpdate& sql_statement);/*更新数据*/
private:
	string path_;//该数据库路径
	string current_database_;//当前选定数据库
	CatalogManager* catalog_manager_;//目录管理器
	BufferManager*	buffer_manager_;//缓存管理器
};
#endif // ! API_H_