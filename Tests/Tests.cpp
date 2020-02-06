#include "stdafx.h"

// General
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// https://docs.microsoft.com/ru-ru/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2019#Initialize_and_cleanup



TEST_CLASS(CChunkTests)
{
public:
		
	TEST_METHOD(ChunksWriteSmokeTest)
	{
		const uint8 cTestBytes[] = { 0, 1, 2, 3, 4 };

		std::shared_ptr<IByteBuffer> bb = std::make_shared<CByteBuffer>(cTestBytes, sizeof(cTestBytes));

		std::shared_ptr<IFile> f = std::make_shared<CFile>("D:\\TestFile.bin");

		CChunkedFile cf;
		cf.AddChunk("TEST", bb);
		cf.AddChunk("TEST", cTestBytes, sizeof(cTestBytes));
		cf.Save(f);

		Assert::AreEqual(f->getSize(), size_t(4 + 4 + (12 * 2) + (5 * 2)));
	}


	TEST_METHOD(ChunksWriteAndReadSmokeTest)
	{
		const uint8 cTestBytes[] = { 0, 1, 2, 3, 4 };

		std::shared_ptr<IByteBuffer> bb = std::make_shared<CByteBuffer>();
		bb->writeBytes(cTestBytes, sizeof(cTestBytes));

		std::shared_ptr<IFile> f = std::make_shared<CFile>("D:\\TestFile.bin");

		CChunkedFile cf;
		cf.AddChunk("TEST", bb);
		cf.AddChunk("SOME", bb);
		cf.Save(f);

		std::shared_ptr<IByteBuffer> bb2 = cf.GetChunk("TEST");
		Assert::AreEqual(bb2->getSize(), size_t(5));

		uint8 data[5];
		bb2->readBytes(data, 5);

		for (size_t i = 0; i < 5; i++)
			Assert::AreEqual(data[i], cTestBytes[i]);
	}
};
