#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <FLAC/stream_decoder.h>

// ALSA
int init_alsa();
int exit_alsa();

// FLAC
void play_music(const char* filename);
