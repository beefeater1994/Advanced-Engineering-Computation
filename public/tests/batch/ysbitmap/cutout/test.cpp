#include <ysbitmap.h>
#include <ysport.h>

YSRESULT CutOutTest(const char fn[])
{
	YsBitmap bmp;
	if(YSOK!=bmp.LoadPng(fn))
	{
		fprintf(stderr,"Cannot open %s\n",fn);
		return YSERR;
	}

	YSRESULT res=YSOK;

	printf("Testing %s\n",fn);
	printf("Input Width=%d x %d\n",(int)bmp.GetWidth(),(int)bmp.GetHeight());

	YsString cutFn(fn);
	cutFn.RemoveExtension();

	int x0=-50;
	int y0=-50;
	int bigWid=bmp.GetWidth()+100;
	int bigHei=bmp.GetHeight()+100;
	int pieceWid=bigWid/3;
	int pieceHei=bigHei/3;
	for(int x=0; x<3; ++x)
	{
		for(int y=0; y<3; ++y)
		{
			printf("%d %d\n",x,y);

			int X0=x0+pieceWid*x;
			int Y0=y0+pieceHei*y;
			auto subBmp=bmp.CutOut(X0,Y0,pieceWid,pieceHei);

			if(subBmp.GetWidth()!=pieceWid || subBmp.GetHeight()!=pieceHei)
			{
				fprintf(stderr,"Cut-out bitmap size is incorrect.\n");
				res=YSERR;
			}

			for(int subX=0; subX<subBmp.GetWidth(); ++subX)
			{
				for(int subY=0; subY<subBmp.GetHeight(); ++subY)
				{
					auto subBmpPix=subBmp.GetPixelPointer(subX,subY);

					int srcX=X0+subX,srcY=Y0+subY;
					if(YSTRUE==bmp.IsInRange(srcX,srcY))
					{
						auto srcBmpPix=bmp.GetPixelPointer(srcX,srcY);
						if(subBmpPix[0]!=srcBmpPix[0] || 
						   subBmpPix[1]!=srcBmpPix[1] || 
						   subBmpPix[2]!=srcBmpPix[2] || 
						   subBmpPix[3]!=srcBmpPix[3])
						{
							fprintf(stderr,"Pixel in the cut-out bitmap at (%d,%d) and source bitmap at (%d,%d) do not match.\n",
							    subX,subY,srcX,srcY);
							fprintf(stderr,"  Cut-out (%d,%d,%d,%d)\n",subBmpPix[0],subBmpPix[1],subBmpPix[2],subBmpPix[3]);
							fprintf(stderr,"  Source  (%d,%d,%d,%d)\n",srcBmpPix[0],srcBmpPix[1],srcBmpPix[2],srcBmpPix[3]);
							res=YSERR;
							subY=subBmp.GetHeight();
							subX=subBmp.GetWidth();
							break;
						}
					}
					else
					{
						if(subBmpPix[0]!=0 || subBmpPix[1]!=0 || subBmpPix[2]!=0 || subBmpPix[3]!=0)
						{
							fprintf(stderr,"Pixel in the cut-out bitmap at (%d,%d) must be (0,0,0,0)\n",subX,subY);
							fprintf(stderr,"It actually is (%d,%d,%d,%d)\n",subBmpPix[0],subBmpPix[1],subBmpPix[2],subBmpPix[3]);
							res=YSERR;
							subY=subBmp.GetHeight();
							subX=subBmp.GetWidth();
							break;
						}
					}
				}
			}

			// YsString appendix;
			// appendix.Printf("_%d_%d",x,y);
			// auto ful=cutFn;
			// ful.Append(appendix);
			// ful.ReplaceExtension(".png");
			// YsFileIO::File fp(ful,"wb");
			// subBmp.SavePng(fp);
		}
	}

	return res;
}

int main(void)
{
	YsFileIO::ChDir(YsSpecialPath::GetProgramBaseDirW());

	int nFail=0;
	if(YSOK!=CutOutTest("foliage.png"))
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
