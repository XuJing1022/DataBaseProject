//Implemented by Lai ZhengMin
#include "FileInfo.h"
#include "ConstValue.h"

FileInfo::FileInfo(void)
{
	db_name_ = "";
	type_ = FORMAT_RECORD;
	file_name_ = "";
	block_amount_in_file_ = 0;
	file_length_ = 0;
	first_block_ = 0;
	next_ = 0;
}
FileInfo::FileInfo(string db, int f_type, string f, int rec_amount, int rec_len, BlockInfo* first, FileInfo* nxt)
{
	db_name_ = db;
	type_ = f_type;
	file_name_ = f;
	block_amount_in_file_ = rec_amount;
	file_length_ = rec_len;
	first_block_ = first;
	next_ = nxt;
}
FileInfo::~FileInfo() {}

string FileInfo::get_db_name()
{
	return db_name_;
}
string FileInfo::get_file_name()
{
	return file_name_;
}
int FileInfo::get_type()
{
	return type_;
}

BlockInfo* FileInfo::GetFirstBlock()
{
	return first_block_;
}
void FileInfo::SetFirstBlock(BlockInfo* bp)
{
	first_block_ = bp;
}

FileInfo* FileInfo::GetNext()
{
	return next_;
}
void FileInfo::SetNext(FileInfo* fp)
{
	next_ = fp;
}

void FileInfo::IncreaseRecordAmount()
{
	block_amount_in_file_++;
}
void FileInfo::IncreaseRecordLength()
{
	file_length_ += 4096;
}