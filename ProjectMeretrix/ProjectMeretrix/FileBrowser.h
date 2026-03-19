#pragma once
#include "raylib.h"

#define MAX_FILES 256
#define MAX_CHAR_PATH 512

typedef struct {
	char name[MAX_CHAR_PATH];
	char fullPath[MAX_CHAR_PATH];
	int isDirectory;
} FileEntry;

typedef struct {
	FileEntry files[MAX_FILES];
	int fileCount;
	char currentPath[MAX_CHAR_PATH];
	int selectedIndex;
	Rectangle panelRectangle;
} FileBrowser;

void FileBrowser_LoadDirectory(FileBrowser* fb, const char* path);
void FileBrowser_Init(FileBrowser* fb, const char* startPath, Rectangle rect);
void FileBrowser_Update(FileBrowser* fb);
void FileBrowser_Draw(FileBrowser* fb);