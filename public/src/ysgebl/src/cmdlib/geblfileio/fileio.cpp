/* ////////////////////////////////////////////////////////////

File Name: fileio.cpp
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <ysport.h>

#include <ysshellext.h>
#include <ysshellextedit.h>
#include <ysshellextio.h>
#include "fileio.h"
#include "../geblcmdparam/cmdparam.h"

YSRESULT GeblCmd_FileIo_ReadFile(YsShellExtEdit &shl,const YsString fn)
{
	YsString ext;
	fn.GetExtension(ext);

	ext.Capitalize();
	if(0==strcmp(ext,".SRF"))
	{
		FILE *fp=fopen(fn,"r");
		if(NULL!=fp)
		{
			YsTextFileInputStream inStream(fp);
			shl.LoadSrf(inStream);
			fclose(fp);
			return YSOK;
		}
		else
		{
			return YSERR;
		}
	}
	else if(0==strcmp(ext,".STL"))
	{
		return shl.LoadStl(fn);
	}
	else
	{
		fprintf(stderr,"File type %s not supported.\n",(const char *)ext);
		return YSERR;
	}
}

YSRESULT GeblCmd_FileIo_SaveFile(const GeblCmd_CommandParameterInfo &cpi,const YsShellExtEdit &shl)
{
	YSRESULT res=YSOK;
	for(auto &outFile : cpi.OutputFileNameArray())
	{
		YsString ext;
		outFile.fn.GetExtension(ext);

		ext.Capitalize();
		if(0==strcmp(ext,".SRF"))
		{
			FILE *fp=fopen(outFile.fn,"w");
			if(NULL!=fp)
			{
				YsTextFileOutputStream outStream(fp);

				YsShellExtWriter writer;
				writer.SaveSrf(outStream,shl.Conv());
				shl.Saved();

				fclose(fp);
				printf("Saved %s\n",outFile.fn.Txt());
			}
			else
			{
				printf("Failed to save %s\n",outFile.fn.Txt());
				res=YSERR;
			}
		}
		else if(0==strcmp(ext,".STL"))
		{
			FILE *fp=fopen(outFile.fn,"wb");
			if(NULL!=fp)
			{
				int nIgnored;
				shl.SaveBinStl(nIgnored,fp,"STL generated by PolygonCrest cmd.");
				shl.Saved();
				fclose(fp);
				printf("Saved %s\n",outFile.fn.Txt());
			}
			else
			{
				printf("Failed to save %s\n",outFile.fn.Txt());
				res=YSERR;
			}
		}
		else if(0==strcmp(ext,".OBJ"))
		{
			FILE *fp=fopen(outFile.fn,"w");
			if(NULL!=fp)
			{
				YsTextFileOutputStream outStream(fp);

				YsShellExtObjWriter writer;
				YsShellExtObjWriter::WriteOption option;
				writer.WriteObj(outStream,shl.Conv(),option);
				shl.Saved();

				fclose(fp);
				printf("Saved %s\n",outFile.fn.Txt());
			}
			else
			{
				printf("Failed to save %s\n",outFile.fn.Txt());
				res=YSERR;
			}
		}
		else if(0==strcmp(ext,".OFF"))
		{
			FILE *fp=fopen(outFile.fn,"w");
			if(NULL!=fp)
			{
				YsTextFileOutputStream outStream(fp);

				YsShellExtOffWriter writer;
				writer.WriteOff(outStream,shl.Conv());
				shl.Saved();

				fclose(fp);
			}
			else
			{
				printf("Failed to save %s\n",outFile.fn.Txt());
				res=YSERR;
			}
		}
	}
	return res;
}

