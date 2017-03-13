#ifndef YSSHELLEXT_ALIGNMENTUTIL_IS_INCLUDED
#define YSSHELLEXT_ALIGNMENTUTIL_IS_INCLUDED
/* { */

#include <ysshellext.h>

class YsShellExt_AlignmentUtil
{
public:
	enum ALIGNTYPE
	{
		ALIGN_MINIMUM,
		ALIGN_CENTER,
		ALIGN_MAXIMUM
	};

private:
	double GetTargetValue(const YsShellExt &shl,int axis,ALIGNTYPE alignType) const;
	void GetMinMax(double &min,double &max,const YsShellExt &shl,int axis,YsShell::PolygonHandle plHd) const;
	void GetMinMax(double &min,double &max,const YsShellExt &shl,int axis,YsShellExt::ConstEdgeHandle ceHd) const;
	void GetMinMax(double &min,double &max,const YsShellExt &shl,int axis,YsShellExt::FaceGroupHandle fgHd) const;

	// T can be YsShell::PolygonHandle, YsShellExt::ConstEdgeHandle, or YsShellExt::FaceGroupHandle.
	template <class T>
	YsVec3 GetDisplacement(const YsShellExt &shl,T hd,int axis,ALIGNTYPE alignType,const double targetValue) const
	{
		double min,max;
		GetMinMax(min,max,shl,axis,hd);

		double d=0.0;
		switch(alignType)
		{
		case ALIGN_MINIMUM:
			d=targetValue-min;
			break;;
		case ALIGN_CENTER:
			d=targetValue-(min+max)/2.0;
			break;;
		case ALIGN_MAXIMUM:
			d=targetValue-max;
			break;;
		}

		auto v=YsVec3::Origin();
		v[axis]=d;
		return v;
	}

	YsArray <YsShell::VertexHandle> selVtHd;
	YsArray <YsShell::PolygonHandle> selPlHd;
	YsArray <YsShellExt::ConstEdgeHandle> selCeHd;
	YsArray <YsShellExt::FaceGroupHandle> selFgHd;

public:
	void CleanUp(void);

	template <class T>
	void AddVertex(const YsShellExt &,const T &vtHdStore)
	{
		for(auto vtHd : vtHdStore)
		{
			selVtHd.Add(vtHd);
		}
	}
	template <class T>
	void AddPolygon(const YsShellExt &,const T &plHdStore)
	{
		for(auto plHd : plHdStore)
		{
			selPlHd.Add(plHd);
		}
	}
	template <class T>
	void AddConstEdge(const YsShellExt &,const T &ceHdStore)
	{
		for(auto ceHd : ceHdStore)
		{
			selCeHd.Add(ceHd);
		}
	}
	template <class T>
	void AddFaceGroup(const YsShellExt &,const T &fgHdStore)
	{
		for(auto fgHd : fgHdStore)
		{
			selFgHd.Add(fgHd);
		}
	}

	template <class SHLCLASS>
	void Align(SHLCLASS &shl,int axis,ALIGNTYPE alignType)
	{
		typename SHLCLASS::StopIncUndo undoGuard(shl);

		auto targetValue=GetTargetValue(shl.Conv(),axis,alignType);

		for(auto vtHd : selVtHd)
		{
			auto vtPos=shl.GetVertexPosition(vtHd);
			vtPos[axis]=targetValue;
			shl.SetVertexPosition(vtHd,vtPos);
		}
		for(auto plHd : selPlHd)
		{
			auto vec=GetDisplacement(shl.Conv(),plHd,axis,alignType,targetValue);
			for(auto vtHd : shl.GetPolygonVertex(plHd))
			{
				auto vtPos=shl.GetVertexPosition(vtHd)+vec;
				shl.SetVertexPosition(vtHd,vtPos);
			}
		}
		for(auto ceHd : selCeHd)
		{
			auto vec=GetDisplacement(shl.Conv(),ceHd,axis,alignType,targetValue);
			for(auto vtHd : shl.GetConstEdgeVertex(ceHd))
			{
				auto vtPos=shl.GetVertexPosition(vtHd)+vec;
				shl.SetVertexPosition(vtHd,vtPos);
			}
		}
		for(auto fgHd : selFgHd)
		{
			auto vec=GetDisplacement(shl.Conv(),fgHd,axis,alignType,targetValue);
			for(auto plHd : shl.GetFaceGroup(fgHd))
			{
				for(auto vtHd : shl.GetPolygonVertex(plHd))
				{
					auto vtPos=shl.GetVertexPosition(vtHd)+vec;
					shl.SetVertexPosition(vtHd,vtPos);
				}
			}
		}
	}
};

/* } */
#endif
