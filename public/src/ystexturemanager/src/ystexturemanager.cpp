#include "ystexturemanager.h"
#include <yspng.h>
#include <stdlib.h>


/* static */ YsTextureManager::DATA_FORMAT YsTextureManager::GetFormatFromString(const char str[])
{
	YsString s(str);
	if('.'==s[0])
	{
		s.Delete(0);
	}
	if(0==s.STRCMP("png"))
	{
		return FOM_PNG;
	}
	if(0==s.STRCMP("rawrgba"))
	{
		return FOM_RAW_RGBA;
	}
	if(0==s.STRCMP("rawz"))
	{
		return FOM_RAW_Z;
	}
	return FOM_UNKNOWN;
}
/* static */ const char *YsTextureManager::GetStringFromFormat(DATA_FORMAT fom)
{
	switch(fom)
	{
	default:
		break;
	case FOM_RAW_RGBA:
		return "rawrgba";
	case FOM_PNG:
		return ".png";
	case FOM_RAW_Z:
		return "rawz";
	}
	return "unknown";
}


YsTextureManager::Unit::Unit()
{
	owner=nullptr;
	texPtr=nullptr;
	CleanUp();
}

YsTextureManager::Unit::~Unit()
{
	CleanUp();
}

void YsTextureManager::Unit::CleanUp(void)
{
	CleanTextureCache();
	label.Set("");
	fType=FOM_UNKNOWN;
	wid=0;
	hei=0;
	upScaleWid=0;
	upScaleHei=0;
	texCoordScale.Set(1,1);
	dat.CleanUp();
	fn.Set(L"");
	filterType=FILTERTYPE_NEAREST;
	flags=0;
	randomNoiseLevel=0.05;
}

void YsTextureManager::Unit::CleanTextureCache(void) const
{
	if(NULL!=texPtr)
	{
		YsTextureManager::Delete(texPtr);
		texPtr=NULL;
	}
}

YSRESULT YsTextureManager::Unit::SetData(DATA_FORMAT fType,int wid,int hei,YSSIZE_T length,const unsigned char fileData[])
{
	CleanTextureCache();
	if(FOM_PNG==fType || FOM_RAW_RGBA==fType)
	{
		this->fType=fType;
		this->wid=wid;
		this->hei=hei;
		dat.Set(length,fileData);
		return YSOK;
	}
	return YSERR;
}

YsTextureManager::Unit &YsTextureManager::Unit::CopyFrom(Unit &incoming)
{
	CleanUp();

	flags=incoming.flags;
	label=incoming.label;
	filterType=incoming.filterType;
	fType=incoming.fType;
	randomNoiseLevel=incoming.randomNoiseLevel;
	fn=incoming.fn;
	wid=incoming.wid;
	hei=incoming.hei;
	upScaleWid=incoming.upScaleWid;
	upScaleHei=incoming.upScaleHei;
	texCoordScale=incoming.texCoordScale;
	dat=incoming.dat;

	return *this;
}

YSRESULT YsTextureManager::Unit::SetData(DATA_FORMAT fType,int w,int h,YsConstArrayMask <unsigned char> dat)
{
	return SetData(fType,w,h,dat.GetN(),dat);
}

YSRESULT YsTextureManager::Unit::MakeShadowMap(int w,int h)
{
	CleanTextureCache();
	this->fType=FOM_RAW_Z;
	this->wid=w;
	this->hei=h;
	this->flags|=FLAG_RENDERTARGET;
	return YSOK;
}

YsVec2 YsTextureManager::Unit::ScaleTexCoord(double u,double v) const
{
	return YsVec2(u*texCoordScale.x(),v*texCoordScale.y());
}

YsVec2 YsTextureManager::Unit::ScaleTexCoord(YsVec2 uv) const
{
	return ScaleTexCoord(uv.x(),uv.y());
}

YSRESULT YsTextureManager::Unit::SetLabel(const YsString &str)
{
	label=str;
	return YSOK;
}

YSRESULT YsTextureManager::Unit::SetFileName(const YsWString &fn)
{
	this->fn=fn;
	return YSOK;
}

YSBOOL YsTextureManager::Unit::IsActualTextureReady(void) const
{
	if(NULL!=texPtr)
	{
		return YsTextureManager::IsTextureReady(texPtr);
	}
	return YSFALSE;
}

int YsTextureManager::Unit::GetWidth(void) const
{
	return wid;
}
int YsTextureManager::Unit::GetHeight(void) const
{
	return hei;
}
YsVec2i YsTextureManager::Unit::GetSize(void) const
{
	return YsVec2i(GetWidth(),GetHeight());
}

void YsTextureManager::Unit::SetDontSave(YSBOOL dontSave)
{
	if(YSTRUE==dontSave)
	{
		flags|=FLAG_DONTSAVE;
	}
	else
	{
		flags&=(~FLAG_DONTSAVE);
	}
}

void YsTextureManager::Unit::ApplyRandomNoise(int wid,int hei,unsigned char rgba[]) const
{
	for(int i=0; i<wid*hei; ++i)
	{
		double m=(1.0-randomNoiseLevel)+randomNoiseLevel*(double)rand()/(double)RAND_MAX;
		double r,g,b;
		r=m*(double)rgba[i*4];
		g=m*(double)rgba[i*4+1];
		b=m*(double)rgba[i*4+2];

		rgba[i*4  ]=(unsigned char)r;
		rgba[i*4+1]=(unsigned char)g;
		rgba[i*4+2]=(unsigned char)b;
	}
}

YSRESULT YsTextureManager::Unit::MakeActualTexture(void) const
{
	return MakeActualTexture(owner->GetTextureGenerationOption());
}

YSRESULT YsTextureManager::Unit::MakeActualTexture(TextureGenerationOption opt) const
{
	CleanTextureCache();
	texPtr=YsTextureManager::Alloc();

	if(0==(flags&FLAG_RENDERTARGET))
	{
		switch(fType)
		{
		case FOM_PNG:
			{
				YsPngBinaryMemoryStream inStream(dat.GetN(),dat);
				YsRawPngDecoder png;
				if(YSOK==(YSRESULT)png.Decode(inStream))
				{
					this->wid=png.wid;
					this->hei=png.hei;
					ApplyRandomNoise(png.wid,png.hei,png.rgba);
					return SetUpRGBABitmap(texPtr,png.wid,png.hei,png.rgba,opt);
				}
			}
			break;
		case FOM_RAW_RGBA:
			return SetUpRGBABitmap(texPtr,wid,hei,dat,opt);
		default:
			break;
		}
	}
	else
	{
		switch(fType)
		{
		case FOM_RAW_Z:
			return YsTextureManager::CreateDepthMapRenderTarget(texPtr,wid,hei);
		default:
			break;
		}
	}
	return YSERR;
}

YSRESULT YsTextureManager::Unit::SetUpRGBABitmap(
	ActualTexture *texPtr,int wid,int hei,const unsigned char rgba[],const TextureGenerationOption &opt) const
{
	if(YSTRUE!=opt.needUpScaling)
	{
		return YsTextureManager::TransferRGBABitmap(texPtr,wid,hei,rgba);
	}
	else
	{
		upScaleWid=UpscaleDimension(wid);
		upScaleHei=UpscaleDimension(hei);
		texCoordScale.Set((double)wid/(double)upScaleWid,(double)hei/(double)upScaleHei);

		auto *usrgba=new unsigned char [upScaleWid*upScaleHei*4];

		for(int y=0; y<upScaleHei; ++y)
		{
			for(int x=0; x<upScaleWid; ++x)
			{
				auto dstPtr=usrgba+4*(y*upScaleWid+x);
				if(x<wid && upScaleHei-hei<=y)
				{
					auto srcPtr=rgba+4*((y-(upScaleHei-hei))*wid+x);
					dstPtr[0]=srcPtr[0];
					dstPtr[1]=srcPtr[1];
					dstPtr[2]=srcPtr[2];
					dstPtr[3]=srcPtr[3];
				}
				else
				{
					dstPtr[0]=0;
					dstPtr[1]=0;
					dstPtr[2]=0;
					dstPtr[3]=0;
				}
			}
		}

		auto res=YsTextureManager::TransferRGBABitmap(texPtr,upScaleWid,upScaleHei,usrgba);
		delete [] usrgba;
		return res;
	}
}

int YsTextureManager::Unit::UpscaleDimension(int d) const
{
	int us=1;
	while(us<d)
	{
		us*=2;
	}
	return us;
}

YsBitmap YsTextureManager::Unit::GetBitmap(void) const
{
	YsBitmap bmp;
	if(0==(flags&FLAG_RENDERTARGET))
	{
		switch(fType)
		{
		case FOM_PNG:
			bmp.LoadPng(dat.GetN(),dat);
			// LoadPng function is flipping the image in MoveFrom function
			// so that (0,0) is the top-left.
			break;
		case FOM_RAW_RGBA:
			bmp.PrepareBitmap(wid,hei);
			{
				auto rgba=bmp.GetEditableRGBABitmapPointer();
				for(YSSIZE_T idx=0; idx<wid*hei*4; ++idx)
				{
					rgba[idx]=dat[idx];
				}
				bmp.Invert();
				// Bitmap must be flipped to be consistent with LoadPng function for FOM_PNG.
			}
			break;
		default:
			break;
		}
	}
	return bmp;
}

YsTextureManager::DATA_FORMAT YsTextureManager::Unit::GetFileType(void) const
{
	return fType;
}

YsTextureManager::ActualTexture *YsTextureManager::Unit::GetActualTexture(void) const
{
	return texPtr;
}

const YsString &YsTextureManager::Unit::GetLabel(void) const
{
	return label;
}

YsTextureManager::Unit::FILTERTYPE YsTextureManager::Unit::GetFilterType(void) const
{
	return filterType;
}

const YsWString &YsTextureManager::Unit::GetFileName(void) const
{
	return fn;
}

const YsArray <unsigned char> &YsTextureManager::Unit::GetFileData(void) const
{
	return dat;
}

const double YsTextureManager::Unit::GetRandomNoiseLevel(void) const
{
	return randomNoiseLevel;
}

void YsTextureManager::Unit::SetRandomNoiseLevel(const double randomNoiseLevel)
{
	CleanTextureCache();
	this->randomNoiseLevel=randomNoiseLevel;
}

YSBOOL YsTextureManager::Unit::IsRenderTarget(void) const
{
	if(0!=(flags&FLAG_RENDERTARGET))
	{
		return YSTRUE;
	}
	return YSFALSE;
}

YSBOOL YsTextureManager::Unit::MustNotBeSavedToFile(void) const
{
	if(0!=(flags&FLAG_RENDERTARGET))
	{
		return YSTRUE;
	}
	if(0!=(flags&FLAG_DONTSAVE))
	{
		return YSTRUE;
	}
	return YSFALSE;
}

////////////////////////////////////////////////////////////

YsTextureManager::YsTextureManager()
{
	ClearLoadingInfo();
}

void YsTextureManager::CleanUp(void)
{
	TexHandle texHd=nullptr;
	while(YSOK==bmpArray.MoveToNextIncludingFrozen(texHd) && nullptr!=texHd)
	{
		auto texPtr=bmpArray.GetObjectMaybeFrozen(texHd);
		if(nullptr!=texPtr)
		{
			texPtr->CleanUp();
		}
	}
	bmpArray.ClearDeep();
	bmpDict.CleanUp();
}

void YsTextureManager::SetTextureGenerationOption(TextureGenerationOption opt)
{
	this->genOpt=opt;
}
YsTextureManager::TextureGenerationOption YsTextureManager::GetTextureGenerationOption(void) const
{
	return this->genOpt;
}

YSBOOL YsTextureManager::IsLabelUsed(const YsString &str) const
{
	return bmpDict.IsWordIncluded(str);
}

YsString YsTextureManager::MakeUniqueLabel(void) const
{
	YsString newLabel;
	for(;;)
	{
		char label[17];
		for(int i=0; i<16; ++i)
		{
			int r=rand()%('Z'+1-'A'+10);
			if(r<10)
			{
				label[i]='0'+r;
			}
			else
			{
				label[i]='A'+r-10;
			}
		}
		label[16]=0;

		newLabel=label;
		if(YSTRUE!=IsLabelUsed(newLabel))
		{
			break;
		}
	}
	return newLabel;
}

YsTextureManager::TexHandle YsTextureManager::AddTexture(const YsString &labelIn)
{
	YsString label=labelIn;
	if(0==labelIn.Strlen())
	{
		label=MakeUniqueLabel();
	}

	if(YSTRUE==bmpDict.IsWordIncluded(label))
	{
		return nullptr;
	}

	auto bmpHd=bmpArray.Create();
	if(NULL!=bmpHd)
	{
		bmpArray[bmpHd]->CleanUp();
		bmpArray[bmpHd]->owner=this;
		bmpArray[bmpHd]->SetLabel(label);
		bmpDict.AddWord(label,bmpHd);
		return bmpHd;
	}
	return NULL;
}

YsTextureManager::TexHandle YsTextureManager::AddTexture(const YsString &labelIn,DATA_FORMAT fType,int w,int h,YSSIZE_T length,const unsigned char fileData[])
{
	YsString label=labelIn;
	if(0==labelIn.Strlen())
	{
		label=MakeUniqueLabel();
	}

	if(YSTRUE==bmpDict.IsWordIncluded(label))
	{
		return nullptr;
	}

	auto bmpHd=bmpArray.Create();
	if(NULL!=bmpHd)
	{
		bmpArray[bmpHd]->CleanUp();
		bmpArray[bmpHd]->owner=this;
		bmpArray[bmpHd]->SetData(fType,w,h,length,fileData);
		bmpArray[bmpHd]->SetLabel(label);
		bmpDict.AddWord(label,bmpHd);
		return bmpHd;
	}
	return NULL;
}

YsTextureManager::TexHandle YsTextureManager::AddTexture(const YsString &label,DATA_FORMAT fType,int w,int h,YsConstArrayMask <unsigned char> dat)
{
	return AddTexture(label,fType,w,h,dat.GetN(),dat);
}

YsTextureManager::TexHandle YsTextureManager::AddShadowMapTexture(const YsString &labelIn,DATA_FORMAT fType,int w,int h)
{
	YsString label=labelIn;
	if(0==labelIn.Strlen())
	{
		label=MakeUniqueLabel();
	}

	if(YSTRUE==bmpDict.IsWordIncluded(label))
	{
		return nullptr;
	}

	auto bmpHd=bmpArray.Create();
	if(nullptr!=bmpHd)
	{
		bmpArray[bmpHd]->CleanUp();
		bmpArray[bmpHd]->owner=this;
		bmpArray[bmpHd]->MakeShadowMap(w,h);
		bmpArray[bmpHd]->SetLabel(label);
		bmpDict.AddWord(label,bmpHd);
		return bmpHd;
	}
	return nullptr;
}

void YsTextureManager::Delete(TexHandle texHd)
{
	auto texPtr=bmpArray[texHd];
	if(nullptr!=texPtr)
	{
		bmpDict.DeleteWord(GetLabel(texHd));
		texPtr->CleanUp();
		bmpArray.Delete(texHd);
	}
}

YsTextureManager::TexHandle YsTextureManager::Duplicate(TexHandle texHd,const YsString &labelIn)
{
	YsString label=labelIn;
	if(0==labelIn.Strlen())
	{
		label=MakeUniqueLabel();
	}

	if(YSTRUE==bmpDict.IsWordIncluded(label))
	{
		return NULL;
	}

	if(nullptr!=bmpArray[texHd])
	{
		auto newTexHd=bmpArray.Create();
		if(NULL!=newTexHd)
		{
			bmpArray[newTexHd]->CopyFrom(*bmpArray[texHd]);
			bmpArray[newTexHd]->label=label;
			bmpDict.AddWord(label,newTexHd);
			return newTexHd;
		}
	}
	return nullptr;
}

YsString YsTextureManager::GetLabel(YsTextureManager::TexHandle texHd) const
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex)
	{
		return tex->label;
	}
	return YsString("");
}

YsTextureManager::TexHandle YsTextureManager::FindTextureFromLabel(const YsString &label) const
{
	auto texHandlePtr=bmpDict.FindAttrib(label);
	if(NULL!=texHandlePtr)
	{
		return *texHandlePtr;
	}
	return NULL;
}

YSRESULT YsTextureManager::FreezeTexture(TexHandle texHd)
{
	if(YSTRUE!=bmpArray.IsFrozen(texHd))
	{
		auto label=GetLabel(texHd);
		YSRESULT res=bmpArray.Freeze(texHd);
		if(YSOK==res)
		{
			bmpDict.DeleteWord(label);
			return YSOK;
		}
	}
	return YSERR;
}

YSRESULT YsTextureManager::MeltTexture(TexHandle texHd)
{
	if(YSTRUE==bmpArray.IsFrozen(texHd))
	{
		auto res=bmpArray.Melt(texHd);
		if(YSOK==res)
		{
			auto label=GetLabel(texHd);
			bmpDict.AddWord(label,texHd);
			return YSOK;
		}
	}
	return YSERR;
}

YsEditArray <YsTextureManager::Unit,8>::HandleEnumerator YsTextureManager::AllTexture(void) const
{
	return bmpArray.AllHandle();
}

YsTextureManager::DATA_FORMAT YsTextureManager::GetTextureFormat(TexHandle texHd) const
{
	return bmpArray[texHd]->fType;
}

YSBOOL YsTextureManager::IsReady(TexHandle texHd) const
{
	if(nullptr!=texHd)
	{
		auto unitPtr=GetTexture(texHd);
		if(nullptr!=unitPtr)
		{
			return unitPtr->IsActualTextureReady();
		}
	}
	return YSFALSE;
}

YsVec2i YsTextureManager::GetTextureSize(TexHandle texHd) const
{
	auto &bmp=*bmpArray[texHd];
	if(FOM_RAW_RGBA==bmp.fType)
	{
		return YsVec2i(bmp.wid,bmp.hei);
	}
	return YsVec2i(0,0);
}

YSRESULT YsTextureManager::SetTextureFileName(TexHandle texHd,const YsWString &fn)
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex)
	{
		tex->SetFileName(fn);
		return YSOK;
	}
	return YSERR;
}

YsWString YsTextureManager::GetTextureFileName(TexHandle texHd) const
{
	return bmpArray[texHd]->fn;
}

YSRESULT YsTextureManager::SetTextureFilterType(YsTextureManager::TexHandle texHd,YsTextureManager::Unit::FILTERTYPE filterType)
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex)
	{
		if(tex->filterType!=filterType)
		{
			tex->CleanTextureCache();
		}
		tex->filterType=filterType;
		return YSOK;
	}
	return YSERR;
}

const YsTextureManager::Unit::FILTERTYPE YsTextureManager::GetTextureFilterType(YsTextureManager::TexHandle texHd) const
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex)
	{
		return tex->filterType;
	}
	return Unit::FILTERTYPE_NEAREST;
}

const YsArray <unsigned char> &YsTextureManager::GetTextureFileData(TexHandle texHd) const
{
	return bmpArray[texHd]->dat;
}

YSRESULT YsTextureManager::SetTextureFileData(TexHandle texHd,DATA_FORMAT fType,int w,int h,YSSIZE_T length,const unsigned char fileData[])
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex)
	{
		tex->SetData(fType,w,h,length,fileData);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsTextureManager::SetDontSave(TexHandle texHd,YSBOOL dontSave)
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex)
	{
		tex->SetDontSave(dontSave);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsTextureManager::SetTextureFileData(TexHandle texHd,DATA_FORMAT fType,int w,int h,YsConstArrayMask <unsigned char> fileData)
{
	return SetTextureFileData(texHd,fType,w,h,fileData.GetN(),fileData);
}

const double YsTextureManager::GetRandomNoiseLevel(TexHandle texHd) const
{
	return bmpArray[texHd]->GetRandomNoiseLevel();
}

YSRESULT YsTextureManager::SetRandomNoiseLevel(TexHandle texHd,const double randomNoiseLevel)
{
	auto tex=bmpArray[texHd];
	if(NULL!=tex && randomNoiseLevel!=tex->GetRandomNoiseLevel())
	{
		tex->CleanTextureCache();
		tex->SetRandomNoiseLevel(randomNoiseLevel);
		return YSOK;
	}
	return YSOK;
}

const YsTextureManager::Unit *YsTextureManager::GetTexture(TexHandle texHd) const
{
	return bmpArray[texHd];
}

const YsTextureManager::Unit *YsTextureManager::GetTextureReady(TexHandle texHd) const
{
	if(nullptr!=texHd)
	{
		auto unitPtr=bmpArray[texHd];
		if(nullptr!=unitPtr)
		{
			if(YSTRUE!=unitPtr->IsActualTextureReady())
			{
				unitPtr->MakeActualTexture(genOpt);
			}
			if(YSTRUE==unitPtr->IsActualTextureReady())
			{
				return unitPtr;
			}
		}
	}
	return nullptr;
}

YSRESULT YsTextureManager::Save(YsTextOutputStream &outStream) const
{
	for(auto texHd : AllTexture())
	{
		auto unitPtr=GetTexture(texHd);
		if(YSTRUE==unitPtr->MustNotBeSavedToFile())
		{
			continue;
		}


		YsString label=GetLabel(texHd);
		outStream.Printf("TEXMAN TEXTURE \"%s\"\n",label.Txt());

		YsString fType(GetStringFromFormat(GetTextureFormat(texHd)));
		outStream.Printf("TEXMAN TEXTUREFORMAT %s\n",fType.Txt());

		if(FOM_RAW_RGBA==GetTextureFormat(texHd))
		{
			auto reso=GetTextureSize(texHd);
			outStream.Printf("TEXMAN TEXTURERESOLUTION %d %d\n",reso.x(),reso.y());
		}


		YsWString wfn=GetTextureFileName(texHd);
		if(0<wfn.Strlen())
		{
			YsString fn;
			fn.EncodeUTF8 <wchar_t> (wfn);
			outStream.Printf("TEXMAN TEXTUREFILE \"%s\"\n",fn.Txt());
		}

		switch(GetTextureFilterType(texHd))
		{
		case YsTextureManager::Unit::FILTERTYPE_NEAREST:
			outStream.Printf("TEXMAN TEXTUREFILTER NEAREST\n");
			break;
		case YsTextureManager::Unit::FILTERTYPE_LINEAR:
			outStream.Printf("TEXMAN TEXTUREFILTER LINEAR\n");
			break;
		}

		YsArray <YsString> base64;
		YsEncodeBase64(base64,GetTextureFileData(texHd));

		outStream.Printf("TEXMAN TEXTUREDATA %d\n",base64.GetN());
		for(auto &str : base64)
		{
			outStream.Printf("TEXMAN %s\n",str.Txt());
		}

		outStream.Printf("TEXMAN RANDOMNOISE %.2lf\n",GetRandomNoiseLevel(texHd));

		outStream.Printf("TEXMAN ENDTEXTURE\n");
	}
	return YSOK;
}

void YsTextureManager::BeginLoadTexMan(void)
{
	ClearLoadingInfo();
}

void YsTextureManager::EndLoadTexMan(void)
{
	ClearLoadingInfo();
}

void YsTextureManager::ClearLoadingInfo(void)
{
	loadingState=LOADINGSTATE_NORMAL;
	loadingTexFileName.Set("");
	loadingTexFType=FOM_UNKNOWN;
	loadingTexRandomNoiseLevel=0.05;
	loadingTexLabel.Set("");
	loadingTexData.CleanUp();
	nLoadingTexDataLine=0;
	base64decoder.CleanUp();
	loadingFilterType=Unit::FILTERTYPE_NEAREST;
}

static const char *const texManCmd[]=
{
	"TEXTURE",      // 0 Begin Texture definition
	"TEXTUREFORMAT",// 1 Texture format
	"TEXTUREFILE",  // 2 Texture file name
	"TEXTUREDATA",  // 3 Texture data
	"ENDTEXTURE",   // 4 End Texture definition
	"TEXTUREFILTER",// 5 Texture filter
	"RANDOMNOISE",  // 6 Add random noise true/false
	"TEXTURERESOLUTION",  // 7 Texture resolution.  Used if the format is FOM_RAW_RGBA
	NULL
};

YSRESULT YsTextureManager::LoadTexManOneLine(const YsString &str,YSSIZE_T argc,const YsString args[])
{
	static YsKeyWordList keyWordList;

	if(keyWordList.GetN()==0)
	{
		keyWordList.MakeList(texManCmd);
	}

	if(LOADINGSTATE_NORMAL==loadingState)
	{
		const int cmd=keyWordList.GetId(args[1]);  // args[0] is always "TEXMAN"
		switch(cmd)
		{
		case 0: //	"TEXTURE",    // Texture
			if(3<=argc)
			{
				loadingTexFileName.Set("");
				loadingTexLabel.Set("");
				loadingTexData.CleanUp();
				nLoadingTexDataLine=0;
				base64decoder.CleanUp();
				loadingFilterType=Unit::FILTERTYPE_NEAREST;
				loadingTexSize.Set(0,0);

				loadingTexLabel=args[2];

				return YSOK;
			}
			break;
		case 1: //	"TEXTUREFORMAT",// Texture format
			if(3<=argc)
			{
				loadingTexFType=GetFormatFromString(args[2]);
				return YSOK;
			}
			break;
		case 2: //	"TEXTUREFILE",// Texture file name
			if(3<=argc)
			{
				loadingTexFileName=args[2];
				return YSOK;
			}
			break;
		case 3: //	"TEXTUREDATA",// Texture data
			if(3<=argc)
			{
				nLoadingTexDataLine=atoi(args[2]);
				base64decoder.CleanUp();
				loadingTexData.CleanUp();
				loadingState=LOADINGSTATE_TEXTUREDATA;
				return YSOK;
			}
			break;
		case 4: //	"ENDTEXTURE", // End Texture definition
			{
				base64decoder.Flush(loadingTexData);
				auto texHd=AddTexture(loadingTexLabel,loadingTexFType,loadingTexSize.x(),loadingTexSize.y(),loadingTexData);
				YsWString wfn;
				wfn.SetUTF8String(loadingTexFileName);
				SetTextureFileName(texHd,wfn);
				SetTextureFilterType(texHd,loadingFilterType);
				SetRandomNoiseLevel(texHd,loadingTexRandomNoiseLevel);
			}
			loadingTexFileName.Set("");
			loadingTexLabel.Set("");
			loadingTexData.CleanUp();
			nLoadingTexDataLine=0;
			base64decoder.CleanUp();
			return YSOK;
			break;
		case 5: // "TEXTUREFILTER",// 5 Texture filter
			if(3<=argc)
			{
				if(0==strcmp(args[2],"LINEAR"))
				{
					loadingFilterType=Unit::FILTERTYPE_LINEAR;
				}
				else
				{
					loadingFilterType=Unit::FILTERTYPE_NEAREST;
				}
				return YSOK;
			}
			break;
		case 6: // "RANDOMNOISE"
			if(3<=argc)
			{
				loadingTexRandomNoiseLevel=atof(args[2]);
				return YSOK;
			}
			break;
		case 7: // "TEXTURERESOLUTION"
			if(4<=argc)
			{
				loadingTexSize.Set(atoi(args[2]),atoi(args[3]));
			}
			break;
		}
	}
	else if(LOADINGSTATE_TEXTUREDATA==loadingState)
	{
		if(2<=argc)
		{
			base64decoder.DecodeString(loadingTexData,args[1]);
		}
		--nLoadingTexDataLine;
		if(0>=nLoadingTexDataLine)
		{
			loadingState=LOADINGSTATE_NORMAL;
		}
		return YSOK;
	}

	YsPrintf("Error in Texture command.\n");
	YsPrintf("  %s\n",str.Txt());
	return YSERR;
}



