#pragma once
#pragma region 
//API.hͷ�ļ�
//Implemented by Jin Xin
//��catalog Manager�������catalog manager ����
//��B Plus Tree�������BPlusTree ���н���
//��console���������̨����������̨�����Ϣ��
//��file System�������ļ�ϵͳ����
#pragma endregion

#ifndef  API_H_
#define API_H_

#include"SQLStatement.h"

#include"CatalogManager.h"
#include"BufferManager.h"
#include"IndexManager.h"

class API
{
public:
	API(string path);/*API���캯��*/
	~API(void);/*API��������*/
	void help();/*��ʾ��������*/
	void CreateDatabase(SQLCreateDatabase& sql_statement);/*�½����ݿ�*/
	void CreateTable(SQLCreateTable& sql_statement);/*�½����ݱ�*/
	void CreateIndex(SQLCreateIndex& sql_statement);/*�½�����*/
	void ShowDatabases();/*��ʾ���ݿ�*/
	void ShowTables();/*��ʾ���ݱ�*/
	void DropDatabase(SQLDropDatabase& sql_statement);/*ɾ�����ݿ�*/
	void DropTable(SQLDropTable& sql_statement);/*ɾ�����ݱ�*/
	void DropIndex(SQLDropIndex& sql_statement);/*ɾ������*/
	void Use(SQLUse& sql_statement);/*ѡ�����ݿ�*/
	void Insert(SQLInsert& sql_statement);/*��������*/
	void Select(SQLSelect& sql_statement);/*��ѯ����*/
	void JoinSelect(SQLJoinSelect& sql_statement);/*join��ѯ*/
	void Delete(SQLDelete& sql_statement);/*ɾ������*/
	void Update(SQLUpdate& sql_statement);/*��������*/
private:
	string path_;//�����ݿ�·��
	string current_database_;//��ǰѡ�����ݿ�
	CatalogManager* catalog_manager_;//Ŀ¼������
	BufferManager*	buffer_manager_;//���������
};
#endif // ! API_H_