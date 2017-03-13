#include "ystexturemanager_nownd.h"

YSRESULT YsTextureManager::Unit::Bind(int) const
{
	return YSERR;
}

YSRESULT YsTextureManager::Unit::BindFrameBuffer(void) const
{
	return YSERR;
}


/* static */ YsTextureManager::ActualTexture *YsTextureManager::Alloc(void)
{
	return nullptr;
}

/* static */ void YsTextureManager::Delete(YsTextureManager::ActualTexture *texPtr)
{
}

/* static */ YSRESULT YsTextureManager::CreateDepthMapRenderTarget(ActualTexture *texPtr,int wid,int hei)
{
	return YSOK;
}

/* static */ YSRESULT YsTextureManager::TransferRGBABitmap(YsTextureManager::ActualTexture *texPtr,int wid,int hei,const unsigned char rgba[])
{
	return YSOK;
}

/* static */ YSBOOL YsTextureManager::IsTextureReady(YsTextureManager::ActualTexture *texPtr)
{
	return YSTRUE;
}

