#include "objects.h"

float spawn_timer; 

void HandlerInit(Handler *handler) {
	spawn_timer = 10.0f;
	handler->hazard_count = 0;
}

void HandlerUpdate(Handler *handler) {
	spawn_timer -= GetFrameTime();
	if(spawn_timer <= 0) {
		AddHazard(handler, (Vector2){1300, 700});
		spawn_timer = GetRandomValue(5, 15);
	}

	for(uint8_t i = 0; i < handler->hazard_count; i++) {
		// Make shortcut pointer to address of hazard[i]
		Hazard *hazard = &handler->hazards[i];
			
		// Move and update hazard bounds rectangle
		hazard->position.x -= 20.0f * GetFrameTime();
		hazard->bounds.x = hazard->position.x; 

		// Remove hazard if outside screen bounds
		if(hazard->position.x < -100) RemoveHazard(handler, i);	
	}
}

void HandlerDraw(Handler *handler) {
	for(uint8_t i = 0; i < handler->hazard_count; i++) {
		DrawRectangleRec(handler->hazards[i].bounds, YELLOW);
	}
}

void AddHazard(Handler *handler, Vector2 position) {
	// Create an instance of hazard struct
	Hazard new_hazard = (Hazard) {
		.position = position,
		.bounds = (Rectangle){position.x, position.y, 100, 100}
	};

	// Add hazard instance to array
	handler->hazards[handler->hazard_count] = new_hazard;

	// Update count
	handler->hazard_count++;
}

void RemoveHazard(Handler *handler, uint8_t id) {
	// Shift all instances one place starting at id
	for(uint8_t i = id; i < handler->hazard_count; i++) {
		handler->hazards[i] = handler->hazards[i + 1];	
	}	
	
	// Update count
	handler->hazard_count--;
}

