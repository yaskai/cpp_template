#pragma once

#include <stdint.h>
#include "raylib.h"

#define MAX_HAZARDS 255 

typedef struct {
	Vector2 position;
	Rectangle bounds;
} Hazard;

typedef struct {
	uint8_t hazard_count;
	Hazard hazards[MAX_HAZARDS];
} Handler;

void HandlerInit(Handler *handler);
void HandlerUpdate(Handler *handler);
void HandlerDraw(Handler *handler);

void AddHazard(Handler *handler, Vector2 position); // Make a new hazard at <position> and add to array
void RemoveHazard(Handler *handler, uint8_t id); 	// Remove hazard at index[id] from array

