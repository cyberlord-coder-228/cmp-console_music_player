#include <stdio.h>
#include <stdlib.h>
#include <FLAC/stream_decoder.h>
#include <alsa/asoundlib.h>

// ALSA
const char* DEVICE = "default";
snd_pcm_t* PCM_HANDLE;
snd_pcm_hw_params_t* PARAMS;
snd_pcm_uframes_t FRAMES;

void alsa_error(const char *message, int error)
{
    fprintf(stderr, "%s: %s\n", message, snd_strerror(error));
    exit(EXIT_FAILURE);
}

int init_alsa()
{
    int e; // error code (if any)

    // Open PCM device for playback
    e = snd_pcm_open(&PCM_HANDLE, DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if (e < 0)
    {
        fprintf(stderr, "Cannot open PCM device: %s\n", snd_strerror(e));
        return e;
    }

    // Allocate hardware parameters object
    e = snd_pcm_hw_params_malloc(&PARAMS);
    if (e < 0)
    {
        fprintf(
            stderr,
            "Cannot allocate hardware parameter structure: %s\n",
            snd_strerror(e)
        );
        return e;
    }

    // Initialize hardware parameters
    e = snd_pcm_hw_params_any(PCM_HANDLE, PARAMS);
    if (e < 0)
    {
        fprintf(
            stderr,
            "Cannot initialize hardware parameter structure: %s\n",
            snd_strerror(e)
        );
        return e;
    }

    // Enable resampling
    unsigned int resample = 1;
    e = (PCM_HANDLE, PARAMS, resample);
    if (e < 0)
    {
        fprintf(stderr, "Cannot set resampling: %s\n", snd_strerror(e));
        return e;
    }

    // Set access type to interleaved
    e = snd_pcm_hw_params_set_access(
        PCM_HANDLE,
        PARAMS,
        SND_PCM_ACCESS_RW_INTERLEAVED
    );
    if (e < 0)
    {
        fprintf(stderr, "Cannot set access type: %s\n", snd_strerror(e));
        return e;
    }

    // Set sample format
    e = snd_pcm_hw_params_set_format(PCM_HANDLE, PARAMS, SND_PCM_FORMAT_S16);
    if (e < 0)
    {
        fprintf(stderr, "Cannot set sample format: %s\n", snd_strerror(e));
        return e;
    }

    // Set sample rate
    unsigned int sample_rate = 44100;
    e = snd_pcm_hw_params_set_rate_near(PCM_HANDLE, PARAMS, &sample_rate, 0);
    if (e < 0)
    {
        fprintf(stderr, "Cannot set sample rate: %s\n", snd_strerror(e));
        return e;
    }

    // Set number of channels
    unsigned int channels = 2;
    e = snd_pcm_hw_params_set_channels(PCM_HANDLE, PARAMS, channels);
    if (e < 0)
    {
        fprintf(stderr, "Cannot set channel count: %s\n", snd_strerror(e));
        return e;
    }

    // Apply hardware parameters
    e = snd_pcm_hw_params(PCM_HANDLE, PARAMS);
    if (e < 0)
    {
        fprintf(stderr, "Cannot set parameters: %s\n", snd_strerror(e));
        return e;
    }

    // Get the number of frames per period
    snd_pcm_hw_params_get_period_size(PARAMS, &FRAMES, 0);

    // // Thats simpler way, but it does not work
    // unsigned int channels = 2;
    // unsigned int rate = 44100;
    // int resample = 1; // allow resampling
    // unsigned int latency = 50000;
    // e = snd_pcm_set_params(
    //     PCM_HANDLE,
    //     SND_PCM_FORMAT_U8,
    //     SND_PCM_ACCESS_RW_INTERLEAVED,
    //     channels,
    //     rate,
    //     resample,
    //     latency
    // );
    // if (e < 0)
    // {
    //     fprintf(stderr, "Cannot set parameters: %s\n", snd_strerror(e));
    // }

    return 1;
}

int exit_alsa()
{
    int err;
    err = snd_pcm_close(PCM_HANDLE);
    if (err < 0)
    {
        fprintf(stderr, "Failed to close PCM device: %s\n", snd_strerror(err));
    }
    return err;
}

// FLAC
FLAC__StreamDecoder* DECODER;

FLAC__StreamDecoderWriteStatus _write_callback_(
    const FLAC__StreamDecoder *decoder,
    const FLAC__Frame *frame,
    const FLAC__int32* const buffer[],
    void* client_data
)
{
    int err;

    unsigned int channels = frame->header.channels;
    unsigned int samples = frame->header.blocksize;

    // Convert double track signal to interleaved
    // eg { left(32): AAAA, right(32): BBBB } -> data(16): ABABABAB
    int16_t* data = (int16_t*)malloc(channels*samples*sizeof(int16_t));
    for (int s = 0; s < samples; s++)
    {
        for (int ch = 0; ch < channels; ch++)
        {
            int pos = channels*s + ch;
            data[pos] = (int16_t)(buffer[ch][s]);
        }
    }

    // Write the decoded frame to the ALSA PCM device
    snd_pcm_sframes_t written = 0;
    
        written = snd_pcm_writei(PCM_HANDLE, data, samples);

        if (written < 0)
        {
            if (err == -EPIPE) alsa_error("underrun occurred", err);
            else alsa_error("error writing to PCM device", err);
        }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void _error_callback_(
    const FLAC__StreamDecoder* decoder,
    FLAC__StreamDecoderErrorStatus status,
    void *client_data
)
{
    fprintf(
        stderr,
        "FLAC decoding error: %s\n",
        FLAC__StreamDecoderErrorStatusString[status]
    );
    exit(EXIT_FAILURE);
}

FLAC__StreamDecoderInitStatus _init_flac_for_(const char* filename)
{
    DECODER = FLAC__stream_decoder_new();
    FLAC__StreamDecoderInitStatus status = FLAC__stream_decoder_init_file(
        DECODER,
        filename,
        _write_callback_,
        NULL,
        _error_callback_,
        NULL
    );
    if (status != FLAC__STREAM_DECODER_INIT_STATUS_OK)
    {
        fprintf(
            stderr,
            "Could not init flac decoder: %s\n",
            status
        );
    }
    return status;
}

void play_flac(const char* filename)
{
    _init_flac_for_(filename);

    // Start decoding with playback callback
    if (!FLAC__stream_decoder_process_until_end_of_stream(DECODER))
    {
        fprintf(stderr, "FLAC decoding failed\n");
    }

    // Clean up
    FLAC__stream_decoder_finish(DECODER);
    FLAC__stream_decoder_delete(DECODER);
}


void main()
{
    const char *test_file = "./music/bell.flac";

    init_alsa();

    play_flac(test_file);

    exit_alsa();
}
