#include <fslazywindow.h>
#include "hashtable.h"
#include "simplebitmap.h"
class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
	SimpleBitmap test;
	SimpleBitmap cut;
	HashTable<SimpleBitmap, int> ht;
	int index = 0;
	int number = 0;

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
	if (false == test.LoadPng(argv[1]))
	{
		printf("Error: Failed to read a .PNG file.\n");
	}
	if (argc == 1) {
		printf("Usage: ps3_1 <pngFileName.png> \n");
	}
	else
	{
		int width = 0;
		int height = 0;
		if (test.GetWidth() % 40 == 0) {
			width = test.GetWidth() / 40;
		}
		else {
			width = test.GetWidth() / 40 + 1;
		}
		if (test.GetHeight() % 40 == 0) {
			height = test.GetHeight() / 40;
		}
		else {
			height = test.GetHeight() / 40 + 1;
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				cut = test.CutOut(40 * j, 40 * i, 40, 40);
				if (!ht.IsIncluded(cut)) {
					ht.Update(cut, number);
					number++;
				}
			}
		}
	}
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}
	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	index = 0;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	for (auto a = ht.First(); ht.IsNotNull(a) == true; a = ht.Next(a), index++) 
	{
		SimpleBitmap now = ht.GetKey(a);
		now.Invert();
		glRasterPos2d(40 * index % 1200, 40 * index / 1200 * 40 + 40);
		glDrawPixels(now.GetWidth(), now.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, now.GetBitmapPointer());
		glFlush();
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
