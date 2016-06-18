//Implemented by Jin Xin
#include "CatalogManager.h"
#include <fstream>
#include <boost/filesystem.hpp>
//
#pragma region class 实现：CatalogManager
/*CatalogManager构造函数*/
CatalogManager::CatalogManager(string path)
{
	path_ = path;
	ReadArchiveFile();
}

/*CatalogManager析构函数*/
CatalogManager::~CatalogManager(void)
{
	WriteArchiveFile();
}

/*获取变量path_*/
string CatalogManager::get_path()
{
	return path_;
}

/*获取变量databases_*/
vector<Database>& CatalogManager::GetDBs()
{
	return databases_;
}

/*根据database名获取对应Database类*/
Database* CatalogManager::GetDB(string database_name)
{
	for (auto database = databases_.begin(); database != databases_.end(); database++)
		if (database->get_database_name() == database_name)
			return &(*database);
	return NULL;
}

/*读取目录文档*/
void CatalogManager::ReadArchiveFile()
{
	string file_name = path_ + "catalog";
	boost::filesystem::path file_path(file_name);/*获取目录文档地址*/

	if (boost::filesystem::exists(file_path))/*判断目录文档地址是否存在*/
	{
		ifstream my_ifs;/*创建读取流*/
		my_ifs.open(file_name.c_str(), ios::binary); /*打开读取文件*/
		boost::archive::binary_iarchive iar(my_ifs); /*二进制文件输入归档类*/
		iar >> (*this);    /*恢复该对象*/
		my_ifs.close();/*关闭读取流*/
	}
}

/*写入目录文档*/
void CatalogManager::WriteArchiveFile()
{
	string file_name = path_ + "catalog";
	ofstream my_ofs;
	my_ofs.open(file_name.c_str(), ios::binary);/*打开输入文件*/
	boost::archive::binary_oarchive oar(my_ofs);/*二进制文件输出归档类*/
	oar << (*this);/*保存本对象*/
	my_ofs.close();/*关闭输入流*/
}

/*创建数据库，往私有变量database_中添加一个新建的database类*/
void CatalogManager::CreateDatabase(string database_name)
{
	databases_.push_back(Database(database_name));
}

/*删除数据库，在私有变量database_中找到该database，并删除*/
void CatalogManager::DeleteDatabase(string database_name)
{
	for (auto database = 0; database < databases_.size(); database++)
		if (databases_[database].get_database_name() == database_name)
			databases_.erase(databases_.begin() + database);
}

#pragma endregion

#pragma region class 实现：Database

/*Database不带参数构造函数*/
Database::Database()
{
	;
}

/*Database带参数构造函数*/
Database::Database(string database_name)
{
	database_name_ = database_name;
}

/*Database析构函数*/
Database::~Database()
{

}

/*获取变量database_name_*/
string Database::get_database_name()
{
	return database_name_;
}

/*获取变量tables_*/
vector<Table>& Database::get_tables() 
{
	return tables_; 
}

/*根据table名获取对应的Table类*/
Table* Database::GetTable(string table_name)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)
	{
		if (table->get_tb_name() == table_name)
			return &(*table);
	}
	return NULL;
}

/*根据SQLCreateTable对象创建一个table*/
void Database::CreateTable(SQLCreateTable& sql_obj)
{
	int record_length = 0;
	Table table;
	vector<Attribute> attributes = sql_obj.get_attributes();/*获取attributes变量*/
	for (auto attribute = attributes.begin(); attribute != attributes.end(); attribute++)/*赋值attributes_*/
	{
		table.AddAttribute((*attribute));
		record_length = record_length+(*attribute).get_length();
	}
	table.set_table_name(sql_obj.get_table_name());/*赋值table_name_*/
	table.set_record_length(record_length);/*赋值record_length_*/
	tables_.push_back(table);/*将该table插入到table列表中*/
}

/*根据SQLDropTable对象删除一张table*/
void Database::DropTable(SQLDropTable& sql_obj)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)/*遍历该数据库所有表*/
	{
		if (table->get_tb_name() == sql_obj.get_table_name())/*获取该表的表名判断是否等于要查询的表名*/
		{
			tables_.erase(table);
			return;
		}
	}
}

/*根据SQLDropIndex对象创建一个index*/
void Database::DropIndex(SQLDropIndex& sql_obj)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)/*遍历该数据库所有表*/
	{
		for (auto index = table->get_indexs().begin(); index != table->get_indexs().end(); index++)/*遍历该表所有索引*/
		{
			if (index->get_name() == sql_obj.get_index_name())/*获取该索引名，判断是否等于要查询的索引名*/
			{
				table->get_indexs().erase(index);
				return;
			}
		}
	}
}

/*根据索引名判断该索引是否存在*/
bool Database::CheckIfIndexExists(string index_name)
{
	for (auto table = tables_.begin(); table != tables_.end(); table++)/*遍历该数据库所有表*/
	{
		for (auto index = table->get_indexs().begin(); index != table->get_indexs().end(); index++)/*遍历该表所有索引*/
		{
			if (index->get_name() == index_name)/*获取该索引名，判断是否等于要查询的索引名*/
			{
				return true;
			}
		}
	}
	return false;
}

#pragma endregion

#pragma region class 实现：Table
/*Table的构造函数*/
Table::Table()
{
	table_name_ = "";
	record_length_ = -1;
	first_block_num_ = -1;
	first_rubbish_num_ = -1;
	block_count_ = 0;
}

/*Table的析构函数*/
Table::~Table() 
{
}

/*获取变量table_name_*/
string Table::get_tb_name() 
{
	return table_name_; 
}

/*设置变量table_name_*/
void Table::set_table_name(string table_name) 
{
	table_name_ = table_name;
}

/*获取变量record_length_*/
int Table::get_record_length() 
{
	return record_length_;
}

/*设置变量record_length_*/
void Table::set_record_length(int len) 
{
	record_length_ = len;
}

/*获取变量attributes*/
vector<Attribute>& Table::GetAttributes() 
{ 
	return attributes_;
}

/*根据字段名获取attribute对象*/
Attribute* Table::GetAttribute(string name)
{
	for (auto attr = attributes_.begin(); attr != attributes_.end(); attr++)
	{
		if (attr->get_attr_name() == name)
			return &(*attr);
	}
	return NULL;
}

/*根据字段名获取attribute对象的位置*/
int Table::GetAttributeIndex(string name)
{
	for (unsigned int i = 0; i < attributes_.size(); i++)
	{
		if (attributes_[i].get_attr_name() == name)
			return i;
	}
	return -1;
}

/*获取变量first_block_num*/
int Table::get_first_block_num()
{
	return first_block_num_;
}

/*设置变量first_block_num*/
void Table::set_first_block_num(int num)
{ 
	first_block_num_ = num; 
}

/*获取变量first_rubbish_num_*/
int Table::get_first_rubbish_num() 
{ 
	return first_rubbish_num_; 
}

/*设置变量first_rubbish_num_*/
void Table::set_first_rubbish_num(int num) 
{
	first_rubbish_num_ = num;
}

/*获取变量block_count_*/
int Table::get_block_count() 
{
	return block_count_;
}

/*获取字段数量*/
unsigned long Table::GetAttributeNum() 
{ 
	return attributes_.size();
}

/*传入Attribute对象，将该对象插入Table对象中*/
void Table::AddAttribute(Attribute& attr) 
{
	attributes_.push_back(attr); 
}

/*block_count++*/
void Table::IncreaseBlockCount() 
{ 
	block_count_++; 
}

/*获取变量indexs*/
vector<Index>& Table::get_indexs()
{ 
	return indexs_;
}

/*根据index位置获取对应的Index对象*/
Index* Table::GetIndex(int num) 
{
	return &(indexs_[num]);
}

/*获取Index数量*/
unsigned long Table::GetIndexNum()
{ 
	return indexs_.size(); 
}

/*传入Index对象，将该对象插入该Table对象中*/
void Table::AddIndex(Index& idx) 
{ 
	indexs_.push_back(idx); 
}
#pragma endregion

#pragma region class 实现：Attribute
/*Attribute构造函数*/
Attribute::Attribute()
{
	attribute_name_ = "";
	data_type_ = -1;
	length_ = -1;
	attribute_type_ = 0;
}

/*Attribute析构函数*/
Attribute::~Attribute()
{
	;
}

/*获取变量attribute_name_*/
string Attribute::get_attr_name()
{
	return attribute_name_;
}

/*设置变量attribute_name_*/
void Attribute::set_attribute_name(string name)
{
	attribute_name_ = name;
}

/*获取变量attribute_type_*/
int Attribute::get_attr_type()
{
	return attribute_type_;
}

/*设置attribute_type_*/
void Attribute::set_attribute_type(int type)
{
	attribute_type_ = type;
}

/*获取变量data_type_*/
int Attribute::get_data_type()
{
	return data_type_;
}

/*设置变量data_type_*/
void Attribute::set_data_type(int type)
{
	data_type_ = type;
}

/*获取变量length_*/
int Attribute::get_length()
{
	return length_;
}

/*设置变量length_*/
void Attribute::set_length(int length)
{
	length_ = length;
}
#pragma endregion

#pragma region class 实现：Index
/*Index构造函数*/
Index::Index() 
{
	;
}

/*Index带参数构造函数*/
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

/*获取变量attribute_name_*/
string Index::get_attr_name()
{
	return attribute_name_; 
}

/*获取变量name_*/
string Index::get_name()
{
	return name_;
}

/*获取变量key_length_*/
int Index::get_key_len() 
{
	return key_length_;
}

/*获取变量key_type_*/
int Index::get_key_type()
{
	return key_type_;
}

/*获取变量rank_*/
int Index::get_rank()
{
	return rank_;
}

/*获取变量root_*/
int Index::get_root() 
{
	return root_;
}

/*设置变量root_*/
void Index::set_root(int root)
{ 
	root_ = root;
}

/*获取变量leaf_head_*/
int Index::get_leaf_head()
{
	return leaf_head_;
}

/*设置变量leaf_head_*/
void Index::set_leaf_head(int leaf_head) 
{
	leaf_head_ = leaf_head;
}

/*获取变量key_count_*/
int Index::get_key_count()
{ 
	return key_count_;
}

/*设置变量key_count_*/
void Index::set_key_count(int key_count) 
{
	key_count_ = key_count;
}

/*获取变量level_*/
int Index::get_level() 
{
	return level_;
}

/*设置变量level_*/
void Index::set_level(int level)
{
	level_ = level;
}

/*获取变量node_count_*/
int Index::get_node_count() 
{
	return node_count_;
}

/*设置变量node_count_*/
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
