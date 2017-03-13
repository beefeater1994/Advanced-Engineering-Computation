#import <Cocoa/Cocoa.h>
// #import AppKit?


// NSAutoReleasePool doesn't seem to exist if Auto Reference Count is on.
// It needs to be checked by __has_feature(objc_arc)
// Can't Apple just make NSAutoReleasePool a dummy class in that case?  
// Ahgh!  Objective-C is inferior to C++.
#ifndef __has_feature
#define __has_feature(x) 0
#endif


#include "fsguifiledialog_mac.h"
#include <string.h>

static struct FsGuiFileDialog_Mac_StringList *AddString(const char str[],struct FsGuiFileDialog_Mac_StringList *currentList)
{
	struct FsGuiFileDialog_Mac_StringList *newStr;
	newStr=(struct FsGuiFileDialog_Mac_StringList *)malloc(sizeof(struct FsGuiFileDialog_Mac_StringList));

	if(NULL!=newStr)
	{
		int len;
		len=strlen(str);

		newStr->fn=(char *)malloc(sizeof(char)*(len+1));
		if(NULL!=newStr->fn)
		{
			strcpy(newStr->fn,str);
			newStr->next=currentList;
			return newStr;
		}
		else
		{
			/* Something went wrong.  Don't change. */
			free(newStr);
			return currentList;
		}
	}
}

static void DeleteStringList(struct FsGuiFileDialog_Mac_StringList *list)
{
	struct FsGuiFileDialog_Mac_StringList *next;
	while(NULL!=list)
	{
		next=list->next;
		free(list->fn);
		free(list);
		list=next;
	}
}

void FsGuiFileDialog_Mac_InitDialog(struct FsGuiFileDialog_Mac *dlg)
{
	dlg->in_multiSelect=0;
	dlg->in_mode=FSGUIFILEDIALOG_MAC_MODE_OPEN;
	dlg->in_fileExtensionList=NULL;
	dlg->defaultPath="";
	dlg->defaultFile="";
	dlg->title="";

	dlg->out_result=0;
	dlg->out_selected=NULL;
}

void FsGuiFileDialog_Mac_AddExtension(struct FsGuiFileDialog_Mac *dlg,const char *ext)
{
	if(NULL!=ext)
	{
		dlg->in_fileExtensionList=AddString(ext,dlg->in_fileExtensionList);
	}
}

void FsGuiFileDialog_Mac_OpenDialog(struct FsGuiFileDialog_Mac *dlg)
{
#if !__has_feature(objc_arc)
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
#endif


	NSPanel *panel=(FSGUIFILEDIALOG_MAC_MODE_SAVE==dlg->in_mode ? [NSSavePanel savePanel] : [NSOpenPanel openPanel]);
	NSMutableArray *extArray=NULL;


	[panel setTitle:[NSString stringWithUTF8String:dlg->title]];
	[panel setTreatsFilePackagesAsDirectories:YES];

	if(FSGUIFILEDIALOG_MAC_MODE_SAVE==dlg->in_mode)
	{
		[panel setCanCreateDirectories:YES];
		// [panel setExtensionHidden:NO];  // How can I do it before OSX 10.9?
		[panel setCanSelectHiddenExtension:YES];
	}
	else
	{
		[panel setCanCreateDirectories:NO];
		if(0!=dlg->in_multiSelect)
		{
			[panel setAllowsMultipleSelection:YES];
		}
		else
		{
			[panel setAllowsMultipleSelection:NO];
		}
	}

	if(NULL!=dlg->in_fileExtensionList)
	{
		struct FsGuiFileDialog_Mac_StringList *ptr=dlg->in_fileExtensionList,*last=NULL;

		extArray=[[NSMutableArray alloc] init];

		while(NULL!=ptr)
		{
			const char *ext=ptr->fn;
			if('*'==ext[0])
			{
				++ext;
			}
			if('.'==ext[0])
			{
				++ext;
			}
			[extArray addObject:[[NSString alloc] initWithUTF8String:ext]];
			last=ptr;
			ptr=ptr->next;
		}

		[panel setAllowedFileTypes:extArray];
		[panel setAllowsOtherFileTypes:NO];
	}



	[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dlg->defaultPath]]];
	if(0!=dlg->defaultFile[0])
	{
		[panel setNameFieldStringValue:[NSString stringWithUTF8String:dlg->defaultFile]];
	}

	if(NSFileHandlingPanelOKButton==[panel runModal])
	{
		if(FSGUIFILEDIALOG_MAC_MODE_SAVE==dlg->in_mode)
		{
			printf("[Selected]\n");
			printf("%s\n",[[[panel URL] path] UTF8String]);

			dlg->out_selected=AddString([[[panel URL] path] UTF8String],dlg->out_selected);
			dlg->out_result=1;
		}
		else
		{
			NSArray *fileArray=[panel URLs];
			if(nil!=fileArray)
			{
				printf("[Selected]\n");
				int idx;
				for(idx=0; idx<[fileArray count]; ++idx)
				{
					NSURL *url=[fileArray objectAtIndex:idx];
					printf("%s\n",[[url path] UTF8String]);
					dlg->out_selected=AddString([[url path] UTF8String],dlg->out_selected);
				}
				dlg->out_result=1;
			}
		}
	}
	else
	{
		dlg->out_result=0;
	}

	[[NSApp mainWindow] makeKeyAndOrderFront:nil];

#if !__has_feature(objc_arc)
	[pool release];
#endif
}

void FsGuiFileDialog_Mac_CleanUp(struct FsGuiFileDialog_Mac *dlg)
{
	DeleteStringList(dlg->in_fileExtensionList);
	DeleteStringList(dlg->out_selected);
}


