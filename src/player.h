#pragma once

#include "raylib.h"

typedef struct {
	bool on_ground;

	Vector2 position;
	Vector2 velocity;
	Rectangle bounds;
} Player;

void PlayerInit(Player *player);
void PlayerUpdate(Player *player);
void PlayerDraw(Player *player);

