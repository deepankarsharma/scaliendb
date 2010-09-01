#ifndef STORAGEFILE_H
#define STORAGEFILE_H

#include "StorageIndexPage.h"
#include "StorageDataPage.h"

//#define DEFAULT_KEY_LIMIT			1000
//#define DEFAULT_DATAPAGE_SIZE		64*1024
//#define DEFAULT_NUM_DATAPAGES		256			// 16.7 MB wort of data pages
//#define DEFAULT_INDEXPAGE_SIZE		256*1024	// to fit 256 keys

#define DEFAULT_KEY_LIMIT			100
#define DEFAULT_DATAPAGE_SIZE		4*1024
#define DEFAULT_NUM_DATAPAGES		16*256			// 16.7 MB wort of data pages
#define DEFAULT_INDEXPAGE_SIZE		16+16*256*(100+8)	// to fit 256 keys

#define INDEXPAGE_OFFSET			12

// default total filesize: 4+4+4+262144+256*65536 ~= 17M

/*
===============================================================================

 StorageFile

===============================================================================
*/

class StorageFile
{
public:
	StorageFile();
	~StorageFile();
	
	void					Open(char* filepath);
	void					Close();
	
	void					SetFileIndex(uint32_t fileIndex);

//	bool					IsNew();
	
	bool					Get(ReadBuffer& key, ReadBuffer& value);
	bool					Set(ReadBuffer& key, ReadBuffer& value, bool copy = true);
	void					Delete(ReadBuffer& key);

	ReadBuffer				FirstKey();
	bool					IsEmpty();

	bool					IsOverflowing();
	StorageFile*			SplitFile();

	void					Read();
	void					ReadRest();
	void					WriteRecovery(int recoveryFD);
	void					WriteData();

	StorageDataPage*		CursorBegin(ReadBuffer& key, Buffer& nextKey);

private:
	int32_t					Locate(ReadBuffer& key);
	void					LoadDataPage(uint32_t index);
	void					MarkPageDirty(StoragePage* page);
	void					SplitDataPage(uint32_t index);
	void					ReorderPages();
	void					ReorderFile();
	void					CopyDataPage(uint32_t index);
	
	int						fd;
	uint32_t				fileIndex;
	uint32_t				indexPageSize;
	uint32_t				dataPageSize;
	uint32_t				numDataPageSlots;
	bool					isOverflowing;
	bool					newFile;
	Buffer					filepath;
	StorageIndexPage		indexPage;
	StorageDataPage**		dataPages;
	uint32_t				numDataPages;
	InTreeMap<StoragePage>	dirtyPages;
};

#endif