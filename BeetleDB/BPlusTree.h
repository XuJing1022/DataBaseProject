//
// BPlusTree.h
//
// Created by Xujing on 2016/6/14.
//

#pragma once
#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <string>
#include <vector>

#include "CatalogManager.h"
#include "BufferManager.h"

using namespace std;

class BPlusTree;		/*B+����*/
class BPlusTreeNode;	/*B+���ڵ���*/

typedef struct
{
	BPlusTreeNode* pnode;
	int index;	
	bool flag;
} FindNodeParam;				/*�ڵ���Ҹ����ṹ*/

class BPlusTree
{
public:
	BPlusTree(Index* idx,BufferManager* bm, CatalogManager* cm,string dbname);
	~BPlusTree(void);
	
	Index* GetIndex();										/*��ȡ��B+����������Ա*/
	BufferManager* GetBufferManager();						/*��ȡ��B+����buffermanager*/
	CatalogManager* GetCatalogManager();					/*��ȡ��B+����Ŀ¼������*/
	int get_degree();										/*��ȡ��*/
	string get_db_name();									/*��ȡ��B+�������ݿ���*/

	bool Add(TKey& key, int block_num, int offset);			/*�ڵ�block_num�����ϼ�Ԫ��key ƫ��offset*/
	bool AdjustAfterAdd(int node);							/*��Ԫ�غ����B+��*/

	bool Remove(TKey key);									/*�Ƴ�keyԪ��*/
	bool AdjustAfterRemove(int node);						/*ɾԪ�غ����B+��*/

	FindNodeParam Search(int node, TKey &key);				/*������ֱ��Ҷ�ӽڵ�Ĳ�ѯ����node��ʼ������key���ڵ�Ҷ�ӽڵ㡣ans.flag��true����key��B+���д��ڣ�false����key��B+���в�����*/
	FindNodeParam SearchBranch(int node, TKey &key);		/*������ɾ�ڵ�ʱ���ڲ��ڵ�仯�Ĳ�ѯ����node��ʼ����ѯkey���ڵ�pnode����index��FindNodeParam�е�flag��true��pnodeΪҶ�ӽڵ㣻false��pnodeΪ�ڲ��ڵ�*/
	BPlusTreeNode* GetNode(int num);						/*��ȡ��num���ڵ㡣ʵ�ַ�������ȡ�ļ�ϵͳ�е�num���飬�ÿ��е�data_���ݼ�Ϊ�ýڵ�*/

	int GetVal(TKey key);									/*��key��ѯvalueֵ*/
	vector<int> GetVal(TKey key, int op_type);				/*��key��ѯvalueֵ��op_type�������ж��Ƿ�Ϊ��Χ��ѯ*/
	int GetNewBlockNum();									/*idx_�������ֵ��һ*/

	void Print();
	void PrintNode(int num);

private:
	Index *idx_;											/*����ָ�룬�����˵�ǰB+���Ļ�����Ϣ��ͷ�ڵ��ַ�����Լ����ȵȣ�����һ��table������һ��B+�����������table����������Ϣ���������б��У����������ָ����table�е������б��ĳһ�*/
	int degree_;											/*��*/
	BufferManager *buffer_m_;								/*���������ָ��*/
	CatalogManager *catalog_m_;								/*Ŀ¼������ָ��*/
	string db_name_;										/*��B+��������db����*/
	void InitTree();										/*��ʼ�����������ڵ㣬��ʼidx����*/
};

class BPlusTreeNode
{
public:
	BPlusTreeNode(BPlusTree* tree,bool isnew,  int blocknum,  bool newleaf=false);
	~BPlusTreeNode();

	int get_block_num();

	TKey GetKeys(int index);			/*��ȡ��index��keyֵ*/
	int GetValues(int index);			/*��ȡ��index��valueֵ*/
	int GetNextLeaf();					/*��ȡ��һ��Ҷ�ӽڵ�*/
	int GetParent();					/*��ȡ��һ�����ڵ㣬�õ����Ǹýڵ���buffer�еĵ�ַ��ţ��Ǹýڵ����ڵ�buffer�ĵ�8λ����*/
	int GetNodeType();					/*��ȡ�ڵ����ͣ����ýڵ�����buffer�еĵ�0λ����*/					
	int GetCount();						/*��ȡ�ڵ�����ݸ��������ýڵ�����buffer�еĵ�4λ����*/
	bool GetIsLeaf();					/*�ж��Ƿ���Ҷ�ӽڵ�*/

	void SetKeys(int index, TKey key);	/*��key.key_����ýڵ�ĵ�index������*/
	void SetValues(int index, int val);	/**/
	void SetNextLeaf(int val);			/*����Ҷ�ӽڵ��ֵ*/
	void SetParent(int val);			/*���ø��ڵ��ֵ*/
	void SetNodeType(int val);			/*�ڵ����ͣ�Ҷ�ӽڵ�Ϊ1����Ҷ�ӽڵ�Ϊ0*/
	void SetCount(int val);				/*���ýڵ�洢��Ԫ�ظ���*/
	void SetIsLeaf(bool val);			/*���ýڵ��Ƿ�ΪҶ�ӽڵ�*/

	void GetBuffer();					/*��file�л�ȡ��ǰB+�����ڵ�db�е�ǰ���������ļ���*/

	bool Search(TKey key, int &index);	/*��B+��������key���ڵ�λ�ã�����ֵ��index�С�����ֵ��true��index��Ϊ��key��λ�Ľڵ��ַ��false��index��Ϊָ����һ���ָ�롣�ڵ�Ԫ�ظ���20���ڣ�˳����ң�20���⣬���ֲ���*/
	int Add(TKey &key);					/*�Ȳ���b+�����Ƿ���ڸ�key���񣺽�key�������Ӧ��λ����*/
	int Add(TKey &key, int &val);		/*����KV��*/
	BPlusTreeNode* Split(TKey &key);	/*����*/

	bool IsRoot();						/*�ж��Ƿ�Ϊ���ڵ�*/
	bool RemoveAt(int index);			/*�Ƴ���index��Ԫ��*/
	void Print();

private:
	BPlusTree* tree_;
	int block_num_;
	int rank_;
	char* buffer_;
	bool is_leaf_;
	bool is_new_node_;
};

#endif