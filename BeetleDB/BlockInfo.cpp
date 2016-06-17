//Implemented by Lai ZhengMin

#include "BlockInfo.h"
#include "ConstValue.h"
#include <fstream>

using namespace std;

BlockInfo::BlockInfo(int num) :dirty_(false), next_(NULL), file_(NULL), age_(0), block_num_(num)
{
	//一个块4KB
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
//拿到当前块的编号
int BlockInfo::get_block_num()
{
	return block_num_;
}
void BlockInfo::set_block_num(int num)
{
	block_num_ = num;
}
//返回块信息
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
//重置年龄为0
void BlockInfo::ResetAge()
{
	age_ = 0;
}
//int *，读4个字节，header前4个（0-3）字节存的是上一个块的编号
int  BlockInfo::GetPrevBlockNum()
{
	return *(int*)(data_);
}
void BlockInfo::SetPrevBlockNum(int num)
{
	*(int*)(data_) = num;
}
//header 第4-7 字节存的是下一个块的编号
int  BlockInfo::GetNextBlockNum()
{
	return *(int*)(data_ + 4);
}
void BlockInfo::SetNextBlockNum(int num)
{
	*(int*)(data_ + 4) = num;
}
//header 第8-11个字节存的是该块所包含的记录的数目
int  BlockInfo::GetRecordCount()
{
	return *(int*)(data_ + 8);
}
void BlockInfo::SetRecordCount(int count)
{
	*(int*)(data_ + 8) = count;
}
//将记录数目减一
void BlockInfo::DecreaseRecordCount()
{
	*(int*)(data_ + 8) = *(int*)(data_ + 8) - 1;
}
//拿到块中存储记录的首地址
char* BlockInfo::GetContentAdress()
{
	return data_ + 12;
}
//从指定路径中把索引信息或块信息二进制文件读到data_中
void BlockInfo::ReadInfo(string path)
{
	path += file_->get_db_name() + "/" + file_->get_file_name();
	//如果是索引文件
	if (file_->get_type() == FORMAT_INDEX) path += ".index";
	//记录文件
	else path += ".records";

	ifstream ifs(path, ios::binary);
	//找到该块所在文件里的位置（根据块号）
	ifs.seekg(block_num_ * 4 * 1024);
	//读取块信息
	ifs.read(data_, 4 * 1024);
	ifs.close();
}
//把data_信息写到指定路径的索引文件或记录文件中
void BlockInfo::WriteInfo(string path)
{
	path += file_->get_db_name() + "/" + file_->get_file_name();
	if (file_->get_type() == FORMAT_INDEX) path += ".index";
	else path += ".records";

	ofstream ofs(path, ios::binary);
	//找到该块所在文件里的位置
	ofs.seekp(block_num_ * 4 * 1024);
	ofs.write(data_, 4 * 1024);
	ofs.close();
}