#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "FileBrowser.h"
#include "Menu.h"

int main() {
    InitWindow(1280, 720, "Meretrix Engine");
    SetTargetFPS(60);

    FileBrowser fb;
    MenuBar mb;

    FileBrowser_Init(&fb, "C:\\", (Rectangle) { 0, 520, 1280, 200 });
    MenuBar_Init(&mb, 1280);

    while (!WindowShouldClose()) {
        FileBrowser_Update(&fb);
        MenuBar_Update(&mb);

        BeginDrawing();
        ClearBackground(BLACK);
        FileBrowser_Draw(&fb);
        MenuBar_Draw(&mb);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}