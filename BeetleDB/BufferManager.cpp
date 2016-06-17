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
//找到可用块的首地址，若空间已满，则运行LRU替换算法
BlockInfo* BufferManager::GetUsableBlock()
{
	if (bhandle_->get_block_count() > 0)
		return bhandle_->GetUsableBlock();
	else//如果无可用块，则执行LRU替换算法，返回一个最老的块
		return fhandle_->LRUAlgorithm();
}
//访问数据库表文件中的编号为block_num的块
BlockInfo* BufferManager::GetFileBlock(string db_name, string tb_name, int file_type, int block_num)
{
	//该文件链表里所有非空块的年龄都加1
	fhandle_->IncreaseAge();
	//拿到该表对应的文件链表头
	FileInfo *file = fhandle_->GetFileInfo(db_name, tb_name, file_type);
	//若文件存在
	if (file)
	{
		//拿到block_num对应的块
		BlockInfo *blo = fhandle_->GetBlockInfo(file, block_num);
		if (blo)//存在，直接返回
			return blo;
		else //如果该块不存在，则发生类似于操作系统的缺页中断，则要创建一个新的block（年龄为0），若发现无可用块，则要用LRU替换算法
		{
			BlockInfo *bp = GetUsableBlock();
			bp->set_block_num(block_num);
			bp->SetFile(file);
			bp->ReadInfo(path_);
			fhandle_->AddBlockInfo(bp);
			return bp;
		}
	}
	//若文件不存在，则创建新的块（若空间已满则执行LRU算法），同时创建对应这个块的文件
	else
	{
		BlockInfo *bp = GetUsableBlock();
		bp->set_block_num(block_num);
		//创建一个新的file
		FileInfo *fp = new FileInfo(db_name, file_type, tb_name, 0, 0, NULL, NULL);
		fhandle_->AddFileInfo(fp);
		bp->SetFile(fp);
		bp->ReadInfo(path_);
		fhandle_->AddBlockInfo(bp);
		return bp;
	}
	return 0;
}
//将block设为修改过（dirty）
void BufferManager::WriteBlock(BlockInfo* block)
{
	block->set_dirty(true);
}
//写回到磁盘	
void BufferManager::WriteToDisk()
{
	fhandle_->WriteToDisk();
}