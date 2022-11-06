#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <string>
#include <vector>
#include "soundsource.h"
#include <QVector3D>

/// Class handling OpenAL setup and management of sound sources.

class SoundManager
{
public:
    struct SoundListener
    {
        QVector3D pos;
        QVector3D vel;
        QVector3D dir;
        QVector3D up;
        SoundListener()
        {
            pos = QVector3D(0.0f, 0.0f, 0.0f);
            vel = QVector3D(0.0f, 0.0f, 0.0f);
            dir = QVector3D(0.0f, 0.0f, 1.0f);
            up = QVector3D(0.0f, 1.0f, 0.0f);
        }
    };

    static SoundManager* getInstance()
    {
        if (!mInstance)
            mInstance = new SoundManager();
        return mInstance;
    }

    bool init();
    void cleanUp();

    /// Creates a new SoundSource with the given parameters.
    SoundSource* createSource(std::string name, QVector3D pos, std::string filePath = "", bool loop = false, float gain = 1.0);
    void updateListener(QVector3D pos, QVector3D vel, QVector3D dir, QVector3D up);

private:
    SoundManager();                         ///< Private constructor.
    SoundManager(SoundManager const&);      ///< Private copy constructor.
    void operator=(SoundManager const&);    ///< Private class assignment operator.
    static SoundManager* mInstance;         ///< Singleton instance pointer.

    /// Debug function.
    bool checkError();

    ALCdevice* mDevice{nullptr};                 ///< Pointer to the ALC Device.
    ALCcontext* mContext{nullptr};               ///< Pointer to the ALC Context.

    SoundListener mListener;               ///< The listener data structure. (Temporary)
};

#endif
