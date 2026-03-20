#define _CRT_SECURE_NO_WARNINGS
#include "FileBrowser.h"
#include <string.h>
#include <stdio.h>
#include "raylib.h"

#define NOGDI
#define NOUSER
#include <windows.h>

#define ICON_SIZE 64
#define ICON_PADDING 16
#define LABEL_HEIGHT 20

static const Color PANEL_COLOR = { 30, 30, 30, 255 };
static const Color SELECT_COLOR = { 60, 100, 160, 255 };
static const Color FOLDER_COLOR = { 220, 180, 80, 255 };
static const Color FILE_COLOR = { 200, 200, 200, 255 };
static const Color HEADER_COLOR = { 20, 20, 20, 255 };

void FileBrowser_LoadDirectory(FileBrowser* fb, const char* path)
{
	fb->scrollOffset = 0;
	fb->selectedIndex = -1;
	fb->fileCount = 0;
	strncpy(fb->currentPath, path, MAX_CHAR_PATH);

	WIN32_FIND_DATAA findData;
	char searchPath[MAX_CHAR_PATH];
	if (path[strlen(path) - 1] == '\\')
		snprintf(searchPath, MAX_CHAR_PATH, "%s*", path);
	else
		snprintf(searchPath, MAX_CHAR_PATH, "%s\\*", path);

	HANDLE hFind = FindFirstFileA(searchPath, &findData);
	if (hFind == INVALID_HANDLE_VALUE) return;

	do {
		if (strcmp(findData.cFileName, ".") == 0) continue;
		if (strcmp(findData.cFileName, "..") == 0) continue;
		if (fb->fileCount >= MAX_FILES) break;

		FileEntry* entry = &fb->files[fb->fileCount++];
		strncpy(entry->name, findData.cFileName, MAX_CHAR_PATH);
		snprintf(entry->fullPath, MAX_CHAR_PATH, "%s\\%s", path, findData.cFileName);
		entry->isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	} while (FindNextFileA(hFind, &findData));

	FindClose(hFind);
}

void FileBrowser_Init(FileBrowser* fb, const char* startPath, Rectangle rect)
{
	fb->scrollOffset = 0;
	fb->selectedIndex = -1;
	fb->panelRectangle = rect;
	FileBrowser_LoadDirectory(fb, startPath);
}

void FileBrowser_Update(FileBrowser* fb)
{
	Rectangle backBtn = { fb->panelRectangle.x + fb->panelRectangle.width - 60,
					 fb->panelRectangle.y + 4, 54, 20 };
	if (CheckCollisionPointRec(GetMousePosition(), fb->panelRectangle))
	{
		float wheel = GetMouseWheelMove();
		if (wheel != 0)
		{
			fb->scrollOffset -= wheel * 20;
			if (fb->scrollOffset < 0) fb->scrollOffset = 0;
		}
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mouse = GetMousePosition();
		if (CheckCollisionPointRec(mouse, fb->panelRectangle))
		{
			int col = (int)(mouse.x - fb->panelRectangle.x - ICON_PADDING) / (ICON_SIZE + ICON_PADDING);
			int row = (int)(mouse.y - fb->panelRectangle.y - 28 + fb->scrollOffset) / (ICON_SIZE + ICON_PADDING + LABEL_HEIGHT);
			int cols = (int)(fb->panelRectangle.width) / (ICON_SIZE + ICON_PADDING);
			int index = row * cols + col;
			if (index >= 0 && index < fb->fileCount)
			{
				if (fb->selectedIndex == index && fb->files[index].isDirectory)
				{
					char newPath[MAX_CHAR_PATH];
					if (fb->currentPath[strlen(fb->currentPath) - 1] == '\\')
						snprintf(newPath, MAX_CHAR_PATH, "%s%s", fb->currentPath, fb->files[index].name);
					else
						snprintf(newPath, MAX_CHAR_PATH, "%s\\%s", fb->currentPath, fb->files[index].name);
					FileBrowser_LoadDirectory(fb, newPath);
					fb->selectedIndex = -1;
				}
				else
				{
					fb->selectedIndex = index;
				}
			}
		}
		if (CheckCollisionPointRec(mouse, backBtn))
		{
			char parentPath[MAX_CHAR_PATH];
			strncpy(parentPath, fb->currentPath, MAX_CHAR_PATH);

			char* lastSlash = strrchr(parentPath, '\\');
			if (lastSlash && lastSlash != parentPath)
			{
				*lastSlash = '\0';
				if (strlen(parentPath) == 2 && parentPath[1] == ':')
					strcat(parentPath, "\\");
				FileBrowser_LoadDirectory(fb, parentPath);
				fb->selectedIndex = -1;
			}
		}
	}
}

void FileBrowser_Draw(FileBrowser* fb)
{
	DrawRectangleRec(fb->panelRectangle, PANEL_COLOR);
	DrawRectangle(fb->panelRectangle.x, fb->panelRectangle.y, fb->panelRectangle.width, 28, HEADER_COLOR);
	DrawText("Project", fb->panelRectangle.x + 8, fb->panelRectangle.y + 6, 16, WHITE);
	DrawText(fb->currentPath, fb->panelRectangle.x + 80, fb->panelRectangle.y + 8, 12, GRAY);

	Rectangle backBtn = { fb->panelRectangle.x + fb->panelRectangle.width - 60,
						fb->panelRectangle.y + 4, 54, 20 };
	DrawRectangleRec(backBtn, (Color) { 60, 60, 60, 255 });
	DrawText("< Back", backBtn.x + 4, backBtn.y + 4, 12, WHITE);

	int cols = (int)(fb->panelRectangle.width) / (ICON_SIZE + ICON_PADDING);

	BeginScissorMode(fb->panelRectangle.x, fb->panelRectangle.y +28, fb->panelRectangle.width, fb->panelRectangle.height -28);
	for (int i = 0; i < fb->fileCount; i++) {
		int col = i % cols;
		int row = i / cols;

		float x = fb->panelRectangle.x + col * (ICON_SIZE + ICON_PADDING) + ICON_PADDING;
		float y = fb->panelRectangle.y + 32 + row * (ICON_SIZE + ICON_PADDING + LABEL_HEIGHT) - fb->scrollOffset;

		if (i == fb->selectedIndex) {
			DrawRectangle(x - 4, y - 4, ICON_SIZE + 8,
				ICON_SIZE + LABEL_HEIGHT + 8, SELECT_COLOR);
		}

		Color iconColor = fb->files[i].isDirectory ? FOLDER_COLOR : FILE_COLOR;
		DrawRectangle(x, y, ICON_SIZE, ICON_SIZE, iconColor);

		char displayName[MAX_CHAR_PATH];
		int textWidth = MeasureText(fb->files[i].name, 10);
		if (textWidth > ICON_SIZE)
		{
			strncpy(displayName, fb->files[i].name, MAX_CHAR_PATH);
			int len = strlen(displayName);
			while (len > 0 && MeasureText(displayName, 10) + MeasureText("...", 10) > ICON_SIZE)
			{
				displayName[--len] = '\0';
			}
			strcat(displayName, "...");
		}
		else
		{
			strncpy(displayName, fb->files[i].name, MAX_CHAR_PATH);
		}
		int finalWidth = MeasureText(displayName, 10);
		int textX = x + (ICON_SIZE - finalWidth) / 2;
		DrawText(displayName, textX, y + ICON_SIZE + 2, 10, WHITE);
	}
	EndScissorMode();
	DrawRectangleLinesEx(fb->panelRectangle, 1, (Color) { 60, 60, 60, 255 });
}