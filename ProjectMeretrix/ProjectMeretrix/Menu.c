#define _CRT_SECURE_NO_WARNINGS
#include "Menu.h"
#include "raylib.h"

static const Color MENUBAR_COLOR = { 45, 45, 45, 255 };
static const Color MENUITEM_COLOR = { 60, 60, 60, 255 };
static const Color MENUHOVER_COLOR = { 80, 80, 80, 255 };
static const Color MENUDROP_COLOR = { 50, 50, 50, 255 };
static const Color MENUTEXT_COLOR = { 220, 220, 220, 255 };

void MenuBar_Init(MenuBar* mb, int screenWidth)
{
    mb->activeMenu = -1;
    mb->menuCount = 0;
    mb->barRect = (Rectangle){ 0, 0, screenWidth, MENU_HEIGHT };

    mb->menus[0].label = "File";
    mb->menus[0].isOpen = 0;
    mb->menus[0].items[0] = (MenuItem){ "New Scene", 1 };
    mb->menus[0].items[1] = (MenuItem){ "Open Scene", 2 };
    mb->menus[0].items[2] = (MenuItem){ "Save", 3 };
    mb->menus[0].items[3] = (MenuItem){ "Exit", 4 };
    mb->menus[0].itemCount = 4;
    mb->menus[0].bounds = (Rectangle){ 0, 0, MENU_ITEM_WIDTH, MENU_HEIGHT };
    mb->menuCount++;

    mb->menus[1].label = "Settings";
    mb->menus[1].isOpen = 0;
    mb->menus[1].items[0] = (MenuItem){ "Preferences", 5 };
    mb->menus[1].items[1] = (MenuItem){ "Theme", 6 };
    mb->menus[1].itemCount = 2;
    mb->menus[1].bounds = (Rectangle){ MENU_ITEM_WIDTH, 0, MENU_ITEM_WIDTH, MENU_HEIGHT };
    mb->menuCount++;

    for (int i = 0; i < mb->menuCount; i++)
    {
        int maxWidth = 0;
        for (int j = 0; j < mb->menus[i].itemCount; j++)
        {
            int w = MeasureText(mb->menus[i].items[j].label, 13);
            if (w > maxWidth) maxWidth = w;
        }
        mb->menus[i].dropWidth = maxWidth + 16;
    }
}

void MenuBar_Update(MenuBar* mb)
{
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        int clickedMenu = -1;

        for (int i = 0; i < mb->menuCount; i++)
        {
            if (CheckCollisionPointRec(mouse, mb->menus[i].bounds))
            {
                clickedMenu = i;
                break;
            }
        }

        if (clickedMenu != -1)
        {
            if (mb->activeMenu == clickedMenu)
                mb->activeMenu = -1;
            else
                mb->activeMenu = clickedMenu;
        }
        else
        {
            int clickedItem = MenuBar_GetClickedItem(mb);
            if (clickedItem == 4)
                CloseWindow();
            mb->activeMenu = -1;
        }
    }
}

int MenuBar_GetClickedItem(MenuBar* mb)
{
    if (mb->activeMenu == -1) return -1;

    Vector2 mouse = GetMousePosition();
    Menu* menu = &mb->menus[mb->activeMenu];

    for (int i = 0; i < menu->itemCount; i++)
    {
        Rectangle itemRect = { menu->bounds.x, MENU_HEIGHT + i * MENU_ITEM_HEIGHT, menu->dropWidth, MENU_ITEM_HEIGHT };
        if (CheckCollisionPointRec(mouse, itemRect)); return menu->items[i].id;
    }
    return -1;
}

void MenuBar_Draw(MenuBar* mb)
{
    DrawRectangleRec(mb->barRect, MENUBAR_COLOR);

    for (int i = 0; i < mb->menuCount; i++)
    {
        if (mb->activeMenu == i)
        {
            DrawRectangleRec(mb->menus[i].bounds, MENUHOVER_COLOR);
        }
        DrawText(mb->menus[i].label, mb->menus[i].bounds.x + 8, mb->menus[i].bounds.y + 6, 14, MENUTEXT_COLOR);
    }

    if (mb->activeMenu != -1)
    {
        Menu* menu = &mb->menus[mb->activeMenu];

        for (int i = 0; i < menu->itemCount; i++)
        {
            Rectangle itemRect = { menu->bounds.x, MENU_HEIGHT + i * MENU_ITEM_HEIGHT, menu->dropWidth, MENU_ITEM_HEIGHT };

            if (CheckCollisionPointRec(GetMousePosition(), itemRect))
            {
                DrawRectangleRec(itemRect, MENUHOVER_COLOR);
            }
            else
            {
                DrawRectangleRec(itemRect, MENUDROP_COLOR);
            }
            DrawText(menu->items[i].label, itemRect.x + 8, itemRect.y + 4, 13, MENUTEXT_COLOR);
        }
    }
    DrawRectangleLinesEx(mb->barRect, 1, (Color) { 60, 60, 60, 255 });
}