//Implemented by Lai ZhengMin

#pragma once

#ifndef _BLOCKHANDLE_H_
#define _BLOCKHANDLE_H

#include "BlockInfo.h"
//�������ĳ�ļ������п�
class BlockHandle
{
public:
	BlockHandle(string path);
	~BlockHandle();

	int get_block_count();
	/* ����first_block ->GetNext(); ��һ����ͷָ�룬���洢��Ϣ*/
	BlockInfo* GetUsableBlock();
	void AddANewBlockBehindFirstBlock(BlockInfo* block);

private:
	BlockInfo* first_block_;
	int block_size_;     //�ܿ���
	int block_count_;    //���õĿ���
	string path_;
	BlockInfo* Add(BlockInfo* block);
};
#endif