#ifndef _WOBBLETEST_H
#define _WOBBLETEST_H

using namespace std;

#include "fixedtest.h"


class WobbleTest : public FixedTest
{
public:
	WobbleTest(void);
	~WobbleTest(void);
	WobbleTest::WobbleTest(TestStruct *testStruct);

	int test(int iPar, int iBaud);

private:
	int _iExitCode;
};

#endif