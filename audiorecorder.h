#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QFile>
#include <QTimer>

enum class AudioRecorderState { INITIAL, RECORDING, PLAYING, IDLE };

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    AudioRecorder();
    ~AudioRecorder();
    void clear();
    void startRecording();
    void startPlaying();
    void stop();

signals:
    void lengthChanged(int newLength);      // length of the recorded message in ms
    void positionChanged(int newPosition);  // position in the audio in ms
    void stateChanged(AudioRecorderState newState);

private:
    void changeState(AudioRecorderState newState);
    QAudioInput *audioInput = nullptr;
    QAudioOutput *audioOutput = nullptr;
    QFile tempAudioFile;
    AudioRecorderState state = AudioRecorderState::IDLE;
    QTimer timer;
};



#endif // AUDIORECORDER_H
