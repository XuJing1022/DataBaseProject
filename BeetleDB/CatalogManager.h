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
	CatalogManager(string path);/*CatalogManager���캯��*/
	~CatalogManager(void);/*CatalogManager��������*/
	string get_path();/*��ȡ����path_*/
	vector<Database>& GetDBs();/*��ȡ����,get_databases() dbs_*/
	Database* GetDB(string database_name);/*����database����ȡ��ӦDatabase��*/
	void ReadArchiveFile();/*��ȡĿ¼�ĵ�*/
	void WriteArchiveFile();/*д��Ŀ¼�ĵ�*/
	void CreateDatabase(string database_name);/*Ŀ¼�и���database������һ��database*/
	void DeleteDatabase(string database_name);/*Ŀ¼�и���database��ɾ��һ��database*/
private:
	friend class boost::serialization::access;/*��Ԫ������Ϊ�����ô��л�����ܹ�����˽�г�Ա������Ҫ����һ����Ԫ��*/
	template<class Archive>/*���л��ĺ�������һ��������ɶ���ı�����ָ�*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & databases_;
	}
	string path_;//�洢database�ļ���·������
	vector<Database> databases_;//�洢���ݿ���ı���
};

class Database {
public:
	Database();/*Database�����������캯��*/
	Database(string database_name);/*Database���������캯��*/
	~Database();/*Database��������*/
	string get_database_name();/*��ȡ����database_name_*/
	vector<Table>& get_tables();/*��ȡ����tables_*/
	Table* GetTable(string table_name);/*����table����ȡ��Ӧ��Table��*/
	void CreateTable(SQLCreateTable& obj);/*����SQLCreateTable���󴴽�һ��table*/
	void DropTable(SQLDropTable& obj);/*����SQLDropTable����ɾ��һ��table*/
	void DropIndex(SQLDropIndex& obj);/*����SQLDropIndex���󴴽�һ��index*/
	bool CheckIfIndexExists(string index_name);/*�����������жϸ������Ƿ����*/
private:
	friend class boost::serialization::access;/*��Ԫ������Ϊ�����ô��л�����ܹ�����˽�г�Ա������Ҫ����һ����Ԫ��*/
	template<class Archive>/*���л��ĺ�������һ��������ɶ���ı�����ָ�*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & database_name_;
		ar & tables_;
	}
	string database_name_;//�洢���ݿ����ֵı���
	vector<Table> tables_;//�洢Table��ı���
};

class Table {
public:
	Table();/*Table�Ĺ��캯��*/
	~Table();/*Table����������*/
	string get_tb_name();/*��ȡ����table_name_*/
	void set_table_name(string table_name);/*���ñ���table_name_*/
	int get_record_length();/*��ȡ����record_length_*/
	void set_record_length(int len);/*���ñ���record_length_*/

	vector<Attribute>& GetAttributes();/*��ȡ����attributes*/
	Attribute* GetAttribute(string name);/*�����ֶ�����ȡattribute����*/
	int GetAttributeIndex(string name);/*�����ֶ�����ȡattribute�����λ��*/

	int get_first_block_num();/*��ȡ����first_block_num*/
	void set_first_block_num(int num);/*���ñ���first_block_num*/
	int get_first_rubbish_num();/*��ȡ����first_rubbish_num_*/
	void set_first_rubbish_num(int num);/*���ñ���first_rubbish_num_*/
	int get_block_count();/*��ȡ����block_count_*/

	unsigned long GetAttributeNum();/*��ȡ�ֶ�����*/
	void AddAttribute(Attribute& attr);/*����Attribute���󣬽��ö������Table������*/
	void IncreaseBlockCount();/*block_count++*/

	std::vector<Index>& get_indexs();/*��ȡ����indexs*/
	Index* GetIndex(int num);/*����indexλ�û�ȡ��Ӧ��Index����*/
	unsigned long GetIndexNum();/*��ȡIndex����*/
	void AddIndex(Index& idx);/*����Index���󣬽��ö�������Table������*/
private:
	friend class boost::serialization::access;/*��Ԫ������Ϊ�����ô��л�����ܹ�����˽�г�Ա������Ҫ����һ����Ԫ��*/
	template<class Archive>/*���л��ĺ�������һ��������ɶ���ı�����ָ�*/
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
	string table_name_;//�洢���ݱ����ֵı���
	int record_length_;//�洢��¼�ܳ��ȵı���
	int first_block_num_;//�洢��һ����ĵ�ַ�ı���
	int first_rubbish_num_;
	int block_count_;//�洢��������ı���

	std::vector<Attribute> attributes_;//�洢�ֶεı���
	std::vector<Index> indexs_;//�洢�����ı���
};

class Attribute
{
public:
	Attribute();/*Attribute���캯��*/
	~Attribute();/*Attribute��������*/
	string get_attr_name();/*��ȡ����attribute_name_*/
	void set_attribute_name(string name);/*���ñ���attribute_name_*/
	int get_attr_type();/*��ȡ����attribute_type_*/
	void set_attribute_type(int type);/*����attribute_type_*/
	int get_data_type();/*��ȡ����data_type_*/
	void set_data_type(int type);/*���ñ���data_type_*/
	int get_length();/*��ȡ����length_*/
	void set_length(int length);/*���ñ���length_*/
private:
	friend class boost::serialization::access;/*��Ԫ������Ϊ�����ô��л�����ܹ�����˽�г�Ա������Ҫ����һ����Ԫ��*/
	template<class Archive>/*���л��ĺ�������һ��������ɶ���ı�����ָ�*/
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & attribute_name_;
		ar & data_type_;
		ar & length_;
		ar & attribute_type_;
	}
	string attribute_name_;//�洢�ֶ����ı���
	int attribute_type_;//�洢�ֶ����͵ı���
	int data_type_;//�洢�������͵ı���
	int length_;//�洢���ֶγ��ȵĳ���
};

class Index {
public:
	Index();/*Index���캯��*/
	Index(std::string name, std::string attr_name, int keytype, int keylen, int rank);/*Index���������캯��*/
	string get_attr_name();/*��ȡ����attribute_name_*/
	string get_name();/*��ȡ����name_*/
	int get_key_len();/*��ȡ����key_length_*/
	int get_key_type();/*��ȡ����key_type_*/
	int get_rank();/*��ȡ����rank_*/
	int get_root();/*��ȡ����root_*/
	void set_root(int root);/*���ñ���root_*/
	int get_leaf_head();/*��ȡ����leaf_head_*/
	void set_leaf_head(int leaf_head);/*���ñ���leaf_head_*/
	int get_key_count();/*��ȡ����key_count_*/
	void set_key_count(int key_count);/*���ñ���key_count_*/
	int get_level();/*��ȡ����level_*/
	void set_level(int level);/*���ñ���level_*/
	int get_node_count();/*��ȡ����node_count_*/
	void set_node_count(int node_count);/*���ñ���node_count_*/

	int IncreaseMaxCount();/*max_count_++*/
	int IncreaseKeyCount();/*key_count_++*/
	int IncreaseNodeCount();/*node_count_++*/
	int IncreaseLevel();/*level_++*/
	int DecreaseKeyCount();/*key_count--*/
	int DecreaseNodeCount();/*node_count--*/
	int DecreaseLevel();/*level--*/
private:
	friend class boost::serialization::access;/*��Ԫ������Ϊ�����ô��л�����ܹ�����˽�г�Ա������Ҫ����һ����Ԫ��*/
	template<class Archive>/*���л��ĺ�������һ��������ɶ���ı�����ָ�*/
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
	int max_count_;//�洢�������ֵ�ı���
	int key_length_;//�洢�������ȵı���
	int key_type_;//�洢�������͵ı���
	int rank_;
	int rubbish_;
	int root_;
	int leaf_head_;/*Ҷ�ӽڵ��ͷ��*/
	int key_count_;
	int level_;
	int node_count_;
	string attribute_name_;//�洢�ֶ����ı���
	string name_;//�洢�������ı���
};

#endif