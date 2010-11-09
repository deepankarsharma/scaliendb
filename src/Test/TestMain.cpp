#include "TestFunction.h"

TEST_START(TestMain);
//TEST_LOG_INIT(LOG_TARGET_STDOUT);
//TEST_ADD(TestArrayListBasic);
//TEST_ADD(TestArrayListRemove);
//TEST_ADD(TestClientAdd);
//TEST_ADD(TestClientBasic);
//TEST_ADD(TestClientBatchedGet);
//TEST_ADD(TestClientBatchedSet);
//TEST_ADD(TestClientBatchedSetRandom);     // TODO: this crashes shard server various ways!
//TEST_ADD(TestClientCreateTable);
//TEST_ADD(TestClientFailover);
//TEST_ADD(TestClientMaro);
//TEST_ADD(TestClientSchemaSet);
//TEST_ADD(TestClientSet);
//TEST_ADD(TestCommonHumanBytes);
//TEST_ADD(TestCommonRandomDistribution)
//TEST_ADD(TestCrashStorage);
//TEST_ADD(TestFileSystemDiskSpace);
//TEST_ADD(TestInTreeMap);
//TEST_ADD(TestInTreeMapInsert);
//TEST_ADD(TestInTreeMapInsertRandom);
//TEST_ADD(TestInTreeMapRemoveRandom);
//TEST_ADD(TestManualBasic);
//TEST_ADD(TestSnprintfTiming);
//TEST_ADD(TestStorageDeleteTestDatabase);
TEST_ADD(TestStorage);
//TEST_ADD(TestStorageBigRandomTransaction);
//TEST_ADD(TestStorageBigTransaction);
//TEST_ADD(TestStorageBinaryData);
//TEST_ADD(TestStorageCapacity);
//TEST_ADD(TestStorageFileSplit);
//TEST_ADD(TestStorageFileThreeWaySplit);
//TEST_ADD(TestStorageShardSize);         // TODO: this creates too much shards!
//TEST_ADD(TestStorageShardSplit);
//TEST_ADD(TestTimingBasicWrite);
//TEST_ADD(TestTimingSnprintf);
//TEST_ADD(TestTimingWrite);
//TEST_ADD(TestFormattingUnsigned);
TEST_EXECUTE();

#ifdef TEST
int main()
{
    return TestMain();
}
#endif