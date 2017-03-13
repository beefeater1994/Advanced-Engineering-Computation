#include <ysshellext_alignmentutil.h>

#include "../ysgebl_gui_editor_base.h"
#include "../textresource.h"



void GeblGuiEditorBase::Edit_Align_MinimumX(FsGuiPopUpMenuItem *)
{
	Edit_Align(0,YsShellExt_AlignmentUtil::ALIGN_MINIMUM);
}
void GeblGuiEditorBase::Edit_Align_CenterX(FsGuiPopUpMenuItem *)
{
	Edit_Align(0,YsShellExt_AlignmentUtil::ALIGN_CENTER);
}
void GeblGuiEditorBase::Edit_Align_MaximumX(FsGuiPopUpMenuItem *)
{
	Edit_Align(0,YsShellExt_AlignmentUtil::ALIGN_MAXIMUM);
}
void GeblGuiEditorBase::Edit_Align_MinimumY(FsGuiPopUpMenuItem *)
{
	Edit_Align(1,YsShellExt_AlignmentUtil::ALIGN_MINIMUM);
}
void GeblGuiEditorBase::Edit_Align_CenterY(FsGuiPopUpMenuItem *)
{
	Edit_Align(1,YsShellExt_AlignmentUtil::ALIGN_CENTER);
}
void GeblGuiEditorBase::Edit_Align_MaximumY(FsGuiPopUpMenuItem *)
{
	Edit_Align(1,YsShellExt_AlignmentUtil::ALIGN_MAXIMUM);
}
void GeblGuiEditorBase::Edit_Align_MinimumZ(FsGuiPopUpMenuItem *)
{
	Edit_Align(2,YsShellExt_AlignmentUtil::ALIGN_MINIMUM);
}
void GeblGuiEditorBase::Edit_Align_CenterZ(FsGuiPopUpMenuItem *)
{
	Edit_Align(2,YsShellExt_AlignmentUtil::ALIGN_CENTER);
}
void GeblGuiEditorBase::Edit_Align_MaximumZ(FsGuiPopUpMenuItem *)
{
	Edit_Align(2,YsShellExt_AlignmentUtil::ALIGN_MAXIMUM);
}

void GeblGuiEditorBase::Edit_Align(int axis,YsShellExt_AlignmentUtil::ALIGNTYPE alignType)
{
	if(nullptr!=Slhd())
	{
		auto &shl=*Slhd();
		YsShellExt_AlignmentUtil alignUtil;
		alignUtil.AddVertex(shl.Conv(),shl.GetSelectedVertex());
		alignUtil.AddPolygon(shl.Conv(),shl.GetSelectedPolygon());
		alignUtil.AddConstEdge(shl.Conv(),shl.GetSelectedConstEdge());
		alignUtil.AddFaceGroup(shl.Conv(),shl.GetSelectedFaceGroup());
		alignUtil.Align(shl,axis,alignType);

		needRemakeDrawingBuffer|=NEED_REMAKE_DRAWING_ALL;
		SetNeedRedraw(YSTRUE);
	}
}
