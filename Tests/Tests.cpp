#include "stdafx.h"

// General
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// https://docs.microsoft.com/ru-ru/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2019#Initialize_and_cleanup



TEST_CLASS(Tests)
{
public:
		
	TEST_METHOD(TestMethod1)
	{
		Assert::AreEqual(0, 1);
	}
};
