//Implemented by Lai ZhengMin

#include "BlockInfo.h"
#include "ConstValue.h"
#include <fstream>

using namespace std;

BlockInfo::BlockInfo(int num) :dirty_(false), next_(NULL), file_(NULL), age_(0), block_num_(num)
{
	//һ����4KB
	data_ = new char[4 * 1024];
}

BlockInfo::~BlockInfo()
{
	delete[]data_;
}
FileInfo* BlockInfo::GetFile()
{
	return file_;
}
//
void BlockInfo::SetFile(FileInfo *f)
{
	file_ = f;
}
//�õ���ǰ��ı��
int BlockInfo::get_block_num()
{
	return block_num_;
}
void BlockInfo::set_block_num(int num)
{
	block_num_ = num;
}
//���ؿ���Ϣ
char* BlockInfo::get_data()
{
	return data_;
}

bool BlockInfo::get_dirty()
{
	return dirty_;
}
void BlockInfo::set_dirty(bool dt)
{
	dirty_ = dt;
}

long BlockInfo::get_age()
{
	return age_;
}

BlockInfo* BlockInfo::GetNext()
{
	return next_;
}
void BlockInfo::SetNext(BlockInfo* block)
{
	next_ = block;
}

void BlockInfo::IncreaseAge()
{
	++age_;
}
//��������Ϊ0
void BlockInfo::ResetAge()
{
	age_ = 0;
}
//int *����4���ֽڣ�headerǰ4����0-3���ֽڴ������һ����ı��
int  BlockInfo::GetPrevBlockNum()
{
	return *(int*)(data_);
}
void BlockInfo::SetPrevBlockNum(int num)
{
	*(int*)(data_) = num;
}
//header ��4-7 �ֽڴ������һ����ı��
int  BlockInfo::GetNextBlockNum()
{
	return *(int*)(data_ + 4);
}
void BlockInfo::SetNextBlockNum(int num)
{
	*(int*)(data_ + 4) = num;
}
//header ��8-11���ֽڴ���Ǹÿ��������ļ�¼����Ŀ
int  BlockInfo::GetRecordCount()
{
	return *(int*)(data_ + 8);
}
void BlockInfo::SetRecordCount(int count)
{
	*(int*)(data_ + 8) = count;
}
//����¼��Ŀ��һ
void BlockInfo::DecreaseRecordCount()
{
	*(int*)(data_ + 8) = *(int*)(data_ + 8) - 1;
}
//�õ����д洢��¼���׵�ַ
char* BlockInfo::GetContentAdress()
{
	return data_ + 12;
}
//��ָ��·���а�������Ϣ�����Ϣ�������ļ�����data_��
void BlockInfo::ReadInfo(string path)
{
	path += file_->get_db_name() + "/" + file_->get_file_name();
	//����������ļ�
	if (file_->get_type() == FORMAT_INDEX) path += ".index";
	//��¼�ļ�
	else path += ".records";

	ifstream ifs(path, ios::binary);
	//�ҵ��ÿ������ļ����λ�ã����ݿ�ţ�
	ifs.seekg(block_num_ * 4 * 1024);
	//��ȡ����Ϣ
	ifs.read(data_, 4 * 1024);
	ifs.close();
}
//��data_��Ϣд��ָ��·���������ļ����¼�ļ���
void BlockInfo::WriteInfo(string path)
{
	path += file_->get_db_name() + "/" + file_->get_file_name();
	if (file_->get_type() == FORMAT_INDEX) path += ".index";
	else path += ".records";

	ofstream ofs(path, ios::binary);
	//�ҵ��ÿ������ļ����λ��
	ofs.seekp(block_num_ * 4 * 1024);
	ofs.write(data_, 4 * 1024);
	ofs.close();
}