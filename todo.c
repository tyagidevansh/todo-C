#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "styles/terminal/style_terminal.h" 
#include "styles/bluish/style_bluish.h"
#include "styles/amber/style_amber.h"
#include "styles/cherry/style_cherry.h"
#include "styles/lavanda/style_lavanda.h"
#include "styles/jungle/style_jungle.h"

#include <stdio.h>
#include <string.h>

#define NTASKS 100
#define TASKSIZE 1024

void saveTasks(const char tasks[][TASKSIZE], const bool checkboxes[], int taskCount) {
    FILE *file = fopen("tasks.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < taskCount; i++) {
            fprintf(file, "%d|%s\n", checkboxes[i], tasks[i]);
        }
        fclose(file);
    }
}

int loadTasks(char tasks[][TASKSIZE], bool checkboxes[]) {
    FILE *file = fopen("tasks.txt", "r");
    int taskCount = 0;
    if (file != NULL) {
        while (fscanf(file, "%d|%1023[^\n]", (int*)&checkboxes[taskCount], tasks[taskCount]) == 2) {
            taskCount++;
        }
        fclose(file);
    }
    return taskCount;
}

int main() {
    InitWindow(800, 600, "To-Do List");
    SetTargetFPS(60);

    GuiLoadStyleTerminal();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    int visualStyleActive = 1;
    int prevVisualStyleActive = 1;

    static char tasks[NTASKS][TASKSIZE] = {0};  
    static int taskCount = 0;          
    static char inputText[TASKSIZE] = {0};  
    static bool checkboxes[NTASKS] = {};

    Vector2 scrollOffset = {0, 0};
    Rectangle scrollPanelBounds = {10, 130, 780, 450};
    Rectangle contentBounds = {0, 0, 760, NTASKS * 40};
    Rectangle panelView = {0};

    taskCount = loadTasks(tasks, checkboxes);

    while (!WindowShouldClose()) {
        if (GuiButton((Rectangle){10, 10, 120, 30}, "Add Task") | IsKeyPressed(KEY_ENTER)) {
            if (taskCount < NTASKS && strlen(inputText) > 0 && strlen(inputText) < TASKSIZE) {
                strcpy(tasks[taskCount], inputText);
                taskCount++;
                memset(inputText, 0, sizeof(inputText));  
                checkboxes[taskCount] = false;
            }
        }

        bool anyChecked = false;
        for (int i = 0; i < taskCount; i++) {
            if (checkboxes[i]) {
                anyChecked = true;
                break;
            }
        }
        Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
        Color textColorDisabled = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_DISABLED));   
        Color bgColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));   

        ClearBackground(bgColor);

        BeginDrawing();

        GuiLabel((Rectangle){10, 50, 200, 30}, "Enter Task:");
        GuiTextBox((Rectangle){10, 90, 300, 30}, inputText, sizeof(inputText), true);
        
        GuiComboBox((Rectangle){150, 10, 120, 30}, "default;Terminal;Bluish;Amber;Cherry;Lavanda;Jungle", &visualStyleActive);

        if (visualStyleActive != prevVisualStyleActive) {
            GuiLoadStyleDefault();
            
            switch (visualStyleActive) {
                case 1: GuiLoadStyleTerminal(); break;
                case 2: GuiLoadStyleBluish(); break;
                case 3: GuiLoadStyleAmber(); break;
                case 4: GuiLoadStyleCherry(); break;
                case 5: GuiLoadStyleLavanda(); break;
                case 6: GuiLoadStyleJungle(); break;
            }

            prevVisualStyleActive = visualStyleActive;
        }

        if (anyChecked) {
            if (GuiButton((Rectangle){300, 10, 120, 30}, "Delete Task(s)")) {
                for (int i = 0; i < taskCount; i++) {
                    if (checkboxes[i]) {
                        for (int j = i; j < taskCount; j++) {
                            strcpy(tasks[j], tasks[j + 1]);
                            checkboxes[j] = checkboxes[j + 1];
                        }    
                        taskCount--;
                        i--;
                    }
                }
                anyChecked = false;
            }
        }

        GuiScrollPanel(scrollPanelBounds, NULL, contentBounds, &scrollOffset, &panelView);

        //scroll area
        BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);

            

        for (int i = 0; i < taskCount; i++) {
            GuiCheckBox((Rectangle){10, 137 + i * 40 + scrollOffset.y, 10, 10}, NULL, &checkboxes[i]);
            
            if (checkboxes[i]) {
                DrawText(tasks[i], 30, 130 + i * 40 + scrollOffset.y, 24, textColorDisabled);
            } else {
                DrawText(tasks[i], 30, 130 + i * 40 + scrollOffset.y, 24, textColor);
            }
        }
        
        //scroll area ends
        EndScissorMode();

        EndDrawing();
    }

    saveTasks(tasks, checkboxes, taskCount);

    CloseWindow();
    return 0;
}

//gcc -o todo todo.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11