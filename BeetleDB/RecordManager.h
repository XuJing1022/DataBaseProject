
//Implemented by Lai ZhengMin & Xu Jing

#pragma once
#ifndef _RECORDMANAGER_H_
#define _RECORDMANAGER_H_

#include <string>
#include <vector>
#include "QueryParser.h"
#include "SQLStatement.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#include "Exceptions.h"
#include "BlockInfo.h"
//一个数据库有很多表（可以看做是段，表名代表段号），表是用块（可以看做是页，块号代表页号）的集合存储的，而记录是存在块内的，记录通过块内（页内）的偏移量得到。
using namespace std;
//记录（tuple）管理器
class RecordManager
{
public:
	RecordManager(CatalogManager *cm, BufferManager *bm, string dbname);
	~RecordManager(void);
	string intToString(int x);
	void Insert(SQLInsert& st);
	vector<vector<TKey>> Select(SQLSelect& st);
	void JoinSelect(SQLJoinSelect& st);
	void Delete(SQLDelete& st);
	void Update(SQLUpdate& st);
	//返回表中块号为block_num的块
	BlockInfo* GetBlockInfo(Table* tbl, int block_num);
	//返回tb1第block_num块的第offset个tuple
	vector<TKey> GetRecord(Table* tbl, int block_num, int offset);
	//删除表内第block_num块的第offset个记录
	void DeleteRecord(Table* tbl, int block_num, int offset);
	//用数据类型集合和key集合更新表内第block_num块的第offset个记录
	void UpdateRecord(Table* tbl, int block_num, int offset, vector<int>& indices/*数据类型集合*/, vector<TKey>& values/*每个数据类型对应的键值集合*/);
	//对于表tb1的某行键值集合是否满足where子句
	bool SatisfyWhere(Table* tbl, vector<TKey> keys, SQLWhere where);

	/**********************    聚集函数实现  ***************************/
	//xj 06/16	aggregation
	TKey Min(vector<vector<TKey>> tuples, int MinIndex);
	TKey Max(vector<vector<TKey>> tuples, int MinIndex);
	TKey* Avg(vector<vector<TKey>> tuples, int MinIndex);
	int Count(vector<TKey> tuples, int Index);
private:
	CatalogManager* catalog_m_;
	BufferManager* buffer_m_;
	string db_name_;
};
#endif
