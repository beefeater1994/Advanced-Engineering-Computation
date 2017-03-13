#include <ysclass.h>

YSRESULT SomethingTest(void)
{
	if(true)
	{
		return YSOK;
	}
	else
	{
		fprintf(stderr,"Failed!\n");
		return YSERR;
	}
}

int main(void)
{
	int nFail=0;
	if(YSOK!=SomethingTest())
	{
		++nFail;
	}

	printf("%d failed.\n",nFail);
	if(0<nFail)
	{
		return 1;
	}
	return 0;
}
