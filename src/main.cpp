#include "raylib.h"
#include "include/resource_dir.h"

int main () {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Raylib Project");
	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
