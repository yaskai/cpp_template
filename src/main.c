#include "raylib.h"
#include "include/resource_dir.h"
#include "config.h"
#include "audioplayer.h"
#include "ui.h"
#include "visuals.h"

int main() {
	SetTraceLogLevel(LOG_ERROR);

	Config conf = (Config){0};
	LoadConfig(&conf);

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	//InitWindow(1366, 768, "Sav Craft");
	//SetTargetFPS(100);

	InitWindow(conf.ww, conf.wh, "Sav Craft");
	SetTargetFPS(conf.fps);

	InitAudioDevice();

	SearchAndSetResourceDir("resources");

	AudioPlayer ap = (AudioPlayer){0};
	AudioPlayerInit(&ap);

	Ui ui = (Ui){0};
	UiInit(&ui, &ap, &conf);

	Visuals vis = (Visuals){0};
	VisualsInit(&vis);
	
	while(!WindowShouldClose()) {
		AudioPlayerUpdate(&ap);
		UiUpdate(&ui);

		BeginDrawing();
		ClearBackground(BLACK);

		VisualsDraw(&vis);
		UiDraw(&ui);

		EndDrawing();
	}

	CloseWindow();
	AudioPlayerClose(&ap);
	UiClose(&ui);

	return 0;
}

