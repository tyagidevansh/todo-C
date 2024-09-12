#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "styles/terminal/style_terminal.h" 
#include "styles/bluish/style_bluish.h"

#define NTASKS 100
#define TASKSIZE 1024

int main(void) {
    InitWindow(800, 600, "To-Do List");
    SetTargetFPS(60);

    Font font = GetFontDefault();

    GuiLoadStyleTerminal();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    int visualStyleActive = 0;
    int prevVisualStyleActive = 1;

    static char tasks[NTASKS][TASKSIZE] = {0};  
    static int taskCount = 0;          
    static char inputText[TASKSIZE] = {0};  
    static bool checkboxes[NTASKS] = {};

    Vector2 scrollOffset = {0, 0};
    Rectangle scrollPanelBounds = {10, 130, 780, 450};
    Rectangle contentBounds = {0, 0, 760, NTASKS * 40};
    Rectangle panelView = {0};

    while (!WindowShouldClose()) {
        if (GuiButton((Rectangle){10, 10, 120, 30}, "Add Task") | IsKeyPressed(KEY_ENTER)) {
            if (taskCount < NTASKS && strlen(inputText) > 0 && strlen(inputText) < TASKSIZE) {
                strcpy(tasks[taskCount], inputText);
                taskCount++;
                memset(inputText, 0, sizeof(inputText));  
                checkboxes[taskCount] = false;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        GuiLabel((Rectangle){10, 50, 200, 30}, "Enter Task:");
        GuiTextBox((Rectangle){10, 90, 300, 30}, inputText, sizeof(inputText), true);

        GuiScrollPanel(scrollPanelBounds, NULL, contentBounds, &scrollOffset, &panelView);
        BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);

        for (int i = 0; i < taskCount; i++) {
            GuiCheckBox((Rectangle){10, 137 + i * 40 + scrollOffset.y, 10, 10}, NULL, &checkboxes[i]);
            
            if (checkboxes[i]) {
                DrawText(tasks[i], 30, 130 + i * 40 + scrollOffset.y, 24, GRAY);
            } else {
                DrawText(tasks[i], 30, 130 + i * 40 + scrollOffset.y, 24, GREEN);
            }
        }

        EndScissorMode();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

//gcc -o todo todo.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11