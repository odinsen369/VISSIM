#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <cstdint>
#include <string>
#include <cstdio>

/// Structure to contain WAVE file data.
struct wave_t
{
    uint32_t size;              ///< Size of the WAVE file.
    uint32_t chunkSize;         ///< Size of the fmt chunk.
    uint32_t sampleRate;        ///< Sample rate of the audio.
    uint32_t avgBytesPerSec;    ///< Average byte rate.
    uint32_t dataSize;          ///< Size of the data chunk.
    short formatType;           ///< Audio format.
    short channels;             ///< Number of audio channels.
    short bytesPerSample;       ///< Number of bytes per sample.
    short bitsPerSample;        ///< Number of bits per sample.

    unsigned char* buffer {nullptr};      ///< Buffer to hold audio data.
    wave_t() : buffer(NULL) {}  ///< Structure Constructor. Initialises data buffer to NULL.
};

/// Class to handle loading of Microsoft signed 16-bit PCM WAVE audio files.
class WavFileHandler
{
public:
    /// Loads a given wave file, fills wave_t structure with wave data.
    static bool loadWave(std::string filePath, wave_t* wavePtr);

private:
    /// Error handling function.
    static bool endOnError(std::string errmsg);
};

#endif
