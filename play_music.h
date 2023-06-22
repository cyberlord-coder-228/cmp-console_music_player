#include <stdio.h>
#include <stdlib.h>
#include <FLAC/stream_decoder.h>
#include <alsa/asoundlib.h>

// ALSA
// const char* DEVICE = "default";
// snd_pcm_t* PCM_HANDLE;
// snd_pcm_hw_params_t* PARAMS;
// snd_pcm_uframes_t FRAMES;

void alsa_error(const char *message, int error);

int init_alsa();

int exit_alsa();

// FLAC
// FLAC__StreamDecoder* DECODER;

FLAC__StreamDecoderWriteStatus _write_callback_(
    const FLAC__StreamDecoder *decoder,
    const FLAC__Frame *frame,
    const FLAC__int32* const buffer[],
    void* client_data
);

void _error_callback_(
    const FLAC__StreamDecoder* decoder,
    FLAC__StreamDecoderErrorStatus status,
    void *client_data
);

FLAC__StreamDecoderInitStatus _init_flac_for_(const char* filename);

void play_flac(const char* filename);
