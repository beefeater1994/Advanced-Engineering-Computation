#include <simplebitmap.h>

int main(int argc,char *argv[])
{
	SimpleBitmap bmp;
	if(2<=argc && true==bmp.LoadPng(argv[1]))
	{
		FILE *fp=fopen("./verify.png","wb");
		if(nullptr!=fp)
		{
			bmp.SavePng(fp);
			fclose(fp);
		}
	}
	else
	{
		printf("Usage: testbitmap <png-file>\n");
	}
	return 0;
}
