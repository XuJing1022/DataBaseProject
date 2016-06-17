//Implemented by Lai ZhengMin
#include "BlockHandle.h"

//管理某个表的所有块
BlockHandle::BlockHandle(string path)
{
	first_block_ = new BlockInfo(0);
	block_size_ = 300; //一个表最大可装300块，即1200KB
	block_count_ = 0; //已创建的可用的块数
	path_ = path;
	Add(first_block_);
}

BlockHandle::~BlockHandle()
{
	BlockInfo* b = first_block_;
	//释放所有块
	while (block_count_ > 0)
	{
		BlockInfo* bn = b->GetNext();
		delete b;
		b = bn;
		block_count_--;
	}
}
//拿到可用的块的数目
int BlockHandle::get_block_count() { return block_count_; }

/* 返回first_block ->GetNext(); 第一块是空块指针*/
BlockInfo* BlockHandle::GetUsableBlock()
{
	if (block_count_ == 0) return NULL;

	BlockInfo* p = first_block_->GetNext();
	first_block_->SetNext(first_block_->GetNext()->GetNext());
	block_count_--;

	p->ResetAge();
	p->SetNext(NULL);
	return p;
}

//在first_block后插入一个block
void BlockHandle::AddANewBlockBehindFirstBlock(BlockInfo* block)
{
	if (block_count_ == 0)
	{
		first_block_ = block;
		block->SetNext(block);
	}
	else
	{
		block->SetNext(first_block_->GetNext());
		first_block_->SetNext(block);
	}
	block_count_++;
}
//递归地开辟300块
BlockInfo* BlockHandle::Add(BlockInfo* block)
{
	//开辟新块，编号都为0
	BlockInfo* adder = new BlockInfo(0);
	adder->SetNext(block->GetNext());
	block->SetNext(adder);
	block_count_++;
	if (block_count_ == block_size_) return adder;
	else return Add(adder);
}