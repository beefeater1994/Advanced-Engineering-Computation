#include "ysshellext_constedgeslider.h"

YsShellExt_ConstEdgeSlider::State::State()
{
	edge[0]=nullptr;
	edge[1]=nullptr;
	onVertex=-1;
}



YSRESULT YsShellExt_ConstEdgeSlider::SlideTo(State &sta,const YsShellExt &shl,const YsVec3 &goal,const Condition &) const
{
	auto currentEdge=sta.edge;
	if(sta.edge[0]==sta.edge[1])
	{
		auto connVtHd=shl.GetConstEdgeConnectedVertex(sta.edge[0]);
		if(1==connVtHd.GetN() || 2==connVtHd.GetN())
		{
			currentEdge[1]=connVtHd[0];
		}
		else
		{
			return YSERR;
		}
	}

	auto lastEdge=sta.edge;
	for(;;)
	{
		decltype(sta.edge) nextEdge;
		nextEdge[0]=nullptr;
		nextEdge[1]=nullptr;

		const YsVec3 edVtPos[2]=
		{
			shl.GetVertexPosition(currentEdge[0]),
			shl.GetVertexPosition(currentEdge[1]),
		};

		auto nearPos=YsGetNearestPointOnLine3(edVtPos,goal);
		if(YSTRUE==YsCheckInBetween3(nearPos,edVtPos))
		{
			sta.edge=currentEdge;
			sta.pos=nearPos;
			sta.onVertex=-1;
			return YSOK;
		}
		else
		{
			int across,opp;
			if(0.0<(edVtPos[1]-edVtPos[0])*(nearPos-edVtPos[0]))
			{
				// Across currentEdge[1];
				across=1;
				opp=0;
			}
			else
			{
				// Across currentEdge[0];
				across=0;
				opp=1;
			}

			//   {connVtHd[0]}-----{currentEdge[across]}-----{connVtHd[1]}

			//       {oppVtHd}-----{currentEdge[across]}
			//                              OR
			//                     {currentEdge[across]}-----{oppVtHd}

			auto connVtHd=shl.GetConstEdgeConnectedVertex(currentEdge[across]);
			auto oppVtHd=currentEdge[opp];
			if(2==connVtHd.GetN())
			{
				nextEdge[0]=currentEdge[across];
				nextEdge[1]=YsPickOneOfAandBThatIsNotC(connVtHd[0],connVtHd[1],oppVtHd);

				if(YSTRUE==YsSameEdge<YsShell::VertexHandle>(lastEdge,nextEdge))  // Moving back & forth
				{
					sta.edge=currentEdge;
					sta.pos=shl.GetVertexPosition(currentEdge[across]);
					sta.onVertex=across;
					return YSOK;
				}
			}
			else  // Cannot go any farther.
			{
				sta.edge=currentEdge;
				sta.pos=shl.GetVertexPosition(currentEdge[across]);
				sta.onVertex=across;
				return YSOK;
			}
		}
		lastEdge=currentEdge;
		currentEdge=nextEdge;
	}
}

