#include "stdafx.h"
#include "CppUnitTest.h"
#include "../GGP-Wizards/Object.h"

// .cpp included for static variable definition (nextInstanceId)
//TODO can this be done better to remove .cpp include?
#include "../GGP-Wizards/Object.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestObjectInstanceIdIncrement)
		{
			Object* obj1 = new Object();
			Object* obj2 = new Object();
			
			Assert::AreNotEqual(obj1->GetInstanceId(), obj2->GetInstanceId());
			
			delete obj1;
			delete obj2;
		}

		TEST_METHOD(TestObjectInstanceIdStartsAt0)
		{
			Object* obj1 = new Object();

			Assert::AreNotEqual(obj1->GetInstanceId(), 0);
			
			delete obj1;
		}

		TEST_METHOD(TestObjectOperatorOverload)
		{
			Object* obj1 = new Object();
			Object* obj2 = new Object();

			Assert::AreEqual(obj1 == obj2, false);
			Assert::AreEqual(obj1 != obj2, true);

			delete obj1;
			delete obj2;

		}
	};
}