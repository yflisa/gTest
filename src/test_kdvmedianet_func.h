#ifndef __TEST_KDVMEDIANET_SEND_FUNC_H__
#define __TEST_KDVMEDIANET_SEND_FUNC_H__
#include "unittest.h"


class  CTestKdvmedianetFunc : public testing::Test
{
public:
	CTestKdvmedianetFunc() {}
	virtual ~CTestKdvmedianetFunc() {}
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
protected:
	virtual void SetUp();
	virtual void TearDown();
};

#endif