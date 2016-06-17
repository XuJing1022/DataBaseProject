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
//LRU�㷨���ظñ��������������ļ������ϵĿ��ָ�루�������ʹ�õĿ飩
BlockInfo* FileHandle::LRUAlgorithm()
{
	FileInfo* fp = first_file_;
	//���ϵĿ��ǰһ��
	BlockInfo* oldestbefore = NULL;
	//���ϵĿ�
	BlockInfo* oldest = fp->GetFirstBlock();
	//�ҵ����ϵĿ�
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
	//������ϵĿ鱻�޸Ĺ��������������д���ļ�
	if (oldest->get_dirty())
		oldest->WriteInfo(path_);
	//������ϵĿ��ǰһλ���ǿտ飬��˵����һ��������ϵĿ顣��ʱ��ΪҪ�Ƴ�oldest�����и��ļ���һ��ͱ����oldest->next
	if (oldestbefore == NULL) oldest->GetFile()->SetFirstBlock(oldest->GetNext());
	else oldestbefore->SetNext(oldest->GetNext());
	//�����ϵĿ���������Ϊ0
	oldest->ResetAge();
	//oldest��next��Ϊnull
	oldest->SetNext(NULL);
	//�������ϵĿ��ָ��
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
			//����ÿ鱻�޸Ĺ�����д���ļ���
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