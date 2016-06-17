//Implemented by Lai ZhengMin
#include "FileHandle.h"

//implement LRU Algorithm
FileHandle::FileHandle(string p)
{
	first_file_ = new FileInfo();
	path_ = p;
}

FileHandle::~FileHandle()
{
	WriteToDisk();
	FileInfo* fp = first_file_;
	while (fp != NULL)
	{
		FileInfo* fpn = fp->GetNext();
		BlockInfo* bp = fp->GetFirstBlock();
		while (bp != NULL)
		{
			BlockInfo* bpn = bp->GetNext();
			delete bp;
			bp = bpn;
		}
		delete fp;
		fp = fpn;
	}
}

FileInfo* FileHandle::GetFileInfo(string db_name, string tb_name, int file_type)
{
	FileInfo* fp = first_file_;
	while (fp != NULL)
	{
		if (fp->get_db_name() == db_name && fp->get_type() == file_type && fp->get_file_name() == tb_name)
			return fp;
		fp = fp->GetNext();
	}
	return NULL;
}

BlockInfo* FileHandle::GetBlockInfo(FileInfo* file, int block_pos)
{
	BlockInfo* bp = file->GetFirstBlock();
	while (bp != NULL)
	{
		if (bp->get_block_num() == block_pos)
			return bp;
		bp = bp->GetNext();
	}
	return NULL;
}
//LRU算法返回该表所包含的所有文件中最老的块的指针（最近最少使用的块）
BlockInfo* FileHandle::LRUAlgorithm()
{
	FileInfo* fp = first_file_;
	//最老的块的前一块
	BlockInfo* oldestbefore = NULL;
	//最老的块
	BlockInfo* oldest = fp->GetFirstBlock();
	//找到最老的块
	while (fp != NULL)
	{
		BlockInfo* bpbefore = NULL;
		BlockInfo* bp = fp->GetFirstBlock();
		while (bp != NULL)
		{
			if (bp->get_age() > oldest->get_age())
			{
				oldestbefore = bpbefore;
				oldest = bp;
			}
			bpbefore = bp;
			bp = bp->GetNext();
		}
		fp = fp->GetNext();
	}
	//如果最老的块被修改过，则把它的内容写回文件
	if (oldest->get_dirty())
		oldest->WriteInfo(path_);
	//如果最老的块的前一位置是空块，则说明第一块就是最老的块。这时因为要移除oldest，所有该文件第一块就变成了oldest->next
	if (oldestbefore == NULL) oldest->GetFile()->SetFirstBlock(oldest->GetNext());
	else oldestbefore->SetNext(oldest->GetNext());
	//将最老的块年龄重置为0
	oldest->ResetAge();
	//oldest的next置为null
	oldest->SetNext(NULL);
	//返回最老的块的指针
	return oldest;
}

void FileHandle::AddFileInfo(FileInfo* file)
{
	FileInfo *fp = first_file_;
	if (fp == NULL) fp = file;
	else
	{
		while (fp->GetNext() != NULL)
			fp = fp->GetNext();
		fp->SetNext(file);
	}
}

void FileHandle::AddBlockInfo(BlockInfo* block)
{
	BlockInfo *bp = block->GetFile()->GetFirstBlock();
	if (bp == NULL) block->GetFile()->SetFirstBlock(block);
	else
	{
		while (bp->GetNext() != NULL)
			bp = bp->GetNext();
		bp->SetNext(block);
	}
	block->GetFile()->IncreaseRecordAmount();
	block->GetFile()->IncreaseRecordLength();
}

void FileHandle::IncreaseAge()
{
	FileInfo* fp = first_file_;
	while (fp != NULL)
	{
		BlockInfo* bp = fp->GetFirstBlock();
		while (bp != NULL)
		{
			bp->IncreaseAge();
			bp = bp->GetNext();
		}
		fp = fp->GetNext();
	}
}

void FileHandle::WriteToDisk()
{
	FileInfo* fp = first_file_;
	while (fp != NULL)
	{
		BlockInfo* bp = fp->GetFirstBlock();
		while (bp != NULL)
		{
			//如果该块被修改过，则写到文件中
			if (bp->get_dirty())
			{
				bp->WriteInfo(path_);
				bp->set_dirty(false);
			}
			bp = bp->GetNext();
		}
		fp = fp->GetNext();
	}
}