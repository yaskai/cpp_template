#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include "raylib.h"

#define COMMENT_MARKER '#'

enum CONfIG_BLOCKS : uint8_t {
	CONF_WINDOW,
	CONF_COLORS
};

enum UI_COLORS : uint8_t { BG, FG, HL, BGA };

typedef struct {
	uint8_t cur_block;

	unsigned int flags;
	unsigned int ww, wh, fps;

	Color colors[4];
} Config;

int LoadConfig(Config *conf);
void ChangeBlock(Config *conf, char *line);

Color ParseColor(char *line);
Color HexToRgb(unsigned int hex);

#endif
