#ifndef VISUALS_H_
#define VISUALS_H_

#include "raylib.h"

typedef struct {
	Camera cam;
	Model model;
} Visuals;

void VisualsInit(Visuals *vis);
void VisualsDraw(Visuals *vis);

#endif
