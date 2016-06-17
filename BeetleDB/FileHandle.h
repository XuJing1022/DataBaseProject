//Implemented by Lai ZhengMin
#pragma once
#ifndef _FILEHANDLE_H_
#define _FILEHANDLE_H_

#include <string>
#include "FileInfo.h"
#include "BlockInfo.h"

using namespace std;
//����ĳ���Ӧ���ļ�
class FileHandle
{
public:
	//������Ŀ¼���ļ���Ϣͷָ��
	FileHandle(string p);
	//�ͷ��ļ������п�Ŀռ�
	~FileHandle();
	//�������ݿ������������ļ������õ����ļ�
	FileInfo* GetFileInfo(string db_name, string tb_name, int file_type);
	//�����ļ��Ϳ������ļ��е�λ���õ��ÿ�
	BlockInfo* GetBlockInfo(FileInfo* file, int block_pos);
	//��LRU�㷨����һ�����ϵĿ�
	BlockInfo* LRUAlgorithm();
	//���ļ�����β����һ��block
	void AddBlockInfo(BlockInfo* block);
	//����block�ҵ���Ӧ���ļ���������block�嵽���ļ��Ŀ��β��
	void AddFileInfo(FileInfo* file);
	//�������ļ��еĿ������1
	void IncreaseAge();
	//���ļ������е��ļ���Ϣд�ش���
	void WriteToDisk();
private:
	//�ļ�������ָ��
	FileInfo* first_file_;
	//�ñ����ڵ�·�������ļ���������·����
	string path_;
};
#endif