#include <stdio.h>
#include <ysport.h>
#include <ysshellext.h>
#include <ysshellextio.h>

int main(int argc,char *argv[])
{
	if(3<=argc)
	{
		YsShellExt shl;

		YsFileIO::File ifp(argv[1],"r");
		if(nullptr!=ifp)
		{
			auto inStream=ifp.InStream();
			YsShellExtReader reader;
			reader.MergeSrf(shl,inStream);
		}
		else
		{
			fprintf(stderr,"Cannot open input file.\n");
			return 1;
		}

		shl.EnableSearch();
		for(auto ceHd : shl.AllConstEdge())
		{
			shl.DeleteConstEdge(ceHd);
		}
		YsShellEdgeEnumHandle edHd=nullptr;
		while(YSOK==shl.MoveToNextEdge(edHd))
		{
			auto edge=shl.GetEdge(edHd);
			if(2!=shl.GetNumPolygonUsingEdge(edge))
			{
				shl.AddConstEdge(2,edge,YSFALSE);
			}
		}

		for(auto plHd : shl.AllPolygon())
		{
			shl.ForceDeletePolygon(plHd);
		}

		YsFileIO::File ofp(argv[2],"w");
		if(nullptr!=ofp)
		{
			auto outStream=ofp.OutStream();
			YsShellExtWriter writer;
			writer.SaveSrf(outStream,shl);
		}
		else
		{
			fprintf(stderr,"Cannot open file for output.\n");
		}
	}
	return 0;
}
