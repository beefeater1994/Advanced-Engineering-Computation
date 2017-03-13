#include <ysmergesort.h>

#include "ysglparticlemanager.h"


YsGLParticleManager::YsGLParticleManager()
{
	CleanUp();
}
YsGLParticleManager::~YsGLParticleManager()
{
}
void YsGLParticleManager::CleanUp(void)
{
	vtxBuf.CleanUp();
	texCoordBuf.CleanUp();
	colBuf.CleanUp();
	psBuf.CleanUp();
	particle.CleanUp();
	particleOrder.CleanUp();
	particleDist.CleanUp();
}
YSSIZE_T YsGLParticleManager::GetNumParticle(void) const
{
	return particle.GetN();
}
void YsGLParticleManager::Sort(const YsVec3 &viewPos,const YsVec3 &viewDir)
{
	particleOrder.Set(particle.GetN(),nullptr);
	particleDist.Set(particle.GetN(),nullptr);

	for(auto idx : particle.AllIndex())
	{
		particleOrder[idx]=idx;
		particleDist[idx]=-(particle[idx].pos-viewPos)*viewDir;
	}

	YsSimpleMergeSort <double,YSSIZE_T> (particleDist.GetN(),particleDist,particleOrder);
}
void YsGLParticleManager::MakeBuffer(void)
{
	vtxBuf.CleanUp();
	texCoordBuf.CleanUp();
	colBuf.CleanUp();
	psBuf.CleanUp();

	for(auto idx : particleOrder)
	{
		auto &p=particle[idx];
		vtxBuf.Add(p.pos);
		texCoordBuf.Add(p.texCoord[0],p.texCoord[1]);
		colBuf.Add(p.col);
		psBuf.Add(p.dimension);
	}
}
