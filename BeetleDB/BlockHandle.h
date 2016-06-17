//Implemented by Lai ZhengMin

#pragma once

#ifndef _BLOCKHANDLE_H_
#define _BLOCKHANDLE_H

#include "BlockInfo.h"
//负责管理某文件下所有块
class BlockHandle
{
public:
	BlockHandle(string path);
	~BlockHandle();

	int get_block_count();
	/* 返回first_block ->GetNext(); 第一个是头指针，不存储信息*/
	BlockInfo* GetUsableBlock();
	void AddANewBlockBehindFirstBlock(BlockInfo* block);

private:
	BlockInfo* first_block_;
	int block_size_;     //总块数
	int block_count_;    //可用的块数
	string path_;
	BlockInfo* Add(BlockInfo* block);
};
#endif