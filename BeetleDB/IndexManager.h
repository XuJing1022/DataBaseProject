//
// IndexManager.h
//
// Created by Xujing on 2016/6/15.
//
#pragma once
#ifndef _INDEXMANAGER_H_
#define _INDEXMANAGER_H_

#include <string>

#include "SQLStatement.h"
#include "CatalogManager.h"
#include "BufferManager.h"
#include "BPlusTree.h"

using namespace std;

class IndexManager
{
public:
	IndexManager(CatalogManager *cm, BufferManager *bm, string dbname);
	~IndexManager(void);
	void CreateIndex(SQLCreateIndex& st);
private:
	CatalogManager* catalog_m_;
	BufferManager* buffer_m_;
	string db_name_;
};

#endif
