//Implemented by Lai ZhengMin
#pragma once
#ifndef _FILEINFO_H_
#define _FILEINFO_H_

#include "BlockInfo.h"
#include <string>

using namespace std;

class BlockInfo;
//���file����Ϊindex�ļ���record�ļ���
class FileInfo
{
public:
	FileInfo();
	//�ļ���Ϣ���������ݿ������ļ����͡��ļ������ļ������Ŀ����Ŀ���ļ����ȡ��ļ�����ͷָ�롢��һ���ļ�ָ��
	FileInfo(string db/*database*/, int f_type/*index or record*/, string f, int rec_amount/*���������Ŀ*/, int rec_len/*�ļ�����*/, BlockInfo* first, FileInfo* nxt);
	~FileInfo(void);
	string get_db_name();
	string get_file_name();
	int get_type();

	BlockInfo* GetFirstBlock();
	void SetFirstBlock(BlockInfo* bp);
	//�õ���һ���ļ�ָ��
	FileInfo* GetNext();
	void SetNext(FileInfo* fp);
	//���ӿ����Ŀ
	void IncreaseRecordAmount();
	//�����ļ��ܳ���
	void IncreaseRecordLength();
private:
	//�����ڵ����ݿ�����
	string db_name_;
	//��index�ļ�����record�ļ�		
	int type_;
	//�ļ���
	string file_name_;
	//���ļ��еĿ����Ŀ
	int block_amount_in_file_;
	//���ļ��ܳ����ǿ鳤�ı�����
	int file_length_;
	//�ļ��ڿ���׵�ַָ��
	BlockInfo *first_block_;
	//��һ���ļ�ָ��
	FileInfo *next_;
};
#endif