#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

const int WIDTH = 850;
const int HEIGHT = 450;
int score = 0;

std::vector<std::pair<Texture2D*, Vector2>> donuts;
Texture2D alien_invasion;
Texture2D asteroid_crumbcake;
Texture2D quantum_foam;
Texture2D solar_flare;
Texture2D titan_toffee;
Texture2D venusian_volcano;
std::vector<Texture2D*> donut_selection;

int donut_adding_timer = 0;

Texture2D player_tex;
float player_x;
float player_y;

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");
	player_tex = LoadTexture("images/alien-donut-eater.png");
	player_tex.width /= 4;
	player_tex.height /= 4;
	player_x = WIDTH/2-player_tex.width/2;
	player_y = HEIGHT-player_tex.height-20;

	alien_invasion = LoadTexture("images/alien-invasion.png");
	asteroid_crumbcake = LoadTexture("images/asteroid-crumbcake.png");
	quantum_foam = LoadTexture("images/quantum-foam.png");
	solar_flare = LoadTexture("images/solar-flare.png");
	titan_toffee = LoadTexture("images/titan-toffee.png");
	venusian_volcano = LoadTexture("images/venusian-volcano.png");

	donut_selection = {
		&alien_invasion, &asteroid_crumbcake, &quantum_foam,
		&solar_flare, &titan_toffee, &venusian_volcano
	};
	for (Texture2D* tex : donut_selection)
	{
		tex->width /= 10; tex->height /= 10;
	}

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

void UpdateDrawFrame()
{
	donut_adding_timer++;

	if (donut_adding_timer >= GetFPS())
	{
		donuts.push_back(std::make_pair(
			donut_selection[GetRandomValue(0, donut_selection.size())],
			Vector2 {(float)GetRandomValue(0, WIDTH), -80}
		));
	}
	BeginDrawing();
	
	ClearBackground(DARKPURPLE);

	for (int i = 0; i < donuts.size(); i++)
	{
		donuts[i].second.y += 100 * GetFrameTime();
		if (CheckCollisionRecs(
			{player_x, player_y, (float)player_tex.width, (float)player_tex.height},
			{donuts[i].second.x, donuts[i].second.y, (float)donuts[i].first->width, (float)donuts[i].first->height}
		))
		{
			donuts.erase(donuts.begin() + i);
			score++;
		}

		DrawTextureV(*donuts[i].first, donuts[i].second, WHITE);
	}
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		player_x -= 200 * GetFrameTime();
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		player_x += 200 * GetFrameTime();

	DrawTexture(player_tex, player_x, player_y, WHITE);

	std::string score_str = "Score: " + score;
	DrawText(score_str.c_str(), 20, 20, 20, WHITE);
	EndDrawing();
}