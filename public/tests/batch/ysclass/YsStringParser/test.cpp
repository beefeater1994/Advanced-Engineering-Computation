#include <stdio.h>

#include <ysclass.h>


int main(void)
{
	YsString str="word {group} { },, ( 12345)  \"This is a pen.\"  \\\"";

	const char *const parsed[]=
	{
		"word",
		"{",
		"group",
		"}",
		"{",
		"}",
		"",
		"",
		"(",
		"12345",
		")",
		"This is a pen.",
		"\"",
		nullptr
	};

	YsStringParser <YsString> parser;
	parser.Parse(str);

	int nFail=0;
	for(int i=0; i<parser.Argv().GetN(); ++i)
	{
		auto arg=parser.Argv()[i];
		printf("%s\n",arg.Txt());
		if(0!=arg.Strcmp(parsed[i]))
		{
			fprintf(stderr,"Error!\n");
			fprintf(stderr,"  Should be \"%s\"\n",parsed[i]); 
			++nFail;
		}
	}

	printf("%d failed\n",nFail);
	if(0==nFail)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


