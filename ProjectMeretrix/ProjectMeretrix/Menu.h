#pragma once
#include "raylib.h"

#define MAX_MENU_ITEMS 16
#define MENU_HEIGHT 28
#define MENU_ITEM_WIDTH 80
#define MENU_ITEM_HEIGHT 24

typedef struct {
	const char* label;
	int id;
} MenuItem;

typedef struct {
	const char* label;
	MenuItem items[MAX_MENU_ITEMS];
	int itemCount;
	int isOpen;
	int dropWidth;
	Rectangle bounds;
} Menu;

typedef struct {
	Menu menus[8];
	int menuCount;
	int activeMenu;
	Rectangle barRect;
} MenuBar;

void MenuBar_Init(MenuBar* mb, int screenWidth);
void MenuBar_Update(MenuBar* mb);
void MenuBar_Draw(MenuBar* mb);
int MenuBar_GetClickedItem(MenuBar* mb);