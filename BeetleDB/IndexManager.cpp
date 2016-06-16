//
// IndexManager.cpp
//
// Created by Xujing on 2016/6/15.
//
#include "IndexManager.h"
#include "Exceptions.h"
#include "RecordManager.h"

#include <string>
#include <fstream>
using namespace std;

IndexManager::IndexManager(CatalogManager *cm, BufferManager *bm, string dbname):catalog_m_(cm), buffer_m_(bm), db_name_(dbname){}

IndexManager::~IndexManager(void){}

void IndexManager::CreateIndex(SQLCreateIndex& st)
{
	/* 获取要创建索引的table_name */
	string tb_name = st.get_tb_name();
	Table *tb = catalog_m_->GetDB(db_name_)->GetTable(tb_name);
	if (tb == NULL) throw TableNotExistException();
	if (tb->GetIndexNum() != 0) throw OneIndexEachTableException();

	/* 只支持主索引，即只能对primary key创建索引 */
	Attribute *attr = tb->GetAttribute(st.get_column_name());
	if (attr->get_attr_type() != 1) throw IndexMustBeCreatedOnPrimaryKeyException();

	/* 创建索引文件 */
	string file_name = catalog_m_->get_path() + db_name_ + "/" + st.get_index_name() + ".index";
	ofstream ofs(file_name.c_str(), ios::binary);
	ofs.close();

	/* 创建一个索引，并添加至对应表的索引列表中 */
	Index idx(st.get_index_name(), st.get_column_name(), attr->get_data_type(), attr->get_length(),
		(4 * 1024 - 12)/ (4 + attr->get_length()) / 2 - 1);
	tb->AddIndex(idx);

	/* 获取所有记录并创建B+树 */
	BPlusTree tree(tb->GetIndex(0), buffer_m_, catalog_m_, db_name_);	/*B+树索引用指针引用catalog_m_，因为catalog_m_是所有数据库，所有表，及表中所有信息（如索引信息）的总管理，所以局部数据结构都会引用该索引*/
	RecordManager *rm = new RecordManager(catalog_m_, buffer_m_, db_name_);
	
	int col_idx = tb->GetAttributeIndex(st.get_column_name());
	int block_num = tb->get_first_block_num();							/*获取该表的第一块号*/

	for (int i = 0; i < tb->get_block_count(); i++)						/*循环表的所有块数遍*/
	{
		BlockInfo *bp = rm->GetBlockInfo(tb, block_num);				/*获取第block_num块的数据信息*/
		for (int j = 0; j < bp->GetRecordCount(); j++)					/*循环第block_num块中所有记录条数遍*/
		{			
			vector<TKey> tkey_value = rm->GetRecord(tb, block_num, j);	/*获取第block_num块中的第j条数据记录*/
			tree.Add(tkey_value[col_idx], block_num, j);				/*将第col_idx个列项属性插入B+树*/			
		}
		block_num = bp->GetNextBlockNum();
	}
	delete rm;

	buffer_m_->WriteToDisk();											/*将B+树索引所在的缓存块写回磁盘*/
	catalog_m_->WriteArchiveFile();

	tree.Print();
}
