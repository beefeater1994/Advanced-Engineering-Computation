#ifndef YSGLCASCADEDSHADOWMAP_IS_INCLUDED
#define YSGLCASCADEDSHADOWMAP_IS_INCLUDED
/* { */

#include <ysclass.h>

class YsGLCascadedShadowMap
{
public:
	/*! Orthogonal shadow frustum extends from:
	    (-dx,-dy,0)-(dx,dy,farz)
	    Z direction is taken in the viewVec.
	*/
	class OrthogonalShadowFrustum
	{
	public:
		YsVec3 viewPoint;
		YsVec3 viewVec,upVec;
		double dx,dy,farz;
	};

protected:
	YsArray <OrthogonalShadowFrustum,4> orthoShadowFrustum;

public:
	YsGLCascadedShadowMap();
	~YsGLCascadedShadowMap();
	void CleanUp(void);

	/*! Caculate 2-level light frustums of of cascaded shadow map
	*/
	void CalculateShadowFrustum(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &dirToLight,
	    const double box0size,const double box1size);
};

/* } */
#endif
