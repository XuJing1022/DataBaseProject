
//Implemented by Lai ZhengMin  & XuJing

#pragma once
#ifndef _RECORDMANAGER_H_
#define _RECORDMANAGER_H_

#include <string>
#include <vector>

#include "SQLStatement.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#include "Exceptions.h"
#include "BlockInfo.h"
//һ�����ݿ��кܶ�����Կ����ǶΣ���������κţ��������ÿ飨���Կ�����ҳ����Ŵ���ҳ�ţ��ļ��ϴ洢�ģ�����¼�Ǵ��ڿ��ڵģ���¼ͨ�����ڣ�ҳ�ڣ���ƫ�����õ���
using namespace std;
//��¼��tuple��������
class RecordManager
{
public:
	RecordManager(CatalogManager *cm, BufferManager *bm, string dbname);
	~RecordManager(void);

	void Insert(SQLInsert& st);
	void Select(SQLSelect& st);
	void Delete(SQLDelete& st);
	void Update(SQLUpdate& st);
	//���ر��п��Ϊblock_num�Ŀ�
	BlockInfo* GetBlockInfo(Table* tbl, int block_num);
	//����tb1��block_num��ĵ�offset��tuple
	vector<TKey> GetRecord(Table* tbl, int block_num, int offset);
	//ɾ�����ڵ�block_num��ĵ�offset����¼
	void DeleteRecord(Table* tbl, int block_num, int offset);
	//���������ͼ��Ϻ�key���ϸ��±��ڵ�block_num��ĵ�offset����¼
	void UpdateRecord(Table* tbl, int block_num, int offset, vector<int>& indices/*�������ͼ���*/, vector<TKey>& values/*ÿ���������Ͷ�Ӧ�ļ�ֵ����*/);
	//���ڱ�tb1��ĳ�м�ֵ�����Ƿ�����where�Ӿ�
	bool SatisfyWhere(Table* tbl, vector<TKey> keys, SQLWhere where);

	/**********************                  �ۼ�����ʵ��                      ********************************/
	//xj0616	aggregation
	TKey RecordManager::Min(vector<vector<TKey> > tuples, int MinIndex);
	TKey RecordManager::Max(vector<vector<TKey> > tuples, int MinIndex);
	TKey* RecordManager::Avg(vector<vector<TKey> > tuples, int MinIndex);
	int RecordManager::Count(vector<vector<TKey> > tuples, int Index);

private:
	CatalogManager* catalog_m_;
	BufferManager* buffer_m_;
	string db_name_;
};
#endif