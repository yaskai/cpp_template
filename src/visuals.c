#include "raylib.h"
#include "raymath.h"
#include "visuals.h"

void VisualsInit(Visuals *vis) {
	vis->cam = (Camera) {
		.position = (Vector3){0, 0, 0.5f},
		.target = (Vector3){0, 0, -1},
		.up = (Vector3){0, 1, 0},
		.fovy = 70.0f,
		.projection = CAMERA_PERSPECTIVE
	};

	vis->model = LoadModel("hello_kitty.glb");
}

void VisualsDraw(Visuals *vis) {
	BeginMode3D(vis->cam);
	DrawModel(vis->model, (Vector3){0, -0.25f, 0}, 5.0f, WHITE);
	EndMode3D();
}
