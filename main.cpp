#include "raylib.h"
#include <vector>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int screenWidth = 800;
int screenHeight = 450;

struct bloco {
  int vida;
  int x;
  int y;
  int w;
  int h;
  bool is_wall;
};

struct Placa{
  int x;
  int y;
  int w;
  int h;
  int velocity;
};

std::vector<bloco> blocos;

Placa placa = {300, screenHeight-30, 80, 20, 4};

int vx = 4;
int vy = 4;


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    

    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) placa.x += placa.velocity;
        if (IsKeyDown(KEY_LEFT)) placa.x -= placa.velocity;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawCircleV(ballPosition, 5, MAROON);
            DrawRectangle(placa.x, placa.y, placa.w, placa.h, MAROON);

            if(ballPosition.y <= 0 || ballPosition.y >= screenHeight){
              vy = -vy;
            }

            if((ballPosition.x <= placa.x + placa.w && ballPosition.x >= placa.x) && (ballPosition.y <= placa.y+placa.h && ballPosition.y >= placa.y)){
              vy = -vy;
              //vx = +vx;
            }

            if(placa.x+placa.w >= screenWidth){
              placa.x = screenWidth-placa.w;
            }

            if(placa.x <= 0){
              placa.x = 0;
            }

            if(ballPosition.x <= 0 || ballPosition.x >= screenWidth){
              vx = -vx;
            }



            ballPosition.x += vx;
            ballPosition.y += vy;

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}