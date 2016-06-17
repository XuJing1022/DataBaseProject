//Implemented by Lai ZhengMin
#pragma once
#ifndef _BUFFERMANAGER_H_
#define _BUFFERMANAGER_H_

#include <string>
#include "BlockHandle.h"
#include "FileHandle.h"

using namespace std;
//buffer管理。主要管理Block和File
class BufferManager
{
public:
	BufferManager(string path);
	~BufferManager();
	//拿到数据库表文件中的编号为block_num的块
	BlockInfo* GetFileBlock(string db_name, string tb_name, int file_type, int block_num);
	void WriteBlock(BlockInfo* block);
	void WriteToDisk();

private:
	BlockHandle* bhandle_;
	FileHandle* fhandle_;
	string path_;
	//返回可用块的首地址
	BlockInfo* GetUsableBlock();
};
#endif