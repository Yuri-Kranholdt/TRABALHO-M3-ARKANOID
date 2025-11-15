#include "raylib.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "map.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int screenWidth = 800;
int screenHeight = 450;

struct Bloco {
  int vida;
  int x;
  int y;
  int w;
  int h;
  bool is_wall;
  bool is_dead;
  Color color;
  int velocity;
};

Color cores[] = {
    {0,   0,   255, 255},  // azul
    {0,   255, 0,   255},  // verde
    {255, 165, 0,   255},  // laranja
    {255, 0,   0,   255},  // vermelho
    {0,   255, 255, 255},  // ciano
    {255, 0,   255, 255},  // magenta
    {255, 255, 0,   255},  // amarelo
    {192, 192, 192, 255},  // prata
    {255, 215, 0,   255}   // ouro
};


int raio = 6;

int per_line = 10;

int bloco_h = 30;

int wall_width = bloco_h;

int bloco_w = (screenWidth - (wall_width * 2)) / per_line;

int n_lines = 3;

int start_from = n_lines * bloco_h;

bool starting = true;


std::vector<Bloco> blocos;

Bloco placa = {1, screenWidth/2, screenHeight-20, 80, 20, true, false, MAROON, 5};
Vector2 ballPosition = { 0, 0 };


int vx = 0;
int vy = 0;

void block_colision(Bloco *bloco){

    if(ballPosition.x + raio >= bloco->x && 
       ballPosition.x - raio <= bloco->x + bloco->w &&
       ballPosition.y + raio >= bloco->y && 
       ballPosition.y - raio <= bloco->y + bloco->h){

        if(!bloco->is_wall){
            if(bloco->vida - 1 <= 0){
                bloco->is_dead = true;
            }
            else {
                bloco->vida--;
            }
        }
       
       int overlap_left = (ballPosition.x + raio) - bloco->x;
       int overlap_right = (bloco->x + bloco->w) - (ballPosition.x - raio);
       int overlap_top = (ballPosition.y + raio) - bloco->y;
       int overlap_bottom = (bloco->y + bloco->h) - (ballPosition.y - raio);

       int touched = std::min({overlap_left, overlap_right, overlap_top, overlap_bottom});

       if(touched == overlap_left){
            vx = -vx;
            ballPosition.x = bloco->x - raio;
       }
       else if (touched == overlap_right) {
            vx = abs(vx);
            ballPosition.x = bloco->x + bloco->w + raio;
       }
       else if (touched == overlap_top){
             vy = -vy;
            ballPosition.y = bloco->y - raio;
       }
       else if (touched == overlap_bottom) {
            vy = abs(vy);
            ballPosition.y = bloco->y + bloco->h + raio;
       }

       else {
            vx = -vx;
            vy = -vy;
       }

    }

}

Color cor_aleatoria(){
    unsigned char r = rand() % 256;
    unsigned char g = rand() % 256;
    unsigned char b = rand() % 256;

    return {r, g, b, 255};
}

void read_map(const Mapa& matriz){
    int total_lines = (screenHeight-(bloco_h*2))/bloco_h;

    for(int i=0; i<total_lines; i++){
        for(int j=0; j<per_line; j++){
            int index = matriz[i][j];
            if(index != 0){
                Color cor = cores[index-1];
                int life = index == 8 or index == 9 ? 2 : 1;
                bool is_wall = index == 8 ? true : false;

                blocos.push_back({
                    life, 
                    wall_width+(j*bloco_w),
                    bloco_h + (i*bloco_h),
                    bloco_w,
                    bloco_h,
                    is_wall,
                    false,
                    cor,
                    0
                });
            }
        }
    }
}

void init_walls(){
    blocos.push_back({1, 0, bloco_h, wall_width, screenHeight-bloco_h, true, false, Color{137, 137, 137, 255}, 0});
    blocos.push_back({1, screenWidth-wall_width, bloco_h, wall_width, screenHeight-bloco_h, true, false, Color{137, 137, 137, 255}, 0});
    blocos.push_back({1, 0, 0, screenWidth, bloco_h, true, false, Color{137, 137, 137, 255}, 0});
}

void draw_blocks(){
    for(Bloco &block : blocos){
        if(!block.is_dead){
            DrawRectangle(block.x, block.y, block.w, block.h, block.color);
            block_colision(&block);
        }
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    init_walls();

    read_map(GetMapa(2));

    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");

    srand(static_cast<unsigned int>(time(0)));

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

            DrawCircleV(ballPosition, raio, MAROON);
            DrawRectangle(placa.x, placa.y, placa.w, placa.h, MAROON);
            draw_blocks();

            if(ballPosition.y <= 0 || ballPosition.y >= screenHeight){
                vy = -vy;
            }

            block_colision(&placa);

            if(IsKeyDown(KEY_SPACE) && starting){
                vy = 4;
                vx = 1;
                starting = false;
            }

            if(starting){
                ballPosition.x = placa.x +(placa.w/ 2);
                ballPosition.y = placa.y - raio;
            }


            if(placa.x+placa.w >= screenWidth-wall_width){
                placa.x = (screenWidth-wall_width)-placa.w;
            }

            if(placa.x <= wall_width){
                placa.x = wall_width;
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