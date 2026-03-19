#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "FileBrowser.h"

int main() {
    InitWindow(1280, 720, "Meretrix Engine");
    SetTargetFPS(60);

    FileBrowser fb;
    FileBrowser_Init(&fb, "C:\\", (Rectangle) { 0, 520, 1280, 200 });

    while (!WindowShouldClose()) {
        FileBrowser_Update(&fb);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Meretrix Engine", 10, 10, 20, WHITE);
        FileBrowser_Draw(&fb);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}