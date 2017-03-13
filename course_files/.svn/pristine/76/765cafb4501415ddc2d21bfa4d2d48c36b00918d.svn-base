#include <stdio.h>
#include <string.h>

#include "binary_stl.h"


bool IsASCIIStl(const char fn[])
{
	FILE *fp=fopen(fn,"rb");
	if(nullptr!=fp)
	{
		char buf[1024];
		int nBuf=fread(buf,1,1024,fp);
		fclose(fp);

		bool solid=false,facet=false,vertex=false,loop=false;
		for(int i=0; i<nBuf-6; ++i)
		{
			if(0==strncmp(buf+i,"solid",5))
			{
				solid=true;
			}
			if(80<=i)
			{
				if(0==strncmp(buf+i,"facet",5))
				{
					facet=true;
				}
				else if(0==strncmp(buf+i,"vertex",6))
				{
					vertex=true;
				}
				else if(0==strncmp(buf+i,"loop",4))
				{
					loop=true;
				}
			}
		}

		return (solid && facet && vertex && loop);
	}
	return false;
}



static bool IsLittleEndian(void)
{
	int i=1;
	char *ptr=(char *)&i;
	return (1==*ptr);
}

static unsigned int BinaryToInt(const unsigned char dat[4])
{
	if(true==IsLittleEndian())
	{
		unsigned char b0=dat[0];
	    unsigned char b1=dat[1];
	    unsigned char b2=dat[2];
	    unsigned char b3=dat[3];
	    return b0+b1*0x100+b2*0x10000+b3*0x1000000;
	}
	else
	{
		unsigned char b0=dat[3];
	    unsigned char b1=dat[2];
	    unsigned char b2=dat[1];
	    unsigned char b3=dat[0];
	    return b0+b1*0x100+b2*0x10000+b3*0x1000000;
	}
}
static float BinaryToFloat(const unsigned char dat[4])
{
	if(true==IsLittleEndian())
	{
	    const float *fPtr=(const float *)dat;
	    return *fPtr;
	}
	else
	{
		const unsigned char flipped[4]=
		{
			dat[3],dat[2],dat[1],dat[0]
		};
	    const float *fPtr=(const float *)flipped;
	    return *fPtr;
	}
}

bool ReadBinaryStl(std::vector <float> &vtx,std::vector <float> &nom,const char fn[])
{
	if(true==IsASCIIStl(fn))
	{
		printf("Is ASCII format\n");
		return false;
	}


	vtx.clear();
	nom.clear();

	FILE *fp=fopen(fn,"rb");
	if(nullptr!=fp)
	{
		unsigned char buf[256];

		if(80!=fread(buf,1,80,fp))
		{
			fclose(fp);
			return false;
		}

		fread(buf,1,4,fp);
		auto nTri=BinaryToInt(buf);
		printf("%d triangles expected\n",nTri);

		while(50==fread(buf,1,50,fp))
		{
			auto nx=BinaryToFloat(buf);
			auto ny=BinaryToFloat(buf+4);
			auto nz=BinaryToFloat(buf+8);

			auto x0=BinaryToFloat(buf+12);
			auto y0=BinaryToFloat(buf+16);
			auto z0=BinaryToFloat(buf+20);
			auto x1=BinaryToFloat(buf+24);
			auto y1=BinaryToFloat(buf+28);
			auto z1=BinaryToFloat(buf+32);
			auto x2=BinaryToFloat(buf+36);
			auto y2=BinaryToFloat(buf+40);
			auto z2=BinaryToFloat(buf+44);

			vtx.push_back(x0);vtx.push_back(y0);vtx.push_back(z0);
			vtx.push_back(x1);vtx.push_back(y1);vtx.push_back(z1);
			vtx.push_back(x2);vtx.push_back(y2);vtx.push_back(z2);

			nom.push_back(nx);nom.push_back(ny);nom.push_back(nz);
			nom.push_back(nx);nom.push_back(ny);nom.push_back(nz);
			nom.push_back(nx);nom.push_back(ny);nom.push_back(nz);
		}

		fclose(fp);
		return true;
	}
	return false;
}

void GetBoundingBox(YsVec3 &min,YsVec3 &max,const std::vector <float> &vtx)
{
	double minx,miny,minz,maxx,maxy,maxz;
	if(3<=vtx.size())
	{
		minx=vtx[0];
		miny=vtx[1];
		minz=vtx[2];
		maxx=vtx[0];
		maxy=vtx[1];
		maxz=vtx[2];
		for(int i=1; i<vtx.size()/3; ++i)
		{
			double x=vtx[i*3],y=vtx[i*3+1],z=vtx[i*3+2];
			minx=(x<minx ? x : minx);
			miny=(y<miny ? y : miny);
			minz=(z<minz ? z : minz);
			maxx=(x>maxx ? x : maxx);
			maxy=(y>maxy ? y : maxy);
			maxz=(z>maxz ? z : maxz);
		}
		min.Set(minx,miny,minz);
		max.Set(maxx,maxy,maxz);
	}
	else
	{
		min=YsVec3::Origin();
		max=YsVec3::Origin();
	}
}
