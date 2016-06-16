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

class BPlusTree;		/*B+树类*/
class BPlusTreeNode;	/*B+树节点类*/

typedef struct
{
	BPlusTreeNode* pnode;
	int index;	
	bool flag;
} FindNodeParam;				/*节点查找辅助结构*/

class BPlusTree
{
public:
	BPlusTree(Index* idx,BufferManager* bm, CatalogManager* cm,string dbname);
	~BPlusTree(void);
	
	Index* GetIndex();										/*获取本B+树的索引成员*/
	BufferManager* GetBufferManager();						/*获取本B+树的buffermanager*/
	CatalogManager* GetCatalogManager();					/*获取本B+树的目录管理器*/
	int get_degree();										/*获取度*/
	string get_db_name();									/*获取本B+树的数据库名*/

	bool Add(TKey& key, int block_num, int offset);			/*在第block_num个块上加元素key 偏移offset*/
	bool AdjustAfterAdd(int node);							/*加元素后调整B+树*/

	bool Remove(TKey key);									/*移除key元素*/
	bool AdjustAfterRemove(int node);						/*删元素后调整B+树*/

	FindNodeParam Search(int node, TKey &key);				/*【用于直至叶子节点的查询】由node开始搜索到key所在的叶子节点。ans.flag：true，该key在B+树中存在；false，该key在B+树中不存在*/
	FindNodeParam SearchBranch(int node, TKey &key);		/*【用于删节点时，内部节点变化的查询】从node开始，查询key所在的pnode及其index。FindNodeParam中的flag：true，pnode为叶子节点；false，pnode为内部节点*/
	BPlusTreeNode* GetNode(int num);						/*获取第num个节点。实现方法：获取文件系统中第num个块，该块中的data_数据即为该节点*/

	int GetVal(TKey key);									/*由key查询value值*/
	vector<int> GetVal(TKey key, int op_type);				/*由key查询value值，op_type可用于判定是否为范围查询*/
	int GetNewBlockNum();									/*idx_索引最大值增一*/

	void Print();
	void PrintNode(int num);

private:
	Index *idx_;											/*索引指针，描述了当前B+树的基本信息（头节点地址，属性及长度等）。当一张table建立了一个B+树索引，则该table将该索引信息加入索引列表中，这里的索引指针与table中的索引列表的某一项。*/
	int degree_;											/*度*/
	BufferManager *buffer_m_;								/*缓存管理器指针*/
	CatalogManager *catalog_m_;								/*目录管理器指针*/
	string db_name_;										/*该B+树所属的db名字*/
	void InitTree();										/*初始化：创建根节点，初始idx属性*/
};

class BPlusTreeNode
{
public:
	BPlusTreeNode(BPlusTree* tree,bool isnew,  int blocknum,  bool newleaf=false);
	~BPlusTreeNode();

	int get_block_num();

	TKey GetKeys(int index);			/*获取第index个key值*/
	int GetValues(int index);			/*获取第index个value值*/
	int GetNextLeaf();					/*获取下一个叶子节点*/
	int GetParent();					/*获取下一个父节点，得到的是该节点在buffer中的地址编号，是该节点所在的buffer的第8位数据*/
	int GetNodeType();					/*获取节点类型，即该节点所在buffer中的第0位数据*/					
	int GetCount();						/*获取节点的数据个数，即该节点所在buffer中的第4位数据*/
	bool GetIsLeaf();					/*判断是否是叶子节点*/

	void SetKeys(int index, TKey key);	/*将key.key_放入该节点的第index个键中*/
	void SetValues(int index, int val);	/**/
	void SetNextLeaf(int val);			/*设置叶子节点的值*/
	void SetParent(int val);			/*设置父节点的值*/
	void SetNodeType(int val);			/*节点类型：叶子节点为1，非叶子节点为0*/
	void SetCount(int val);				/*设置节点存储的元素个数*/
	void SetIsLeaf(bool val);			/*设置节点是否为叶子节点*/

	void GetBuffer();					/*从file中获取当前B+树所在的db中当前树索引的文件块*/

	bool Search(TKey key, int &index);	/*在B+树中搜索key所在的位置，并赋值到index中。返回值：true，index即为由key定位的节点地址；false，index即为指向下一层的指针。节点元素个数20以内，顺序查找；20以外，二分查找*/
	int Add(TKey &key);					/*先查找b+树中是否存在该key，否：将key插入其对应的位置中*/
	int Add(TKey &key, int &val);		/*插入KV对*/
	BPlusTreeNode* Split(TKey &key);	/*分裂*/

	bool IsRoot();						/*判断是否为根节点*/
	bool RemoveAt(int index);			/*移除第index个元素*/
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