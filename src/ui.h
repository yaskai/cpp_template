#ifndef UI_H
#define UI_H

#include <stdint.h>
#include "raylib.h"
#include "config.h"
#include "audioplayer.h"

enum ICON_ID : uint8_t {
	ICO_NONE, 
	ICO_PLAY,
	ICO_PREV,
	ICO_NEXT,
	ICO_PAUSE,
	ICO_UPARROW,
	ICO_DOWNARROW
};

#define BTN_HOVER	0x01
#define BTN_HIDDEN	0x02
#define PANEL_DRAG	0x04 
#define TAB_OPEN	0x08

enum BUTTON_TYPE : uint8_t {
	PLAY,
	PREV,
	NEXT
};

typedef struct {
	uint8_t flags;
	uint8_t type;	

	Vector2 position;
	Vector2 size;
	Vector2 offset;
	Vector2 *anchor;

	int icon;
	char *text;
} Button;

typedef struct {
	uint8_t flags;
	
	Vector2 anchor;
	Rectangle rec;

	float *fill;
} Bar;

typedef struct {
	uint8_t flags;
	uint8_t entry_count;

	Vector2 position;
	Vector2 size;
	Vector2 offset;
	Vector2 *anchor;
} TrackTab;

typedef struct {
	uint8_t flags;

	uint8_t button_count;
	Button *buttons;

	Vector2 panel_pos;
	Rectangle panel_rect;

	Bar prog_bar;
	TrackTab track_tab;

	AudioPlayer *ap;

	Font font;
	Font symbols;

	int *icons;
	Color colors[4];
} Ui;

void UiInit(Ui *ui, AudioPlayer *ap, Config *conf);
void UiClose(Ui *ui);

void UiUpdate(Ui *ui);
void UiDraw(Ui *ui);

int *GetIcons(Ui *ui);
void UiMakeButtons(Ui *ui);

void AddButton(Ui *ui, Rectangle rec, int ico_id, char *text, uint8_t type);
void ButtonUpdate(Button *button, Ui *ui);
void ButtonDraw(Button *btn, Ui *ui);
void ButtonOnPress(Button *btn, Ui *ui);

void PanelUpdate(Ui *ui);
void PanelDraw(Ui *ui);

#endif
