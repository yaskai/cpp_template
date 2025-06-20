#include <stdio.h>
#include <string.h>
#include "config.h"

int LoadConfig(Config *conf) {
	FILE *file = fopen("options.conf", "r");

	if(!file) {
		puts("ERROR: COULD NOT READ CONFIGURATION FILE");
		return 1;
	}
	
	char buf[128];
	uint8_t colors_added = 0;

	while(fgets(buf, sizeof(buf), file)) {
		if(buf[0] == COMMENT_MARKER) continue;

		if(buf[0] == '[') {
			ChangeBlock(conf, buf);
		}

		if(conf->cur_block == CONF_WINDOW) {
			if(buf[0] == 'r') {
				sscanf(buf, "res: %dx%d", &conf->ww, &conf->wh);
			} 

			if(buf[0] == 'f') {
				sscanf(buf, "fps: %d", &conf->fps);
			}
		}

		if(conf->cur_block == CONF_COLORS) {
			if(buf[0] == '"') {
				conf->colors[colors_added] = ParseColor(buf);
				colors_added++;
			}
		}
	}
	
	fclose(file);	
	return 0;
};

void ChangeBlock(Config *conf, char *line) { 
	char *end = strchr(line, ']');
	if(!end) return;

	size_t len = end - line - 1;
	if(len <= 0) return;

	char block_str[64];
	if(len >= sizeof(block_str)) len = sizeof(block_str) - 1;

	strncpy(block_str, line + 1, len);
	block_str[len] = '\0';

	if(strcmp(block_str, "window") == 0) conf->cur_block = CONF_WINDOW;
	else if(strcmp(block_str, "colors") == 0) conf->cur_block = CONF_COLORS;
}

Color ParseColor(char *line) {
	Color color = (Color){0, 0, 0, 0};

	char *end = strchr(line + 1, '"');
	if(!end) return color;

	size_t len = end - line - 1;
	if(len <= 0) return color;

	char buf[64];
	char clr_str[64];
	if(len >= sizeof(clr_str)) len = sizeof(clr_str) - 1;

	strncpy(clr_str, line + 1, len);
	clr_str[len] = '\0';

	strcpy(buf, "0x");
	strcat(buf, clr_str + 1);
	printf("%s\n", buf);
	
	unsigned int val;
	sscanf(buf, "%x", &val); 

	color = HexToRgb(val);
	return color;
}

Color HexToRgb(unsigned int hex) {
	return (Color) {
		.r = (hex >> 16) & 0xff,
		.g = (hex >> 8) & 0xff,
		.b = hex & 0xff,
		.a = 255
	};
}

