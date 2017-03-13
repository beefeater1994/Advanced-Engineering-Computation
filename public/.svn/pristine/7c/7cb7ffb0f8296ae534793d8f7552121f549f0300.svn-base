#ifndef YSGLPARTICLEMANAGER_IS_INCLUDED
#define YSGLPARTICLEMANAGER_IS_INCLUDED
/* { */

#include "ysglbuffer.h"

class YsGLParticleManager
{
public:
	class Particle
	{
	public:
		YsVec3 pos;
		YsColor col;
		float dimension;
		float texCoord[2];
	};

	YsGLVertexBuffer vtxBuf;
	YsGLVertexBuffer2D texCoordBuf;
	YsGLColorBuffer colBuf;
	YsGLPointSizeBuffer psBuf;

private:
	YsArray <Particle> particle;
	YsArray <YSSIZE_T> particleOrder;
	YsArray <double> particleDist;

public:
	YsGLParticleManager();
	~YsGLParticleManager();
	void CleanUp(void);

	YSSIZE_T GetNumParticle(void) const;

	/*! Adds a particle.  It assumes that each particle takes a constant rectangular space in the
	    texture coordinate.  That is, the pattern is (sMin,tMin)-(sMin+sWid,tMin+tWid) of the current 
	    texture.  In OpenGL 2.0 wrapper, it is specified by SetPointSpriteTextureCoordRange function.
	*/
	inline void Add(const YsVec3 &pos,YsColor col,float dim,float sMin,float tMin)
	{
		particle.Increment();
		particle.Last().pos=pos;
		particle.Last().col=col;
		particle.Last().dimension=dim;
		particle.Last().texCoord[0]=sMin;
		particle.Last().texCoord[1]=tMin;
	}
	inline void Add(const YsVec3 &pos,YsColor col,float dim,YsVec2 &texMin)
	{
		Add(pos,col,dim,texMin.xf(),texMin.yf());
	}
	inline void Add(const YsVec3 &pos,YsColor col,float dim,const float texCoord[2])
	{
		Add(pos,col,dim,texCoord[0],texCoord[1]);
	}

	/*! Sort particles in the given view direction.  Technically, view point is not necessary,
	    but probably it is good for reducing artifacts by the numerical error.
	*/
	void Sort(const YsVec3 &viewPos,const YsVec3 & viewDir);

	/*! Call this function after Sort to populate vertex attribute buffers.
	*/
	void MakeBuffer(void);
};


/* } */
#endif
