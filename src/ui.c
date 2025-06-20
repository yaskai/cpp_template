#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "audioplayer.h"
#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "ui.h"

float title_scroll = 0;
float title_width = 0;
RenderTexture2D title_rt;

Vector2 panel_drag_pos;

void UiInit(Ui *ui, AudioPlayer *ap, Config *conf) {
	ui->flags = (TAB_OPEN);
	ui->ap = ap;

	ui->font = LoadFontEx("maple-mono.ttf", 128, 0, 0);
	ui->icons = GetIcons(ui);

	/*
	Color colors[4] = {
		{20, 24, 82, 255},
		{231, 121, 160, 255}, 
		{218, 196, 185, 255},
		{20, 24, 82, 255},
	};
	memcpy(ui->colors, colors, sizeof(Color) * 4);
	*/

	//memcpy(ui->colors, &conf->colors, sizeof(Color) * 4); 

	unsigned int clr[4] = {
		0x00000,
		0xff7a8,
		0xffccaa,
		0x83769c
	};

	//for(uint8_t i = 0; i < 4; i++) ui->colors[i] = HexToRgb(clr[i]);
	for(uint8_t i = 0; i < 4; i++) ui->colors[i] = conf->colors[i];

	ui->panel_pos = (Vector2){ 100, 200 };
	ui->panel_rect = (Rectangle){ 100, 200, 400, 400 };
	title_rt = LoadRenderTexture(350, 100);
		
	UiMakeButtons(ui);

	ui->prog_bar = (Bar) {
		.flags = 0,	
		.anchor = (Vector2){ui->panel_rect.x, ui->panel_rect.y},
		.rec = (Rectangle){10, 260, 380, 30},
		.fill = &ui->ap->track_prog
	};

	ui->track_tab = (TrackTab) {
		.flags = 0,
		.entry_count = ap->track_count,
		.position = Vector2Zero(),
		.offset = Vector2Zero(),
		.size = (Vector2){600, 40},
		.anchor = NULL
	};
}

void UiClose(Ui *ui) {
	free(ui->buttons);
	free(ui->icons);
}

void UiUpdate(Ui *ui) {
	PanelUpdate(ui);

	for(uint8_t i = 0; i < ui->button_count; i++) {
		Button *btn = &ui->buttons[i];
		ButtonUpdate(btn, ui);
	}
}
 
void UiDraw(Ui *ui) {
	DrawRectangleLinesEx((Rectangle){0, 0, ui->track_tab.size.x, ui->track_tab.size.y}, 2.0f, ui->colors[FG]);
	if(ui->flags & TAB_OPEN) {
		DrawTextEx(ui->symbols, "", (Vector2){10, 15}, 20, 1, ui->colors[FG]);

		for(uint8_t i = 0; i < ui->track_tab.entry_count; i++) {
			Vector2 title_pos = (Vector2){10, ui->track_tab.size.y * (i + 1) + 10};
			Vector2 dur_pos = (Vector2){490, ui->track_tab.size.y * (i + 1) + 10};

			uint8_t col_idA = (ui->ap->track_playing == i) ? BGA : FG;
			uint8_t col_idB = (ui->ap->track_playing == i) ? FG : BGA;
			Color colA = ui->colors[col_idA];
			Color colB = ui->colors[col_idB];

			Rectangle rec = (Rectangle){0, ui->track_tab.size.y * (i + 1), ui->track_tab.size.x, ui->track_tab.size.y};
			
			DrawRectangleRec(rec, colB);
			DrawRectangleLinesEx(rec, 2.0f, ui->colors[FG]);

			DrawTextEx(ui->font, TextFormat("%s", ui->ap->track_paths[i]), title_pos, 20, 1, colA);
			DrawTextEx(ui->font, TextFormat("%s", ui->ap->len_strs[i]), dur_pos, 20, 1, colA);

			if(CheckCollisionPointRec(GetMousePosition(), rec) && IsMouseButtonPressed(0)) {
				LoadTrack(ui->ap, i);
				PlayCurrent(ui->ap);
			}
		}
	}

	PanelDraw(ui);

	for(uint8_t i = 0; i < ui->button_count; i++) {
		Button *btn = &ui->buttons[i];
		ButtonDraw(btn, ui);
	}
}

int *GetIcons(Ui *ui) {
	int code_points[] = { 
		0x0001, // none
		0xf04b, // play
		0xf048, // prev
		0xf051, // next
		0xf04c, // pause
		0xf0de,	// up arrow
	};

	int count = (sizeof(code_points) / (code_points[0]));
	ui->symbols = LoadFontEx("glyphs.ttf", 128, code_points, count);

	int *icons = (int*)malloc(sizeof(int) * count);	
	memcpy(icons, code_points, sizeof(int) * count);
		
	return icons;
}

void UiMakeButtons(Ui *ui) {
	AddButton(ui, (Rectangle){80,  300, 70, 60}, ICO_PREV, "", PREV);
	AddButton(ui, (Rectangle){160, 300, 70, 60}, ICO_PLAY, "", PLAY);
	AddButton(ui, (Rectangle){240, 300, 70, 60}, ICO_NEXT, "", NEXT);
}

void AddButton(Ui *ui, Rectangle rec, int ico_id, char *text, uint8_t type) {
	Button btn = (Button) {
		.flags = 0,
		.type = type,
		.anchor = &ui->panel_pos,
		.offset = (Vector2){rec.x, rec.y},
		.size = (Vector2){rec.width, rec.height},
		.icon = ico_id,
		.text = text
	};

	ui->buttons = realloc(ui->buttons, sizeof(Button) * (++ui->button_count));
	ui->buttons[ui->button_count - 1] = btn;
}

void ButtonUpdate(Button *btn, Ui *ui) {
	btn->flags &= ~BTN_HOVER;

	if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){btn->position.x, btn->position.y, btn->size.x, btn->size.y}))	
		btn->flags |= BTN_HOVER;

	if(IsMouseButtonPressed(0) && (btn->flags & BTN_HOVER)) {
		ButtonOnPress(btn, ui);
	}
	
	btn->position = Vector2Add(btn->offset, *btn->anchor);

	if(btn->icon == ICO_PLAY || btn->icon == ICO_PAUSE)
		btn->icon = (ui->ap->flags & AP_PAUSE) ? ICO_PLAY: ICO_PAUSE;
}

void ButtonDraw(Button *btn, Ui *ui) {
	if(btn->flags & BTN_HIDDEN) return;

	Rectangle rec = (Rectangle){ btn->position.x, btn->position.y, btn->size.x, btn->size.y };
	Vector2 center = (Vector2){ btn->position.x + btn->size.x * 0.25f, btn->position.y + btn->size.y * 0.25f };

	float font_size = 40;
	uint8_t fg_id = (btn->flags & BTN_HOVER) ? 2 : 1;
	float round = 0.15f;
	int seg = 32;

	int ico_id = btn->icon;
	int ico_w = GetGlyphAtlasRec(ui->symbols, ui->icons[ico_id]).width * font_size / ui->symbols.baseSize;
	int ico_h = font_size;

	Vector2 ico_pos = (Vector2){ rec.x + (rec.width - ico_w) / 2, rec.y + (rec.height - ico_h) / 2 + 2 };

	DrawTextEx(ui->font, btn->text, center, 40, 1, ui->colors[fg_id]);
	DrawTextCodepoint(ui->symbols, ui->icons[btn->icon], ico_pos, font_size, ui->colors[fg_id]);
}

void ButtonOnPress(Button *btn, Ui *ui) {
	switch(btn->type) {
		case PLAY: PlayCurrent(ui->ap);	break;
		case PREV: SkipRewind(ui->ap); 	break;
		case NEXT: SkipNext(ui->ap); 	break;
	}
	
}

void PanelUpdate(Ui *ui) {
	Vector2 mouse_pos = GetMousePosition();

	Rectangle prog_rec = (Rectangle) {
		ui->prog_bar.rec.x + ui->panel_rect.x, 
		ui->prog_bar.rec.y + ui->panel_rect.y,
		ui->prog_bar.rec.width,
		ui->prog_bar.rec.height
	};
		
	AudioPlayer *ap = ui->ap;

	if(CheckCollisionPointRec(GetMousePosition(), prog_rec) && IsMouseButtonDown(0)) {
		float t = Clamp((GetMouseX() - prog_rec.x) / prog_rec.width, 0, 1);
		float targ = t * ap->track_lengths[ap->track_playing];

		SeekMusicStream(ap->cur_stream, targ);
		ap->pause_stamp = targ;
		ap->pb_time = targ;
		ap->track_prog = t;
		return;
	}

	if(CheckCollisionPointRec(mouse_pos, ui->panel_rect) && IsMouseButtonPressed(0)) {
		ui->flags |= PANEL_DRAG;
		panel_drag_pos = (Vector2){ mouse_pos.x - ui->panel_rect.x, mouse_pos.y - ui->panel_rect.y };
	} 
	
	if(IsMouseButtonReleased(0)) ui->flags &= ~PANEL_DRAG;

	if(ui->flags & PANEL_DRAG) {
		Vector2 next_pos = Vector2Subtract(mouse_pos, panel_drag_pos);
		ui->panel_pos = next_pos;
		ui->panel_rect.x = next_pos.x, ui->panel_rect.y = next_pos.y;
	}	
}

void PanelDraw(Ui *ui) {
	AudioPlayer *ap = ui->ap;

	float round = 0.1f;
	int seg = 32;

	DrawRectangleRounded(ui->panel_rect, round, seg, ui->colors[BG]);
	DrawRectangleRoundedLinesEx(ui->panel_rect, round, seg, 2.0f, ui->colors[FG]);

	BeginTextureMode(title_rt);
	ClearBackground((Color){ 0, 0, 0, 0});

	char title_buf[(strlen(ap->track_paths[ap->track_playing]) * 2) + 8];
	char *sep = " ** ";

	Vector2 title_pos = (Vector2){ ui->panel_rect.x + 10, ui->panel_rect.y + ui->panel_rect.height * 0.5f };

	title_scroll -= GetFrameTime() * 70;
	if(title_scroll + title_width < -title_width) title_scroll = 0;
		
	strcpy(title_buf, sep);
	strcat(title_buf, ap->track_paths[ap->track_playing]);
	strcat(title_buf, sep);
	strcat(title_buf, ap->track_paths[ap->track_playing]);

	title_width = strlen(title_buf) * 7.35f;

	DrawTextEx(ui->font, title_buf, (Vector2){title_scroll, 0}, 30, 1, ui->colors[FG]);
	DrawTextEx(ui->font, title_buf, (Vector2){title_scroll + (title_width * 2), 0}, 30, 1, ui->colors[FG]);
	EndTextureMode();

	DrawTextureRec(title_rt.texture, (Rectangle){0, 0, title_rt.texture.width, -title_rt.texture.height}, title_pos, WHITE);
	
	Rectangle prog_rec = (Rectangle) {
		ui->prog_bar.rec.x + ui->panel_rect.x, 
		ui->prog_bar.rec.y + ui->panel_rect.y,
		ui->prog_bar.rec.width,
		ui->prog_bar.rec.height
	};

	Rectangle fill_rec = prog_rec;
	fill_rec.width = ui->prog_bar.rec.width * *ui->prog_bar.fill;
	
	DrawRectangleRounded(prog_rec, round, seg, ui->colors[BGA]);	
	DrawRectangleRoundedLinesEx(prog_rec, round, seg, 2.0f, ui->colors[FG]);
	DrawRectangleRounded(fill_rec, round, seg, ui->colors[FG]);	
	
	char et_buf[6];
	int mins = (int)ap->pb_time / 60;
	int secs = (int)ap->pb_time % 60;
	snprintf(et_buf, sizeof(et_buf), "%02d:%02d", mins, secs);
	
	char tt_buf[6];
	mins = (int)ap->track_lengths[ap->track_playing] / 60;
	secs = (int)ap->track_lengths[ap->track_playing] % 60;
	snprintf(tt_buf, sizeof(tt_buf), "%02d:%02d", mins, secs);

	DrawTextEx(
		ui->font,
		TextFormat("%s/%s", et_buf, tt_buf),
		(Vector2){ui->panel_pos.x + prog_rec.width / 2 - 50, ui->panel_pos.y + 265},
		22,
		1,
		ui->colors[HL]
	);
}

