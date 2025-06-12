#include <stdint.h>
#include "raylib.h"
#include "include/resource_dir.h"
#include "player.h"
#include "objects.h"

enum GAME_STATE : uint8_t {
	MAIN, 		// 0
	GAMEOVER	// 1
};
uint8_t game_state = GAMEOVER;

void StartGame(Player *player, Handler *handler);
void UpdateMain(Player *player, Handler *handler);
void DrawMain(Player *player, Handler *handler);

int main() {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Dino Game");
	SetTargetFPS(100);

	SearchAndSetResourceDir("resources");

	Player player;
	Handler handler;
	
	while(!WindowShouldClose()) {
		// Update
		if(game_state == MAIN) 
			UpdateMain(&player, &handler); 
		else {
			if(IsKeyPressed(KEY_SPACE)) StartGame(&player, &handler);
		}

		// Draw
		BeginDrawing();
		ClearBackground(BLACK);

		if(game_state == MAIN) 
			DrawMain(&player, &handler);
		else {
			DrawText("Press space to play!", 1280 / 2 - 100, 800 / 2 - 50, 20, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void StartGame(Player *player, Handler *handler) {
	PlayerInit(player);
	HandlerInit(handler);

	game_state = MAIN;
}

void UpdateMain(Player *player, Handler *handler) {
	PlayerUpdate(player);
	HandlerUpdate(handler);
}

void DrawMain(Player *player, Handler *handler) {
	PlayerDraw(player);
	HandlerDraw(handler);
}

