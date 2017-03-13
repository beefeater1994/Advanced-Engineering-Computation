#ifndef YSGLCPP_IS_INCLUDED
#define YSGLCPP_IS_INCLUDED
/* { */

class YsGL
{
public:
	enum PRIMITIVE_TYPE
	{
		POINTS,
		LINES,
		LINE_STRIP,
		LINE_LOOP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
	};
};

#include "ysglbuffer.h"
#include "ysglbuffer_sphereutil.h"
#include "ysglbuffermanager.h"
#include "ysglcascadedshadowmap.h"
#include "ysglparticlemanager.h"

/* } */
#endif
