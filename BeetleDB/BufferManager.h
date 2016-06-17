//Implemented by Lai ZhengMin
#pragma once
#ifndef _BUFFERMANAGER_H_
#define _BUFFERMANAGER_H_

#include <string>
#include "BlockHandle.h"
#include "FileHandle.h"

using namespace std;
//buffer������Ҫ����Block��File
class BufferManager
{
public:
	BufferManager(string path);
	~BufferManager();
	//�õ����ݿ���ļ��еı��Ϊblock_num�Ŀ�
	BlockInfo* GetFileBlock(string db_name, string tb_name, int file_type, int block_num);
	void WriteBlock(BlockInfo* block);
	void WriteToDisk();

private:
	BlockHandle* bhandle_;
	FileHandle* fhandle_;
	string path_;
	//���ؿ��ÿ���׵�ַ
	BlockInfo* GetUsableBlock();
};
#endif