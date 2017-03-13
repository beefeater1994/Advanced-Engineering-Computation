#include <fslazywindow.h>
#include <fssimplefiledialog.h>
#include <yssystemfont.h>
#include <ystexturemanager.h>
#include <ysgl.h>
#include <ysglslcpp.h>

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;

	YsSystemFontCache sysFont;
	YsTextureManager texMan;
	YsTextureManager::TexHandle msgTexHd,fileNameTexHd;
	YsWString selFileName;

public:
	FsLazyWindowApplication();
	virtual void BeforeEverything(int argc,char *argv[]);
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const;
	virtual void Initialize(int argc,char *argv[]);
	virtual void Interval(void);
	virtual void BeforeTerminate(void);
	virtual void Draw(void);
	virtual bool UserWantToCloseProgram(void);
	virtual bool MustTerminate(void) const;
	virtual long long int GetMinimumSleepPerInterval(void) const;
	virtual bool NeedRedraw(void) const;

	FsSimpleFileDialog fdlg;
	void FileSelected(FsSimpleFileDialog *dlgPtr,int returnCode);
};

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{
}
/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &opt) const
{
	opt.x0=0;
	opt.y0=0;
	opt.wid=1200;
	opt.hei=800;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{
	YsGLSLCreateSharedRenderer();

	sysFont.RequestDefaultFontWithHeight(16);

	auto genOpt=texMan.GetTextureGenerationOption();
	genOpt.needUpScaling=YSTRUE;
	texMan.SetTextureGenerationOption(genOpt);

	msgTexHd=texMan.AddTexture("");
	fileNameTexHd=texMan.AddTexture("");

	const unsigned char fgCol[3]={0,0,0};
	const unsigned char bgCol[3]={0,0,0};
	auto msgFontBmp=sysFont.MakeRGBABitmap(L"Press Space Key",fgCol,bgCol,YSFALSE);
	texMan.SetTextureFileData(msgTexHd,texMan.FOM_RAW_RGBA,msgFontBmp.Width(),msgFontBmp.Height(),4*msgFontBmp.Width()*msgFontBmp.Height(),msgFontBmp.Bitmap());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}
	if(FSKEY_SPACE==key)
	{
		fdlg.Initialize();
		fdlg.title=L"Test";
		fdlg.mode=fdlg.MODE_OPEN;
		fdlg.fileExtensionArray.Add(L".txt");
		fdlg.fileExtensionArray.Add(L".h");
		fdlg.fileExtensionArray.Add(L".cpp");
		fdlg.defaultFileName.SetUTF8String("");
		fdlg.BindCloseModalCallBack(&FsLazyWindowApplication::FileSelected,this);
		fdlg.Show();
	}
	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	int winWid,winHei;
	FsGetWindowSize(winWid,winHei);
	glViewport(0,0,winWid,winHei);

	{
		YsGLVertexBuffer2D vtxBuf;
		YsGLVertexBuffer2D texCoordBuf;
		auto unitPtr=texMan.GetTexture(msgTexHd);
		if(nullptr!=unitPtr)
		{
			auto w=unitPtr->GetWidth();
			auto h=unitPtr->GetHeight();

			vtxBuf.Add<float>(0,0);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(0,1));
			vtxBuf.Add<float>(w,0);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(1,1));
			vtxBuf.Add<float>(w,h);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(1,0));
			vtxBuf.Add<float>(0,h);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(0,0));

			unitPtr->Bind(0);

			auto renderer=YsGLSLSharedPlain2DRenderer();
			YsGLSLUsePlain2DRenderer(renderer);

			YsGLSLUseWindowCoordinateInPlain2DDrawing(renderer,1);

			const GLfloat col[4]={1,0,1,1};
			YsGLSLSetPlain2DRendererUniformColor(renderer,col);
			YsGLSLDrawPlain2DPrimitiveVtxTexCoordfv(renderer,GL_TRIANGLE_FAN,vtxBuf.GetN(),vtxBuf,texCoordBuf);

			YsGLSLEndUsePlain2DRenderer(renderer);
		}
	}

	{
		YsGLVertexBuffer2D vtxBuf;
		YsGLVertexBuffer2D texCoordBuf;

		const unsigned char fgCol[3]={0,0,0};
		const unsigned char bgCol[3]={0,0,0};
		auto txtFontBmp=sysFont.MakeRGBABitmap(selFileName,fgCol,bgCol,YSFALSE);
		texMan.SetTextureFileData(fileNameTexHd,texMan.FOM_RAW_RGBA,txtFontBmp.Width(),txtFontBmp.Height(),4*txtFontBmp.Width()*txtFontBmp.Height(),txtFontBmp.Bitmap());

		auto unitPtr=texMan.GetTexture(fileNameTexHd);
		if(nullptr!=unitPtr)
		{
			auto w=unitPtr->GetWidth();
			auto h=unitPtr->GetHeight();

			vtxBuf.Add<float>(0,100);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(0,1));
			vtxBuf.Add<float>(w,100);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(1,1));
			vtxBuf.Add<float>(w,100+h);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(1,0));
			vtxBuf.Add<float>(0,100+h);
			texCoordBuf.Add(unitPtr->ScaleTexCoord(0,0));

			unitPtr->Bind(0);

			auto renderer=YsGLSLSharedPlain2DRenderer();
			YsGLSLUsePlain2DRenderer(renderer);

			YsGLSLUseWindowCoordinateInPlain2DDrawing(renderer,1);

			const GLfloat col[4]={1,0,1,1};
			YsGLSLSetPlain2DRendererUniformColor(renderer,col);
			YsGLSLDrawPlain2DPrimitiveVtxTexCoordfv(renderer,GL_TRIANGLE_FAN,vtxBuf.GetN(),vtxBuf,texCoordBuf);

			YsGLSLEndUsePlain2DRenderer(renderer);
		}
	}

	FsSwapBuffers();

	needRedraw=false;
}
/* virtual */ bool FsLazyWindowApplication::UserWantToCloseProgram(void)
{
	return true; // Returning true will just close the program.
}
/* virtual */ bool FsLazyWindowApplication::MustTerminate(void) const
{
	return FsLazyWindowApplicationBase::MustTerminate();
}
/* virtual */ long long int FsLazyWindowApplication::GetMinimumSleepPerInterval(void) const
{
	return 10;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	return needRedraw;
}


static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}

void FsLazyWindowApplication::FileSelected(FsSimpleFileDialog *dlgPtr,int returnCode)
{
	printf("File Selected.\n");
	for(auto selFn : dlgPtr->selectedFileArray)
	{
		auto utf8=selFn.GetUTF8String();
		printf("  %s\n",utf8.Txt());
		selFileName=selFn;
	}
}
