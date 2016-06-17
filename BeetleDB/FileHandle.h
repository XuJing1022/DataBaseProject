//Implemented by Lai ZhengMin
#pragma once
#ifndef _FILEHANDLE_H_
#define _FILEHANDLE_H_

#include <string>
#include "FileInfo.h"
#include "BlockInfo.h"

using namespace std;
//管理某表对应的文件
class FileHandle
{
public:
	//创建该目录下文件信息头指针
	FileHandle(string p);
	//释放文件下所有块的空间
	~FileHandle();
	//根据数据库名、表名、文件类型拿到该文件
	FileInfo* GetFileInfo(string db_name, string tb_name, int file_type);
	//根据文件和块所在文件中的位置拿到该块
	BlockInfo* GetBlockInfo(FileInfo* file, int block_pos);
	//用LRU算法返回一个最老的块
	BlockInfo* LRUAlgorithm();
	//往文件链表尾部加一个block
	void AddBlockInfo(BlockInfo* block);
	//根据block找到对应的文件，并将此block插到该文件的块的尾部
	void AddFileInfo(FileInfo* file);
	//将所有文件中的块年龄加1
	void IncreaseAge();
	//将文件链表中的文件信息写回磁盘
	void WriteToDisk();
private:
	//文件链表首指针
	FileInfo* first_file_;
	//该表所在的路径（即文件链表所在路径）
	string path_;
};
#endif