#include "StoragePage.h"

StoragePage::StoragePage()
{
    dirty = false;
    newPage = true;
    deleted = false;
    type = 0;
    prev = next = this;
}

void StoragePage::SetStorageFileIndex(uint32_t fileIndex_)
{
    fileIndex = fileIndex_;
}

uint32_t StoragePage::GetStorageFileIndex()
{
    return fileIndex;
}

void StoragePage::SetOffset(uint32_t offset_)
{
    offset = offset_;
}

uint32_t StoragePage::GetOffset()
{
    return offset;
}

void StoragePage::SetPageSize(uint32_t pageSize_)
{
    pageSize = pageSize_;
}

uint32_t StoragePage::GetPageSize()
{
    return pageSize;
}

void StoragePage::SetDirty(bool dirty_)
{
    dirty = dirty_;
}

bool StoragePage::IsDirty()
{
    return dirty;
}

void StoragePage::SetNew(bool newPage_)
{
    newPage = newPage_;
}

bool StoragePage::IsNew()
{
    return newPage;
}

void StoragePage::SetDeleted(bool deleted_)
{
    deleted = deleted_;
}

bool StoragePage::IsDeleted()
{
    return deleted;
}

void StoragePage::SetRewrite(bool rewrite_)
{
    rewrite = rewrite_;
}

bool StoragePage::NeedRewrite()
{
    return rewrite;
}

char StoragePage::GetType()
{
    return type;
}