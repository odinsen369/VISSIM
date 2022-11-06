#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif

#include <string>
#include <QVector3D>

class SoundSource
{
public:
    /// Constructor.
    SoundSource(std::string name, bool loop = false, float gain = 1.0);
    ~SoundSource();

    void play();
    void pause();
    void stop();

    /// Loads one given WAVE file.

    bool loadWave(std::string filePath);

    void setPosition(QVector3D newPos);             ///< Sets source position from Vector3.
    QVector3D getPosition() {return mPosition;}     ///< Returns source position as Vector3.
    void setVelocity(QVector3D newVel);             ///< Sets source velocity from Vector3.
    QVector3D getVelocity() {return mVelocity;}     ///< Returns source velocity as Vector3.

private:
    std::string mName;          ///< The name of the sound source (Not used).
    ALuint mSource;             ///< The sound source.
    ALuint mBuffer;             ///< The data buffer.
    QVector3D mPosition;    ///< Vector containing source position.
    QVector3D mVelocity;    ///< Vector containing source velocity.
    bool checkError(std::string name);
};

#endif
