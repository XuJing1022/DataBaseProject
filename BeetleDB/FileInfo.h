//Implemented by Lai ZhengMin
#pragma once
#ifndef _FILEINFO_H_
#define _FILEINFO_H_

#include "BlockInfo.h"
#include <string>

using namespace std;

class BlockInfo;
//表的file（分为index文件和record文件）
class FileInfo
{
public:
	FileInfo();
	//文件信息，包含数据库名、文件类型、文件名、文件包含的块的数目、文件长度、文件里块的头指针、下一个文件指针
	FileInfo(string db/*database*/, int f_type/*index or record*/, string f, int rec_amount/*包含块的数目*/, int rec_len/*文件长度*/, BlockInfo* first, FileInfo* nxt);
	~FileInfo(void);
	string get_db_name();
	string get_file_name();
	int get_type();

	BlockInfo* GetFirstBlock();
	void SetFirstBlock(BlockInfo* bp);
	//拿到下一个文件指针
	FileInfo* GetNext();
	void SetNext(FileInfo* fp);
	//增加块的数目
	void IncreaseRecordAmount();
	//增加文件总长度
	void IncreaseRecordLength();
private:
	//表所在的数据库名称
	string db_name_;
	//是index文件还是record文件		
	int type_;
	//文件名
	string file_name_;
	//该文件中的块的数目
	int block_amount_in_file_;
	//该文件总长（是块长的倍数）
	int file_length_;
	//文件内块的首地址指针
	BlockInfo *first_block_;
	//下一个文件指针
	FileInfo *next_;
};
#endif