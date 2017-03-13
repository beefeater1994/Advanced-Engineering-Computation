#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>

#include <fslazywindow.h>

template <class T>
class Lattice2d
{
private:
	Lattice2d(const Lattice2d <T> &);
	Lattice2d <T> &operator=(const Lattice2d <T> &);

protected:
	T *cell;
	int nx,ny;
	double hx,hy;

public:
	Lattice2d();
	~Lattice2d();
	void CleanUp(void);

	bool Create(int nx,int ny);

	int GetNx(void) const;
	int GetNy(void) const;

	T *GetNode(int x,int y);
	const T *GetNode(int x,int y) const;
};

template <class T>
Lattice2d<T>::Lattice2d()
{
	nx=0;
	ny=0;
	hx=0;
	hy=0;
	cell=nullptr;
}

template <class T>
Lattice2d<T>::~Lattice2d()
{
	CleanUp();
}

template <class T>
void Lattice2d<T>::CleanUp(void)
{
	if(nullptr!=cell)
	{
		delete [] cell;
	}
	nx=0;
	ny=0;
	hx=0;
	hy=0;
	cell=nullptr;
}

template <class T>
bool Lattice2d<T>::Create(int nx,int ny)
{
	CleanUp();
	cell=new T [(nx+1)*(ny+1)];
	this->nx=nx;
	this->ny=ny;
	return true;
}

template <class T>
int Lattice2d<T>::GetNx(void) const
{
	return nx;
}
template <class T>
int Lattice2d<T>::GetNy(void) const
{
	return ny;
}

template <class T>
T *Lattice2d<T>::GetNode(int x,int y)
{
	if(0<=x && x<=nx && 0<=y && y<=ny && nullptr!=cell)
	{
		return &cell[(nx+1)*y+x];
	}
	return nullptr;
}

template <class T>
const T *Lattice2d<T>::GetNode(int x,int y) const
{
	if(0<=x && x<=nx && 0<=y && y<=ny && nullptr!=cell)
	{
		return &cell[(nx+1)*y+x];
	}
	return nullptr;
}


class Color
{
public:
	unsigned char r,g,b;
};

void DrawColorLattice(const Lattice2d <Color> &ltc)
{
	const float pixPerBlk=8.0f;

	auto nx=ltc.GetNx();
	auto ny=ltc.GetNy();
	glBegin(GL_QUADS);
	for(int x=0; x<nx; ++x)
	{
		for(int y=0; y<ny; ++y)
		{
			auto node=ltc.GetNode(x,y);

			glColor3ub(node->r,node->g,node->b);
			glVertex2f((float)x*pixPerBlk,(float)y*pixPerBlk);
			glVertex2f((float)(x+1)*pixPerBlk,(float)y*pixPerBlk);
			glVertex2f((float)(x+1)*pixPerBlk,(float)(y+1)*pixPerBlk);
			glVertex2f((float)x*pixPerBlk,(float)(y+1)*pixPerBlk);
		}
	}
	glEnd();
}

class Coord
{
public:
	int x,y;
};

void FloodFill(Lattice2d <Color> &ltc,int nx,int ny,
   unsigned char srcR,unsigned char srcG,unsigned char srcB,
   unsigned char newR,unsigned char newG,unsigned char newB)
{
	auto node=ltc.GetNode(nx,ny);
	if(nullptr==node)
	{
		return;
	}
	if(node->r==srcR && node->g==srcG && node->b==srcB &&
	   (node->r!=newR || node->g!=newG || node->b!=newB))
	{
		std::vector <Coord> todo;
		node->r=newR;
		node->g=newG;
		node->b=newB;

		Coord xy;
		xy.x=nx;
		xy.y=ny;
		todo.push_back(xy);

		while(0<todo.size())
		{
			auto pos=todo.back();
			todo.pop_back();

			const int offset[]=
			{
				-1,0,
				1,0,
				0,-1,
				0,1
			};

			for(int i=0; i<4; ++i)
			{
				int x=pos.x+offset[i*2];
				int y=pos.y+offset[i*2+1];

				auto node=ltc.GetNode(x,y);
				if(nullptr!=node &&
				   node->r==srcR && node->g==srcG && node->b==srcB &&
				   (node->r!=newR || node->g!=newG || node->b!=newB))
				{
					node->r=newR;
					node->g=newG;
					node->b=newB;

					Coord newXY;
					newXY.x=x;
					newXY.y=y;
					todo.push_back(newXY);
				}
			}
		}
	}
}



class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;

	enum {
		nx=64,
		ny=64
	};
	Lattice2d <Color> ltc;


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
	opt.wid=800;
	opt.hei=600;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{
	glShadeModel(GL_SMOOTH);

	ltc.Create(nx,ny);
	for(int x=0; x<=nx; ++x)
	{
		for(int y=0; y<=ny; ++y)
		{
			auto node=ltc.GetNode(x,y);
			if(nullptr!=node)
			{
				node->r=0;
				node->g=0;
				node->b=0;
			}
			else
			{
				printf("Error! Lattice not created correctly.\n");
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

	int lb,rb,mb,mx,my;
	FsGetMouseEvent(lb,mb,rb,mx,my);

	int nx=mx/8;
	int ny=my/8;
	if(0!=lb)
	{
		auto node=ltc.GetNode(nx,ny);
		if(nullptr!=node)
		{
			node->r=255;
			node->g=255;
			node->b=255;
		}
	}
	if(FSKEY_SPACE==key)
	{
		auto node=ltc.GetNode(nx,ny);
		if(nullptr!=node &&
		   (node->r!=255 || node->g!=255 || node->b!=0))
		{
			FloodFill(ltc,nx,ny,node->r,node->g,node->b,255,255,0);
		}
	}
	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawColorLattice(ltc);
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
