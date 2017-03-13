#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fslazywindow.h>
#include <ysglfontdata.h>

#include "bintree.h"



class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;

	BinaryTree <int,int> tree;
	BinaryTree <int,int>::NodeHandle mouseOnHd;
	int xMin,yMin,xMax,yMax;

public:
	void DrawBinaryTree(BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y0,int yStep,int prevX,int prevY) const;
	BinaryTree <int,int>::NodeHandle FindBinaryTreeNode(
	    int &xMin,int &yMin,int &xMax,int &yMax,
	    BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y0,int yStep,int mx,int my) const;

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

void FsLazyWindowApplication::DrawBinaryTree(
    BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y0,int yStep,int prevX,int prevY) const
{
	if(true==ndHd.IsNull())
	{
		return;
	}

	if(mouseOnHd!=ndHd)
	{
		glColor3ub(0,0,0);
	}
	else
	{
		glColor3ub(255,0,0);
	}

	const int cx=(x0+x1)/2;
	const int cy=y0+yStep/2;
	if(true==tree.Up(ndHd).IsNotNull())
	{
		glBegin(GL_LINES);
		glVertex2i(prevX,prevY);
		glVertex2i(cx,cy);
		glEnd();
	}

	char str[256];
	sprintf(str,"%d(%d)",tree.GetKey(ndHd),tree.GetHeight(ndHd));
	glRasterPos2i(cx,y0+yStep-1);
	YsGlDrawFontBitmap16x24(str);

	DrawBinaryTree(tree.Left(ndHd),x0,cx,y0+yStep,yStep,cx,cy);
	DrawBinaryTree(tree.Right(ndHd),cx,x1,y0+yStep,yStep,cx,cy);
}

BinaryTree <int,int>::NodeHandle FsLazyWindowApplication::FindBinaryTreeNode(
	    int &xMin,int &yMin,int &xMax,int &yMax,
	    BinaryTree <int,int>::NodeHandle ndHd,int x0,int x1,int y0,int yStep,int mx,int my) const
{
	if(true==ndHd.IsNull())
	{
		return ndHd;
	}

	if(x0<=mx && mx<=x1 && y0<=my && my<=y0+yStep)
	{
		xMin=x0;
		yMin=y0;
		xMax=x1;
		yMax=y0+yStep;
		return ndHd;
	}

	const int cx=(x0+x1)/2;
	const int cy=y0+yStep/2;
	auto checkLeft=FindBinaryTreeNode(xMin,yMin,xMax,yMax,tree.Left(ndHd),x0,cx,y0+yStep,yStep,mx,my);
	if(true==checkLeft.IsNotNull())
	{
		return checkLeft;
	}
	auto checkRight=FindBinaryTreeNode(xMin,yMin,xMax,yMax,tree.Right(ndHd),cx,x1,y0+yStep,yStep,mx,my);
	if(true==checkRight.IsNotNull())
	{
		return checkRight;
	}
	return tree.Null();
}

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
	mouseOnHd.Nullify();
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
	srand((int)time(nullptr));

	for(int i=0; i<40; ++i)
	{
		tree.Insert(rand()%100,0);
	}
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}

	int lb,mb,rb,mx,my;
	FsGetMouseEvent(lb,mb,rb,mx,my);
	mouseOnHd=FindBinaryTreeNode(xMin,yMin,xMax,yMax,tree.RootNode(),0,800,0,40,mx,my);

	if(FSKEY_DEL==key && true==mouseOnHd.IsNotNull())
	{
		tree.Delete(mouseOnHd);
	}
	if(FSKEY_L==key && true==mouseOnHd.IsNotNull())
	{
		tree.RotateLeft(mouseOnHd);
	}

	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(true==mouseOnHd.IsNotNull())
	{
		glColor3ub(128,128,128);
		glBegin(GL_QUADS);
		glVertex2i(xMin,yMin);
		glVertex2i(xMax,yMin);
		glVertex2i(xMax,yMax);
		glVertex2i(xMin,yMax);
		glEnd();
	}

	DrawBinaryTree(tree.RootNode(),0,800,0,40,0,0);
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
