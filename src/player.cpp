#include "raylib.h"
#include "player.h"

const float gravity = 80.35f;

void PlayerInit(Player *player) {
	player->on_ground = true;
	player->position = (Vector2){100, 700};
	player->velocity = (Vector2){0, 0};
	player->bounds = (Rectangle){100, 700, 100, 100};
}

void PlayerUpdate(Player *player) {
	// Get delta time
	float dt = GetFrameTime();

	// [Take input]
	// Jump
	if(IsKeyPressed(KEY_SPACE) && player->on_ground) {
		player->velocity.y = -200.0f;
		player->on_ground = false;
	}

	// [Test for collisions]
	// Check if grounded(only if falling!)	
	if(player->position.y >= 700 && player->velocity.y > 0) {
		player->on_ground = true;
		player->position.y = 700;
	}

	// [Update position + bounds + physics]

	// Add gravity if not grounded
	if(!player->on_ground) player->velocity.y += gravity * dt;
	
	// Update position
	player->position = (Vector2) { 
		player->position.x + player->velocity.x * dt,
		player->position.y + player->velocity.y * dt
	};

	// Update bounds
	player->bounds = (Rectangle) {
		player->position.x, // X
		player->position.y, // Y
		100,				// W
		100					// H
	};
}

void PlayerDraw(Player *player) {
	DrawRectangleRec(player->bounds, DARKGREEN);	
}

