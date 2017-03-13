#include <fslazywindow.h>
#include <ysclass.h>

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;

	int nVtx;
	YsVec2 tri[6];

	YsVec2i cursorPos;

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
	nVtx=0;
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
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}


	int lb,mb,rb;
	int mx,my;
	auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);

	int cx=mx/50;
	int cy=my/50;
	cx*=50;
	cy*=50;
	cursorPos.Set(cx,cy);

	if(FSMOUSEEVENT_LBUTTONDOWN==evt)
	{
		if(nVtx<6)
		{
			tri[nVtx].Set(cursorPos.x(),cursorPos.y());
			++nVtx;

			for(int i=nVtx; i<6; ++i)
			{
				tri[i]=tri[i-1];
			}

			if(6==nVtx)
			{
				for(auto t : tri)
				{
					printf("%s\n",t.Txt());
				}
				printf("%s\n",YsIntersectionToStr(YsCheckTriangleIntersection(tri,tri+3)));
			}
		}
		else if(6==nVtx)
		{
			nVtx=0;
		}
	}


	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glColor3f(0,0,0);
	glBegin(GL_LINES);
	glVertex2i(cursorPos.x()-10,cursorPos.y());
	glVertex2i(cursorPos.x()+10,cursorPos.y());
	glVertex2i(cursorPos.x(),cursorPos.y()-10);
	glVertex2i(cursorPos.x(),cursorPos.y()+10);
	glEnd();


	if(0<nVtx)
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		for(auto t : tri)
		{
			glVertex2dv(t);
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2dv(tri[0]);
		glVertex2dv(tri[1]);
		glVertex2dv(tri[2]);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2dv(tri[3]);
		glVertex2dv(tri[4]);
		glVertex2dv(tri[5]);
		glEnd();
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
