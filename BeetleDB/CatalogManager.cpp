//Implemented by Jin Xin
#include "CatalogManager.h"
#include <fstream>
#include <boost/filesystem.hpp>
//
#pragma region class ʵ�֣�CatalogManager
/*CatalogManager���캯��*/
CatalogManager::CatalogManager(string path)
{
	path_ = path;
	ReadArchiveFile();
}

/*CatalogManager��������*/
CatalogManager::~CatalogManager(void)
{
	WriteArchiveFile();
}

/*��ȡ����path_*/
string CatalogManager::get_path()
{
	return path_;
}

/*��ȡ����databases_*/
vector<Database>& CatalogManager::GetDBs()
{
	return databases_;
}

/*����database����ȡ��ӦDatabase��*/
Database* CatalogManager::GetDB(string database_name)
{
	for (auto database = databases_.begin(); database != databases_.end(); database++)
		if (database->get_database_name() == database_name)
			return &(*database);
	return NULL;
}

/*��ȡĿ¼�ĵ�*/
void CatalogManager::ReadArchiveFile()
{
	string file_name = path_ + "catalog";
	boost::filesystem::path file_path(file_name);/*��ȡĿ¼�ĵ���ַ*/

	if (boost::filesystem::exists(file_path))/*�ж�Ŀ¼�ĵ���ַ�Ƿ����*/
	{
		ifstream my_ifs;/*������ȡ��*/
		my_ifs.open(file_name.c_str(), ios::binary); /*�򿪶�ȡ�ļ�*/
		boost::archive::binary_iarchive iar(my_ifs); /*�������ļ�����鵵��*/
		iar >> (*this);    /*�ָ��ö���*/
		my_ifs.close();/*�رն�ȡ��*/
	}
}

/*д��Ŀ¼�ĵ�*/
void CatalogManager::WriteArchiveFile()
{
	string file_name = path_ + "catalog";
	ofstream my_ofs;
	my_ofs.open(file_name.c_str(), ios::binary);/*�������ļ�*/
	boost::archive::binary_oarchive oar(my_ofs);/*�������ļ�����鵵��*/
	oar << (*this);/*���汾����*/
	my_ofs.close();/*�ر�������*/
}

/*�������ݿ⣬��˽�б���database_�����һ���½���database��*/
void CatalogManager::CreateDatabase(string database_name)
{
	databases_.push_back(Database(database_name));
}

/*ɾ�����ݿ⣬��˽�б���database_���ҵ���database����ɾ��*/
void CatalogManager::DeleteDatabase(string database_name)
{
	for (auto database = 0; database < databases_.size(); database++)
		if (databases_[database].get_database_name() == database_name)
			databases_.erase(databases_.begin() + database);
}

#pragma endregion

#pragma region class ʵ�֣�Database

/*Database�����������캯��*/
Database::Database()
{
	;
}

/*Database���������캯��*/
Database::Database(string database_name)
{
	database_name_ = database_name;
}

/*Database��������*/
Database::~Database()
{

}

/*��ȡ����database_name_*/
string Database::get_database_name()
{
	return database_name_;
}

/*��ȡ����tables_*/
vector<Table>& Database::get_tables()
{
	return tables_;
}

/*����table����ȡ��Ӧ��Table��*/
Table* Database::GetTable(string table_name)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)
	{
		if (table->get_tb_name() == table_name)
			return &(*table);
	}
	return NULL;
}

/*����SQLCreateTable���󴴽�һ��table*/
void Database::CreateTable(SQLCreateTable& sql_obj)
{
	int record_length = 0;
	Table table;
	vector<Attribute> attributes = sql_obj.get_attributes();/*��ȡattributes����*/
	for (auto attribute = attributes.begin(); attribute != attributes.end(); attribute++)/*��ֵattributes_*/
	{
		table.AddAttribute((*attribute));
		record_length = record_length + (*attribute).get_length();
	}
	table.set_table_name(sql_obj.get_table_name());/*��ֵtable_name_*/
	table.set_record_length(record_length);/*��ֵrecord_length_*/
	tables_.push_back(table);/*����table���뵽table�б���*/
}

/*����SQLDropTable����ɾ��һ��table*/
void Database::DropTable(SQLDropTable& sql_obj)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)/*���������ݿ����б�*/
	{
		if (table->get_tb_name() == sql_obj.get_table_name())/*��ȡ�ñ�ı����ж��Ƿ����Ҫ��ѯ�ı���*/
		{
			tables_.erase(table);
			return;
		}
	}
}

/*����SQLDropIndex���󴴽�һ��index*/
void Database::DropIndex(SQLDropIndex& sql_obj)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)/*���������ݿ����б�*/
	{
		for (auto index = table->get_indexs().begin(); index != table->get_indexs().end(); index++)/*�����ñ���������*/
		{
			if (index->get_name() == sql_obj.get_index_name())/*��ȡ�����������ж��Ƿ����Ҫ��ѯ��������*/
			{
				table->get_indexs().erase(index);
				return;
			}
		}
	}
}

/*�����������жϸ������Ƿ����*/
bool Database::CheckIfIndexExists(string index_name)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)/*���������ݿ����б�*/
	{
		for (auto index = table->get_indexs().begin(); index != table->get_indexs().end(); index++)/*�����ñ���������*/
		{
			if (index->get_name() == index_name)/*��ȡ�����������ж��Ƿ����Ҫ��ѯ��������*/
			{
				return true;
			}
		}
	}
	return false;
}

#pragma endregion

#pragma region class ʵ�֣�Table
/*Table�Ĺ��캯��*/
Table::Table()
{
	table_name_ = "";
	record_length_ = -1;
	first_block_num_ = -1;
	first_rubbish_num_ = -1;
	block_count_ = 0;
}

/*Table����������*/
Table::~Table()
{
}

/*��ȡ����table_name_*/
string Table::get_tb_name()
{
	return table_name_;
}

/*���ñ���table_name_*/
void Table::set_table_name(string table_name)
{
	table_name_ = table_name;
}

/*��ȡ����record_length_*/
int Table::get_record_length()
{
	return record_length_;
}

/*���ñ���record_length_*/
void Table::set_record_length(int len)
{
	record_length_ = len;
}

/*��ȡ����attributes*/
vector<Attribute>& Table::GetAttributes()
{
	return attributes_;
}

/*�����ֶ�����ȡattribute����*/
Attribute* Table::GetAttribute(string name)
{
	for (auto attr = attributes_.begin(); attr != attributes_.end(); attr++)
	{
		if (attr->get_attr_name() == name)
			return &(*attr);
	}
	return NULL;
}

/*�����ֶ�����ȡattribute�����λ��*/
int Table::GetAttributeIndex(string name)
{
	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		if (attributes_[i].get_attr_name() == name)
			return i;
	}
	return -1;
}

/*��ȡ����first_block_num*/
int Table::get_first_block_num()
{
	return first_block_num_;
}

/*���ñ���first_block_num*/
void Table::set_first_block_num(int num)
{
	first_block_num_ = num;
}

/*��ȡ����first_rubbish_num_*/
int Table::get_first_rubbish_num()
{
	return first_rubbish_num_;
}

/*���ñ���first_rubbish_num_*/
void Table::set_first_rubbish_num(int num)
{
	first_rubbish_num_ = num;
}

/*��ȡ����block_count_*/
int Table::get_block_count()
{
	return block_count_;
}

/*��ȡ�ֶ�����*/
unsigned long Table::GetAttributeNum()
{
	return attributes_.size();
}

/*����Attribute���󣬽��ö������Table������*/
void Table::AddAttribute(Attribute& attr)
{
	attributes_.push_back(attr);
}

/*block_count++*/
void Table::IncreaseBlockCount()
{
	block_count_++;
}

/*��ȡ����indexs*/
vector<Index>& Table::get_indexs()
{
	return indexs_;
}

/*����indexλ�û�ȡ��Ӧ��Index����*/
Index* Table::GetIndex(int num)
{
	return &(indexs_[num]);
}

/*��ȡIndex����*/
unsigned long Table::GetIndexNum()
{
	return indexs_.size();
}

/*����Index���󣬽��ö�������Table������*/
void Table::AddIndex(Index& idx)
{
	indexs_.push_back(idx);
}
#pragma endregion

#pragma region class ʵ�֣�Attribute
/*Attribute���캯��*/
Attribute::Attribute()
{
	attribute_name_ = "";
	data_type_ = -1;
	length_ = -1;
	attribute_type_ = 0;
}

/*Attribute��������*/
Attribute::~Attribute()
{
	;
}

/*��ȡ����attribute_name_*/
string Attribute::get_attr_name()
{
	return attribute_name_;
}

/*���ñ���attribute_name_*/
void Attribute::set_attribute_name(string name)
{
	attribute_name_ = name;
}

/*��ȡ����attribute_type_*/
int Attribute::get_attr_type()
{
	return attribute_type_;
}

/*����attribute_type_*/
void Attribute::set_attribute_type(int type)
{
	attribute_type_ = type;
}

/*��ȡ����data_type_*/
int Attribute::get_data_type()
{
	return data_type_;
}

/*���ñ���data_type_*/
void Attribute::set_data_type(int type)
{
	data_type_ = type;
}

/*��ȡ����length_*/
int Attribute::get_length()
{
	return length_;
}

/*���ñ���length_*/
void Attribute::set_length(int length)
{
	length_ = length;
}
#pragma endregion

#pragma region class ʵ�֣�Index
/*Index���캯��*/
Index::Index()
{
	;
}

/*Index���������캯��*/
Index::Index(std::string name, std::string attr_name, int keytype, int keylen, int rank)
{
	attribute_name_ = attr_name;
	name_ = name;
	key_count_ = 0;
	level_ = -1;
	root_ = -1;
	leaf_head_ = -1;
	key_type_ = keytype;
	key_length_ = keylen;
	rank_ = rank;
	rubbish_ = -1;
	max_count_ = 0;
}

/*��ȡ����attribute_name_*/
string Index::get_attr_name()
{
	return attribute_name_;
}

/*��ȡ����name_*/
string Index::get_name()
{
	return name_;
}

/*��ȡ����key_length_*/
int Index::get_key_len()
{
	return key_length_;
}

/*��ȡ����key_type_*/
int Index::get_key_type()
{
	return key_type_;
}

/*��ȡ����rank_*/
int Index::get_rank()
{
	return rank_;
}

/*��ȡ����root_*/
int Index::get_root()
{
	return root_;
}

/*���ñ���root_*/
void Index::set_root(int root)
{
	root_ = root;
}

/*��ȡ����leaf_head_*/
int Index::get_leaf_head()
{
	return leaf_head_;
}

/*���ñ���leaf_head_*/
void Index::set_leaf_head(int leaf_head)
{
	leaf_head_ = leaf_head;
}

/*��ȡ����key_count_*/
int Index::get_key_count()
{
	return key_count_;
}

/*���ñ���key_count_*/
void Index::set_key_count(int key_count)
{
	key_count_ = key_count;
}

/*��ȡ����level_*/
int Index::get_level()
{
	return level_;
}

/*���ñ���level_*/
void Index::set_level(int level)
{
	level_ = level;
}

/*��ȡ����node_count_*/
int Index::get_node_count()
{
	return node_count_;
}

/*���ñ���node_count_*/
void Index::set_node_count(int node_count)
{
	node_count_ = node_count;
}

/*max_count_++*/
int Index::IncreaseMaxCount()
{
	return max_count_++;
}

/*key_count_++*/
int Index::IncreaseKeyCount()
{
	return key_count_++;
}

/*node_count_++*/
int Index::IncreaseNodeCount()
{
	return node_count_++;
}

/*level_++*/
int Index::IncreaseLevel()
{
	return level_++;
}

/*key_count_--*/
int Index::DecreaseKeyCount()
{
	return key_count_--;
}

/*node_count_--*/
int Index::DecreaseNodeCount()
{
	return node_count_--;
}

/*level_--*/
int Index::DecreaseLevel()
{
	return level_--;
}
#pragma endregion
