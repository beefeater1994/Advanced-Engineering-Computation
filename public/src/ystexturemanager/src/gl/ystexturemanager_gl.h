#ifndef YSTEXTUREMANAGERGL_IS_INCLUDED
#define YSTEXTUREMANAGERGL_IS_INCLUDED
/* { */

#include <ystexturemanager.h>
#include <ysgl.h>

class YsTextureManager::ActualTexture
{
public:
	GLuint texId;
	GLuint frameBufferId;
};

/* } */
#endif
