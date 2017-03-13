/* ////////////////////////////////////////////////////////////

File Name: ysshellextedit_triangulationutil.cpp
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include "ysshellext_boundaryutil.h"
#include "ysshellextedit_triangulationutil.h"

void YsShellExtEdit_ApplyTriangulation(YsShellExtEdit &shl,const YsShellExt_TriangulationInfo &triangulator)
{
	YsShellExtEdit::StopIncUndo incUndo(shl);

	for(auto &tri : triangulator.tri)
	{
		if(0<tri.triVtHd.GetN())
		{
			auto plHd=tri.plHd;
			auto plVtHd=shl.GetPolygonVertex(plHd);
			if(3==plVtHd.GetN())
			{
				continue;
			}

			auto fgHd=shl.FindFaceGroupFromPolygon(plHd);

			{
				YsArray <YsArray <YsShell::VertexHandle,3>,4> triVtHd;
				for(YSSIZE_T idx=0; idx<=tri.triVtHd.GetN()-3; idx+=3)
				{
					triVtHd.Increment();
					triVtHd.Last().Add(tri.triVtHd[idx]);
					triVtHd.Last().Add(tri.triVtHd[idx+1]);
					triVtHd.Last().Add(tri.triVtHd[idx+2]);
				}
				YsShellExt_BoundaryInfo boundaryUtil;
				boundaryUtil.MakeInfoFromVertexSequenceArray(shl.Conv(),triVtHd);

				YSRESULT res=YSOK;
				for(auto edIdx : plVtHd.AllIndex())
				{
					if(YSTRUE!=boundaryUtil.IsBoundaryEdge(shl.Conv(),plVtHd[edIdx],plVtHd.GetCyclic(edIdx+1)))
					{
						res=YSERR;
						break;
					}
				}
				if(YSOK!=res)
				{
					printf("%s %d\n",__FUNCTION__,__LINE__);
					printf("  Edge not recovered.\n");
					continue;
				}
			}

			for(YSSIZE_T idx=0; idx<=tri.triVtHd.GetN()-3; idx+=3)
			{
				const YsShellVertexHandle triVtHd[3]=
				{
					tri.triVtHd[idx],
					tri.triVtHd[idx+1],
					tri.triVtHd[idx+2],
				};
				if(0==idx)
				{
					shl.SetPolygonVertex(plHd,3,triVtHd);
				}
				else
				{
					auto newPlHd=shl.AddPolygon(3,triVtHd);

					auto attrib=shl.GetPolygonAttrib(plHd);
					shl.SetPolygonAttrib(newPlHd,*attrib);

					auto nom=shl.GetNormal(plHd);
					auto col=shl.GetColor(plHd);

					shl.SetPolygonNormal(newPlHd,nom);
					shl.SetPolygonColor(newPlHd,col);

					shl.AddFaceGroupPolygon(fgHd,1,&newPlHd);
				}
			}
		}
	}
}

