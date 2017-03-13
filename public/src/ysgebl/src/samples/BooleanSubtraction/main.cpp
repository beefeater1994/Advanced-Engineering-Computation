#include <stdio.h>
#include <ysport.h>
#include <ysshellext.h>
#include <ysshellextio.h>
#include <ysshellextedit_boolutil.h>

int main(int argc,char *argv[])
{
	if(4<=argc)
	{
		YsShellExtEdit shl[2];

		YsFileIO::File ifp0(argv[1],"r");
		if(nullptr!=ifp0)
		{
			auto inStream=ifp0.InStream();
			shl[0].LoadSrf(inStream);
		}
		else
		{
			fprintf(stderr,"Cannot open input file.\n");
			return 1;
		}
		YsFileIO::File ifp1(argv[2],"r");
		if(nullptr!=ifp1)
		{
			auto inStream=ifp1.InStream();
			YsShellExtReader reader;
			shl[1].LoadSrf(inStream);
		}
		else
		{
			fprintf(stderr,"Cannot open input file.\n");
			return 1;
		}


		shl[0].EnableSearch();
		shl[1].EnableSearch();
		shl[0].DisableEditLog();
		shl[1].DisableEditLog();


		YsShellExtEdit_BooleanUtil boolUtil;
		boolUtil.SetShellAndOpType(shl[0],shl[1],YSBOOLMINUS);
		boolUtil.Run();

		boolUtil.MakeVertexMapping(0);  // Also tentatively mark vertices from the other shell as 'used'.
		boolUtil.CacheUnusedPolygon(0);
		boolUtil.TransferRemainingPartOfConstEdge(0);
		boolUtil.ReconstructConstEdge(0);
		boolUtil.CombineConstEdgeOfSameOrigin(0);
		boolUtil.ConstrainBetweenInputShell(0);
		boolUtil.DeleteTemporaryVertex(0);
		boolUtil.MergePolygonFromSameOriginLeavingOneShortestEdgePerVertex(0);
		boolUtil.AddPolygonFromTheOtherShell(0);
		boolUtil.ModifyShell(0);
		boolUtil.ApplyConstEdgeChange(0);
		boolUtil.DeleteCachedUnusedPolygon(0);
		boolUtil.DeleteUsedVertexTurnedUnused(0);


		YsFileIO::File ofp(argv[3],"w");
		if(nullptr!=ofp)
		{
			auto outStream=ofp.OutStream();
			YsShellExtWriter writer;
			writer.SaveSrf(outStream,shl[0].Conv());
		}
		else
		{
			fprintf(stderr,"Cannot open file for output.\n");
		}
	}
	return 0;
}
