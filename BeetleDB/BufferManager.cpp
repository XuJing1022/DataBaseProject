//Implemented by Lai ZhengMin
#include "BufferManager.h"
#include "ConstValue.h"

#include <string>
#include <fstream>

BufferManager::BufferManager(string path) :path_(path)
{
	bhandle_ = new BlockHandle(path);
	fhandle_ = new FileHandle(path);
}

BufferManager::~BufferManager()
{
	delete bhandle_;
	delete fhandle_;
}
//�ҵ����ÿ���׵�ַ�����ռ�������������LRU�滻�㷨
BlockInfo* BufferManager::GetUsableBlock()
{
	if (bhandle_->get_block_count() > 0)
		return bhandle_->GetUsableBlock();
	else//����޿��ÿ飬��ִ��LRU�滻�㷨������һ�����ϵĿ�
		return fhandle_->LRUAlgorithm();
}
//�������ݿ���ļ��еı��Ϊblock_num�Ŀ�
BlockInfo* BufferManager::GetFileBlock(string db_name, string tb_name, int file_type, int block_num)
{
	//���ļ����������зǿտ�����䶼��1
	fhandle_->IncreaseAge();
	//�õ��ñ��Ӧ���ļ�����ͷ
	FileInfo *file = fhandle_->GetFileInfo(db_name, tb_name, file_type);
	//���ļ�����
	if (file)
	{
		//�õ�block_num��Ӧ�Ŀ�
		BlockInfo *blo = fhandle_->GetBlockInfo(file, block_num);
		if (blo)//���ڣ�ֱ�ӷ���
			return blo;
		else //����ÿ鲻���ڣ����������ڲ���ϵͳ��ȱҳ�жϣ���Ҫ����һ���µ�block������Ϊ0�����������޿��ÿ飬��Ҫ��LRU�滻�㷨
		{
			BlockInfo *bp = GetUsableBlock();
			bp->set_block_num(block_num);
			bp->SetFile(file);
			bp->ReadInfo(path_);
			fhandle_->AddBlockInfo(bp);
			return bp;
		}
	}
	//���ļ������ڣ��򴴽��µĿ飨���ռ�������ִ��LRU�㷨����ͬʱ������Ӧ�������ļ�
	else
	{
		BlockInfo *bp = GetUsableBlock();
		bp->set_block_num(block_num);
		//����һ���µ�file
		FileInfo *fp = new FileInfo(db_name, file_type, tb_name, 0, 0, NULL, NULL);
		fhandle_->AddFileInfo(fp);
		bp->SetFile(fp);
		bp->ReadInfo(path_);
		fhandle_->AddBlockInfo(bp);
		return bp;
	}
	return 0;
}
//��block��Ϊ�޸Ĺ���dirty��
void BufferManager::WriteBlock(BlockInfo* block)
{
	block->set_dirty(true);
}
//д�ص�����	
void BufferManager::WriteToDisk()
{
	fhandle_->WriteToDisk();
}