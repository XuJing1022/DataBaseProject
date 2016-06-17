//Implemented by Lai ZhengMin
#pragma once
#ifndef _BLOCKINFO_H_
#define _BLOCKINFO_H_

#include "FileInfo.h"

#include <string>
#include <sys/types.h>

using namespace std;

class FileInfo;

//����Ϣ
class BlockInfo
{
public:
	BlockInfo(int num);
	~BlockInfo();

	FileInfo* GetFile();
	void SetFile(FileInfo *f);

	int get_block_num();
	void set_block_num(int num);

	char* get_data();

	bool get_dirty();
	void set_dirty(bool dt);

	long get_age();

	BlockInfo* GetNext();
	void SetNext(BlockInfo* block);

	void IncreaseAge();
	//��������Ϊ0
	void ResetAge();
	//int *����4���ֽڣ�headerǰ4����0-3���ֽڴ������һ����ı��
	int GetPrevBlockNum();

	void SetPrevBlockNum(int num);
	//header ��4-7 �ֽڴ������һ����ı��
	int GetNextBlockNum();
	void SetNextBlockNum(int num);
	//header ��8-11���ֽڴ���Ǹÿ��������ļ�¼����Ŀ
	int GetRecordCount();
	void SetRecordCount(int count);
	void DecreaseRecordCount();
	//�õ����д洢��¼���׵�ַ
	char* GetContentAdress();
	//��ָ��·���а�������Ϣ�����Ϣ�������ļ�����data_��
	void ReadInfo(string path);
	//��data_��Ϣд��ָ��·���������ļ����¼�ļ���
	void WriteInfo(string path);

private:
	//�ÿ�����Ӧ���ļ���Ϣ
	FileInfo *file_;
	//��ı��
	int block_num_;
	//�洢����Ϣ
	char *data_;
	//�Ƿ�Ϊ��飨���޸Ĺ���
	bool dirty_;
	//���䣨����Խ���Խ�ϣ�
	long age_;
	//��һ��
	BlockInfo *next_;
};
#endif