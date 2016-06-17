//Implemented by Lai ZhengMin
#include "BlockHandle.h"

//����ĳ��������п�
BlockHandle::BlockHandle(string path)
{
	first_block_ = new BlockInfo(0);
	block_size_ = 300; //һ��������װ300�飬��1200KB
	block_count_ = 0; //�Ѵ����Ŀ��õĿ���
	path_ = path;
	Add(first_block_);
}

BlockHandle::~BlockHandle()
{
	BlockInfo* b = first_block_;
	//�ͷ����п�
	while (block_count_ > 0)
	{
		BlockInfo* bn = b->GetNext();
		delete b;
		b = bn;
		block_count_--;
	}
}
//�õ����õĿ����Ŀ
int BlockHandle::get_block_count() { return block_count_; }

/* ����first_block ->GetNext(); ��һ���ǿտ�ָ��*/
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

//��first_block�����һ��block
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
//�ݹ�ؿ���300��
BlockInfo* BlockHandle::Add(BlockInfo* block)
{
	//�����¿飬��Ŷ�Ϊ0
	BlockInfo* adder = new BlockInfo(0);
	adder->SetNext(block->GetNext());
	block->SetNext(adder);
	block_count_++;
	if (block_count_ == block_size_) return adder;
	else return Add(adder);
}