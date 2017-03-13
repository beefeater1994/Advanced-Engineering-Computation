#ifndef YSVISUAL_IS_INCLUDED
#define YSVISUAL_IS_INCLUDED
/* { */

#include <memory>
#include <cstddef>

#include "ysshellextgl.h"
#include <ysshelldnmtemplate.h>



class YsVisual
{
public:
	enum
	{
		DRAWOPAQUE=1,
		DRAWTRANSPARENT=2,
		DRAWALL=3
	};

	enum SPECIAL_RENDERING_REQUIREMENT
	{
		RENDER_NORMAL,
		RENDER_AS_VASI,  // Visual Approach Slope Indicator
		RENDER_AS_PAPI,  // Precision Approach Path Indicator
	};
	SPECIAL_RENDERING_REQUIREMENT specialRenderingRequirment;
	union
	{
		double vasiAngle;
		double papiAngle;
	};

	void SetPapiColor(const double &angle);
	void SetVasiColor(const double &angle);

// Warning!
// Now CPU has total responsibility in calculating the model-view transformation.
// (Because most of the GPUs has such a ****ing low precision.  Give me double!)
// Therefore, this pos and att is relative to the origin.  Not relative to the camera!
	virtual void Draw(const YsVec3 &pos,const YsAtt3 &att) const=0;
	virtual void Draw(const YsMatrix4x4 &modelView,unsigned int drawFlag) const=0;
	virtual void Draw(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &pos,const YsAtt3 &att,unsigned int drawFlag) const=0;
	virtual void DrawShadow(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &pos,const YsAtt3 &att,const YsMatrix4x4 &projPlnTfm) const=0;
};


class YsVisualSrf : public YsShellExtWithVbo <YsShellExt>, public YsVisual
{
public:
	using YsShell::SetMatrix;
	using YsShell::ShootRayH;
	using YsShell::SetTrustPolygonNormal;

	inline YsVisualSrf(){}
	YsVisualSrf(const YsVisualSrf &incoming);
	YsVisualSrf &operator=(const YsVisualSrf &incoming);

	YSRESULT Load(YsTextInputStream &inStream);

	void PrepareVboIfNotDoneYet(void);

// Warning!
// Now CPU has total responsibility in calculating the model-view transformation.
// (Because most of the GPUs has such a ****ing low precision.  Give me double!)
// Therefore, this pos and att is relative to the origin.  Not relative to the camera!
	void Draw(const YsVec3 &pos,const YsAtt3 &att) const;
	void Draw(const YsMatrix4x4 &modelView,unsigned int drawFlag) const;

	void Draw(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &pos,const YsAtt3 &att,unsigned int drawFlag) const;
	void DrawShadow(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &pos,const YsAtt3 &att,const YsMatrix4x4 &projPlnTfm) const;
};



class YsVisualDnm : public YsVisual
{
protected:
	typedef YsShellExtWithVbo <YsShellExt> Shell;
	typedef YsShellDnmContainer<Shell> Dnm;
	typedef YsShellDnmContainer<Shell>::DnmState DnmState;
	std::shared_ptr <Dnm> dnmPtr;
	DnmState dnmState;

public:
	YsVisualDnm();
	YsVisualDnm(std::nullptr_t);
	~YsVisualDnm();

	void CacheTransformation(void);
	void CleanUp(void);

	YsVisualDnm &operator=(std::nullptr_t)
	{
		dnmPtr=nullptr;
		return *this;
	}
	bool operator==(std::nullptr_t) const
	{
		return (nullptr==dnmPtr);
	}
	bool operator!=(std::nullptr_t) const
	{
		return (nullptr!=dnmPtr);
	}

	YSRESULT Load(YsTextInputStream &inStream,const char fn[]);

	void GetBoundingBox(YsVec3 bbx[2]) const;
	void GetBoundingBox(YsVec3 &bbx0,YsVec3 &bbx1) const;

	void SetStateOfAllPart(int sta);
	void SetState(int partId,int sta);
	void SetState(int partId,int st1,int st2,const double &t);
	void SetShow(int partId,YSBOOL show);
	void SetAngle(int partId,const double &angle);
	void SetHeading(int partId,const double &angle);
	void SetPitch(int partId,const double &angle);

	void SetAngleByDnmNodeNumber(int nodeNumber,const double &hdg,const double &pch);

	/*! This function changes color for PAPI and VASI.  Call before Draw function.
	*/
	void SetUpSpecialRenderingRequirement(void);

// Warning!
// Now CPU has total responsibility in calculating the model-view transformation.
// (Because most of the GPUs has such a ****ing low precision.  Give me double!)
// Therefore, this pos and att is relative to the origin.  Not relative to the camera!
	void Draw(const YsVec3 &pos,const YsAtt3 &att) const;
	void Draw(const YsMatrix4x4 &modelView,unsigned int drawFlag) const;
private:
	void Draw(const YsMatrix4x4 &tfm,Dnm::Node *nodePtr,unsigned int drawFlag) const;

public:
	void Draw(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &pos,const YsAtt3 &att,unsigned int drawFlag) const;
private:
	void Draw(
	    const YsMatrix4x4 &curTfm,Dnm::Node *nodePtr,
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    unsigned int drawFlag) const;

public:
	void DrawShadow(
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm,
	    const YsVec3 &pos,const YsAtt3 &att,const YsMatrix4x4 &projPlnTfm) const;
private:
	void DrawShadow(
	    const YsMatrix4x4 &curTfm,Dnm::Node *nodePtr,
	    const YsMatrix4x4 &viewTfm,const YsMatrix4x4 &projTfm) const;

public:
	int GetDnmNodeNumberFromName(const char dnmName[]) const;
};

inline bool operator==(std::nullptr_t,const YsVisualDnm &vis)
{
	return (vis==nullptr);
}
inline bool operator!=(std::nullptr_t,const YsVisualDnm &vis)
{
	return (vis!=nullptr);
}

////////////////////////////////////////////////////////////

/* } */
#endif
