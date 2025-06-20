#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <stdint.h>
#include "raylib.h"

#define AP_PAUSE 0x01

typedef struct {
	uint8_t flags;

	uint8_t track_count;
	uint8_t track_playing;
	char **track_paths;

	float pb_time;
	float track_prog;
	float pause_stamp;

	float *track_lengths;
	char **len_strs;

	Music cur_stream;
} AudioPlayer;

void AudioPlayerInit(AudioPlayer *ap);
void AudioPlayerClose(AudioPlayer *ap);
void AudioPlayerUpdate(AudioPlayer *ap);

void LoadTrack(AudioPlayer *ap, uint8_t id);

void PlayCurrent(AudioPlayer *ap);
void SkipRewind(AudioPlayer *ap);
void SkipNext(AudioPlayer *ap);

#endif
