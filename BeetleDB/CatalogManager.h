#pragma once
#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_

#include <vector>
#include <string>
#include "SQLStatement.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

class Database;
class Table;
class Attribute;
class Index;
class SQLCreateTable;
class SQLDropTable;
class SQLDropIndex;

class CatalogManager
{
public:
	CatalogManager(string path);/*CatalogManager构造函数*/
	~CatalogManager(void);/*CatalogManager析构函数*/
	string get_path();/*获取变量path_*/
	vector<Database>& GetDBs();/*获取变量,get_databases() dbs_*/
	Database* GetDB(string database_name);/*根据database名获取对应Database类*/
	void ReadArchiveFile();/*读取目录文档*/
	void WriteArchiveFile();/*写入目录文档*/
	void CreateDatabase(string database_name);/*目录中根据database名创键一个database*/
	void DeleteDatabase(string database_name);/*目录中根据database名删除一个database*/
private:
	friend class boost::serialization::access;/*友元函数。为了能让串行化类库能够访问私有成员，所以要声明一个友元类*/
	template<class Archive>/*串行化的函数，这一个函数完成对象的保存与恢复*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & databases_;
	}
	string path_;//存储database文件的路径变量
	vector<Database> databases_;//存储数据库类的变量
};

class Database {
public:
	Database();/*Database不带参数构造函数*/
	Database(string database_name);/*Database带参数构造函数*/
	~Database();/*Database析构函数*/
	string get_database_name();/*获取变量database_name_*/
	vector<Table>& get_tables();/*获取变量tables_*/
	Table* GetTable(string table_name);/*根据table名获取对应的Table类*/
	void CreateTable(SQLCreateTable& obj);/*根据SQLCreateTable对象创建一个table*/
	void DropTable(SQLDropTable& obj);/*根据SQLDropTable对象删除一张table*/
	void DropIndex(SQLDropIndex& obj);/*根据SQLDropIndex对象创建一个index*/
	bool CheckIfIndexExists(string index_name);/*根据索引名判断该索引是否存在*/
private:
	friend class boost::serialization::access;/*友元函数。为了能让串行化类库能够访问私有成员，所以要声明一个友元类*/
	template<class Archive>/*串行化的函数，这一个函数完成对象的保存与恢复*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & database_name_;
		ar & tables_;
	}
	string database_name_;//存储数据库名字的变量
	vector<Table> tables_;//存储Table类的变量
};

class Table {
public:
	Table();/*Table的构造函数*/
	~Table();/*Table的析构函数*/
	string get_tb_name();/*获取变量table_name_*/
	void set_table_name(string table_name);/*设置变量table_name_*/
	int get_record_length();/*获取变量record_length_*/
	void set_record_length(int len);/*设置变量record_length_*/

	vector<Attribute>& GetAttributes();/*获取变量attributes*/
	Attribute* GetAttribute(string name);/*根据字段名获取attribute对象*/
	int GetAttributeIndex(string name);/*根据字段名获取attribute对象的位置*/

	int get_first_block_num();/*获取变量first_block_num*/
	void set_first_block_num(int num);/*设置变量first_block_num*/
	int get_first_rubbish_num();/*获取变量first_rubbish_num_*/
	void set_first_rubbish_num(int num);/*设置变量first_rubbish_num_*/
	int get_block_count();/*获取变量block_count_*/

	unsigned long GetAttributeNum();/*获取字段数量*/
	void AddAttribute(Attribute& attr);/*传入Attribute对象，将该对象插入Table对象中*/
	void IncreaseBlockCount();/*block_count++*/

	std::vector<Index>& get_indexs();/*获取变量indexs*/
	Index* GetIndex(int num);/*根据index位置获取对应的Index对象*/
	unsigned long GetIndexNum();/*获取Index数量*/
	void AddIndex(Index& idx);/*传入Index对象，将该对象插入该Table对象中*/
private:
	friend class boost::serialization::access;/*友元函数。为了能让串行化类库能够访问私有成员，所以要声明一个友元类*/
	template<class Archive>/*串行化的函数，这一个函数完成对象的保存与恢复*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & table_name_;
		ar & record_length_;
		ar & first_block_num_;
		ar & first_rubbish_num_;
		ar & block_count_;
		ar & attributes_;
		ar & indexs_;
	}
	string table_name_;//存储数据表名字的变量
	int record_length_;//存储记录总长度的变量
	int first_block_num_;//存储第一个块的地址的变量
	int first_rubbish_num_;
	int block_count_;//存储块的数量的变量

	std::vector<Attribute> attributes_;//存储字段的变量
	std::vector<Index> indexs_;//存储索引的变量
};

class Attribute
{
public:
	Attribute();/*Attribute构造函数*/
	~Attribute();/*Attribute析构函数*/
	string get_attr_name();/*获取变量attribute_name_*/
	void set_attribute_name(string name);/*设置变量attribute_name_*/
	int get_attr_type();/*获取变量attribute_type_*/
	void set_attribute_type(int type);/*设置attribute_type_*/
	int get_data_type();/*获取变量data_type_*/
	void set_data_type(int type);/*设置变量data_type_*/
	int get_length();/*获取变量length_*/
	void set_length(int length);/*设置变量length_*/
private:
	friend class boost::serialization::access;/*友元函数。为了能让串行化类库能够访问私有成员，所以要声明一个友元类*/
	template<class Archive>/*串行化的函数，这一个函数完成对象的保存与恢复*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & attribute_name_;
		ar & data_type_;
		ar & length_;
		ar & attribute_type_;
	}
	string attribute_name_;//存储字段名的变量
	int attribute_type_;//用于标识是否为主键
	int data_type_;//存储数据类型的变量
	int length_;//存储该字段长度的长度
};

class Index {
public:
	Index();/*Index构造函数*/
	Index(std::string name, std::string attr_name, int keytype, int keylen, int rank);/*Index带参数构造函数*/
	string get_attr_name();/*获取变量attribute_name_*/
	string get_name();/*获取变量name_*/
	int get_key_len();/*获取变量key_length_*/
	int get_key_type();/*获取变量key_type_*/
	int get_rank();/*获取变量rank_*/
	int get_root();/*获取变量root_*/
	void set_root(int root);/*设置变量root_*/
	int get_leaf_head();/*获取变量leaf_head_*/
	void set_leaf_head(int leaf_head);/*设置变量leaf_head_*/
	int get_key_count();/*获取变量key_count_*/
	void set_key_count(int key_count);/*设置变量key_count_*/
	int get_level();/*获取变量level_*/
	void set_level(int level);/*设置变量level_*/
	int get_node_count();/*获取变量node_count_*/
	void set_node_count(int node_count);/*设置变量node_count_*/

	int IncreaseMaxCount();/*max_count_++*/
	int IncreaseKeyCount();/*key_count_++*/
	int IncreaseNodeCount();/*node_count_++*/
	int IncreaseLevel();/*level_++*/
	int DecreaseKeyCount();/*key_count--*/
	int DecreaseNodeCount();/*node_count--*/
	int DecreaseLevel();/*level--*/
private:
	friend class boost::serialization::access;/*友元函数。为了能让串行化类库能够访问私有成员，所以要声明一个友元类*/
	template<class Archive>/*串行化的函数，这一个函数完成对象的保存与恢复*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & max_count_;
		ar & attribute_name_;
		ar & name_;
		ar & key_length_;
		ar & key_type_;
		ar & rank_;
		ar & rubbish_;
		ar & root_;
		ar & leaf_head_;
		ar & key_count_;
		ar & level_;
		ar & node_count_;
	}
	int max_count_;//存储数据最大值的变量
	int key_length_;//存储主键长度的变量
	int key_type_;//存储主键类型的变量
	int rank_;
	int rubbish_;
	int root_;
	int leaf_head_;
	int key_count_;
	int level_;
	int node_count_;
	string attribute_name_;//存储字段名的变量
	string name_;//存储索引名的变量
};

#endif
