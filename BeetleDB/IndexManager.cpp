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
	/* ��ȡҪ����������table_name */
	string tb_name = st.get_tb_name();
	Table *tb = catalog_m_->GetDB(db_name_)->GetTable(tb_name);
	if (tb == NULL) throw TableNotExistException();
	if (tb->GetIndexNum() != 0) throw OneIndexEachTableException();

	/* ֻ֧������������ֻ�ܶ�primary key�������� */
	Attribute *attr = tb->GetAttribute(st.get_column_name());
	if (attr->get_attr_type() != 1) throw IndexMustBeCreatedOnPrimaryKeyException();

	/* ���������ļ� */
	string file_name = catalog_m_->get_path() + db_name_ + "/" + st.get_index_name() + ".index";
	ofstream ofs(file_name.c_str(), ios::binary);
	ofs.close();

	/* ����һ�����������������Ӧ��������б��� */
	Index idx(st.get_index_name(), st.get_column_name(), attr->get_data_type(), attr->get_length(),
		(4 * 1024 - 12)/ (4 + attr->get_length()) / 2 - 1);
	tb->AddIndex(idx);

	/* ��ȡ���м�¼������B+�� */
	BPlusTree tree(tb->GetIndex(0), buffer_m_, catalog_m_, db_name_);	/*B+��������ָ������catalog_m_����Ϊcatalog_m_���������ݿ⣬���б�������������Ϣ����������Ϣ�����ܹ������Ծֲ����ݽṹ�������ø�����*/
	RecordManager *rm = new RecordManager(catalog_m_, buffer_m_, db_name_);
	
	int col_idx = tb->GetAttributeIndex(st.get_column_name());
	int block_num = tb->get_first_block_num();							/*��ȡ�ñ�ĵ�һ���*/

	for (int i = 0; i < tb->get_block_count(); i++)						/*ѭ��������п�����*/
	{
		BlockInfo *bp = rm->GetBlockInfo(tb, block_num);				/*��ȡ��block_num���������Ϣ*/
		for (int j = 0; j < bp->GetRecordCount(); j++)					/*ѭ����block_num�������м�¼������*/
		{			
			vector<TKey> tkey_value = rm->GetRecord(tb, block_num, j);	/*��ȡ��block_num���еĵ�j�����ݼ�¼*/
			tree.Add(tkey_value[col_idx], block_num, j);				/*����col_idx���������Բ���B+��*/			
		}
		block_num = bp->GetNextBlockNum();
	}
	delete rm;

	buffer_m_->WriteToDisk();											/*��B+���������ڵĻ����д�ش���*/
	catalog_m_->WriteArchiveFile();

	tree.Print();
}
