#ifndef YSSHELLEXT_CONSTEDGESLIDER_IS_INCLUDED
#define YSSHELLEXT_CONSTEDGESLIDER_IS_INCLUDED
/* { */

#include <ysshellext.h>


class YsShellExt_ConstEdgeSlider
{
public:
	class State
	{
	public:
		YsShell::Edge edge;
		YsVec3 pos;
		// onVertex    -1: between edge[0] and edge[1]
		//              0: at edge[0]
		//              1: at edge[1]
		int onVertex;
		State();
	};

	class Condition
	{
	public:
	};

	YSRESULT SlideTo(State &sta,const YsShellExt &shl,const YsVec3 &goal,const Condition &) const;
};


/* } */
#endif
