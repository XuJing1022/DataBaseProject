//Implemented by Lai ZhengMin
#pragma once
#ifndef _BLOCKINFO_H_
#define _BLOCKINFO_H_

#include "FileInfo.h"

#include <string>
#include <sys/types.h>

using namespace std;

class FileInfo;

//块信息
class BlockInfo
{
public:
	BlockInfo(int num);
	~BlockInfo();

	FileInfo* GetFile();
	void SetFile(FileInfo *f);

	int get_block_num();
	void set_block_num(int num);

	char* get_data();

	bool get_dirty();
	void set_dirty(bool dt);

	long get_age();

	BlockInfo* GetNext();
	void SetNext(BlockInfo* block);

	void IncreaseAge();
	//重置年龄为0
	void ResetAge();
	//int *，读4个字节，header前4个（0-3）字节存的是上一个块的编号
	int GetPrevBlockNum();

	void SetPrevBlockNum(int num);
	//header 第4-7 字节存的是下一个块的编号
	int GetNextBlockNum();
	void SetNextBlockNum(int num);
	//header 第8-11个字节存的是该块所包含的记录的数目
	int GetRecordCount();
	void SetRecordCount(int count);
	void DecreaseRecordCount();
	//拿到块中存储记录的首地址
	char* GetContentAdress();
	//从指定路径中把索引信息或块信息二进制文件读到data_中
	void ReadInfo(string path);
	//把data_信息写到指定路径的索引文件或记录文件中
	void WriteInfo(string path);

private:
	//该块所对应的文件信息
	FileInfo *file_;
	//块的编号
	int block_num_;
	//存储块信息
	char *data_;
	//是否为脏块（被修改过）
	bool dirty_;
	//年龄（年龄越大块越老）
	long age_;
	//下一块
	BlockInfo *next_;
};
#endif