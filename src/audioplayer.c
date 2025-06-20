#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "audioplayer.h"

void AudioPlayerInit(AudioPlayer *ap) {
	ap->track_count = 0;
	ap->flags = (AP_PAUSE);

	DIR *dir = opendir("tracks/"); 
	struct dirent *entry;

	while((entry = readdir(dir)) != NULL) {
		if(entry->d_type != DT_REG) continue;

		ap->track_paths = realloc(ap->track_paths, sizeof(char*) * (ap->track_count+1));

		char *str = strdup(entry->d_name);
		char *ext = strrchr(str, '.');
		if(ext) *ext = '\0';
		ap->track_paths[ap->track_count] = str;

		ap->track_count++;
	}

	closedir(dir);

	ap->track_lengths = (float*)malloc(sizeof(float) * ap->track_count);
	for(uint8_t i = 0; i < ap->track_count; i++) {
		Music track = LoadMusicStream(TextFormat("tracks/%s.mp3", ap->track_paths[i]));
		ap->track_lengths[i] = GetMusicTimeLength(track);
		UnloadMusicStream(track);
	}

	ap->len_strs = (char**)malloc(sizeof(char*) * ap->track_count);
	
	puts("TRACKS:");
	for(uint8_t i = 0; i < ap->track_count; i++) {
		int mins = (int)ap->track_lengths[i] / 60;
		int secs = (int)ap->track_lengths[i] % 60;

		char time_buf[6];
		snprintf(time_buf, sizeof(time_buf), "%02d:%02d", mins, secs);
		if(time_buf[0] == '0') time_buf[0] = ' ';
		ap->len_strs[i] = strdup(time_buf);
		printf("%d. %s, %s \n", i, ap->track_paths[i], ap->len_strs[i]);
	}

	LoadTrack(ap, ap->track_playing);
}

void AudioPlayerClose(AudioPlayer *ap) {
	free(ap->track_paths);
	free(ap->track_lengths);
	free(ap->len_strs);
	UnloadMusicStream(ap->cur_stream);
}

void AudioPlayerUpdate(AudioPlayer *ap) {
	UpdateMusicStream(ap->cur_stream);

	if((ap->flags & AP_PAUSE) == 0) {
		UpdateMusicStream(ap->cur_stream);
		ap->pb_time += GetFrameTime();

		if(ap->pb_time > ap->track_lengths[ap->track_playing]) SkipNext(ap);
	}

	ap->track_prog = ap->pb_time / ap->track_lengths[ap->track_playing];
    if (ap->track_prog > 1.0f) ap->track_prog = 1.0f;
}

void LoadTrack(AudioPlayer *ap, uint8_t id) {
	UnloadMusicStream(ap->cur_stream);
	ap->track_prog = 0;
	ap->pause_stamp = 0;
	ap->pb_time = 0;
	ap->cur_stream = LoadMusicStream(TextFormat("tracks/%s.mp3", ap->track_paths[id]));
	ap->track_playing = id;
}

void PlayCurrent(AudioPlayer *ap) {
    if (ap->flags & AP_PAUSE) {
        PlayMusicStream(ap->cur_stream);
        ap->flags &= ~AP_PAUSE;
    } else {
        PauseMusicStream(ap->cur_stream);
        ap->pause_stamp = GetMusicTimePlayed(ap->cur_stream);
        ap->flags |= AP_PAUSE;
    }
}

void SkipRewind(AudioPlayer *ap) {
	if(ap->pb_time > 2.0f) {
		ap->pb_time = 0;
		SeekMusicStream(ap->cur_stream, 0);
		return;
	}

	if(ap->track_playing < 1) ap->track_playing = ap->track_count - 1; else ap->track_playing--;
	LoadTrack(ap, ap->track_playing);
	ap->flags &= ~AP_PAUSE;
	PlayMusicStream(ap->cur_stream);
}

void SkipNext(AudioPlayer *ap) {
	ap->track_playing++;
	if(ap->track_playing > ap->track_count - 1) ap->track_playing = 0;
	LoadTrack(ap, ap->track_playing);
	ap->flags &= ~AP_PAUSE;
	PlayMusicStream(ap->cur_stream);
}

