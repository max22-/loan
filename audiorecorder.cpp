#include "audiorecorder.h"
#include "config.h"
#include <QAudioOutput>
#include <QAudioDeviceInfo>

AudioRecorder::AudioRecorder()
{
    Config& config = Config::getInstance();

    tempAudioFile.setFileName(config.tempAudioFileName());
    clear();

    QAudioFormat format;
    format.setSampleRate(config.sampleRate());
    format.setChannelCount(config.channelCount());
    format.setSampleSize(config.sampleSize());
    format.setCodec(config.audioCodec());
    format.setByteOrder(config.byteOrder());
    format.setSampleType(config.sampleType());
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qDebug("Default format not supported, trying to use the nearest.");
        format = info.nearestFormat(format);
    }

    // Audio Input *************************
    audioInput = new QAudioInput(format);
    audioInput->setNotifyInterval(10);

    connect(audioInput, &QAudioInput::notify, [this]() {
        int ms = static_cast<int>(audioInput->processedUSecs()/1000);
        emit positionChanged(ms);
    });


    // *************************************
    // Audio Output ************************
    audioOutput = new QAudioOutput(format);
    audioOutput->setNotifyInterval(10);

    connect(audioOutput, &QAudioOutput::notify, [this]() {
        int ms = static_cast<int>(audioOutput->processedUSecs()/1000);
        emit positionChanged(ms);
    });

    connect(audioOutput, &QAudioOutput::stateChanged, [this](QAudio::State newState) {
        switch(newState) {
            case QAudio::IdleState:
                stop();
                break;
            case QAudio::StoppedState:
                stop();
                if(audioOutput->error() != QAudio::NoError)
                    throw QString("Error during file paying.");
                break;
            default:
                break;
        }
    });

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, &AudioRecorder::stop);

}

AudioRecorder::~AudioRecorder() {
    if(audioInput != nullptr)
        delete audioInput;
    if(audioOutput != nullptr)
        delete audioOutput;
}

void AudioRecorder::clear() {
    switch (state) {
        case AudioRecorderState::INITIAL:
            break;
        case AudioRecorderState::RECORDING:
            throw QString("Cannot clear while AudioRecorder is recording.");
            break;
        case AudioRecorderState::PLAYING:
            throw  QString("Cannot clear while AudioRecorder is playing.");
            break;
        case AudioRecorderState::IDLE:
            tempAudioFile.remove();
            state = AudioRecorderState::INITIAL;
            emit stateChanged(state);
            emit lengthChanged(Config::getInstance().maxRecordingTimeS()*1000);
            emit positionChanged(0);
            break;
    }
}

void AudioRecorder::startRecording() {
    switch (state) {
        case AudioRecorderState::INITIAL:
            tempAudioFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            audioInput->start(&tempAudioFile);
            timer.start(Config::getInstance().maxRecordingTimeS()*1000);
            state = AudioRecorderState::RECORDING;
            emit stateChanged(state);
            emit lengthChanged(Config::getInstance().maxRecordingTimeS()*1000);
            break;
        case AudioRecorderState::RECORDING:
            throw QString("Cannot record : AudioRecorder is already recording.");
            break;
        case AudioRecorderState::PLAYING:
            throw QString("Cannot record : AudioRecorder is currently playing.");
            break;
        case AudioRecorderState::IDLE:
            throw QString("You have to call AudioRecorder::clear() before recording again.");
            break;

    }
}

void AudioRecorder::startPlaying() {
    switch (state) {
        case AudioRecorderState::INITIAL:
            throw QString("Cannot play : no message has been recorded yet.");
            break;
        case AudioRecorderState::RECORDING:
            throw QString("Cannot play : AudioRecorder is currently recording.");
            break;
        case AudioRecorderState::PLAYING:
            throw QString("Cannot play : AudioRecorder is already playing.");
            break;
        case AudioRecorderState::IDLE:
            tempAudioFile.open(QIODevice::ReadOnly);
            audioOutput->start(&tempAudioFile);
            state = AudioRecorderState::PLAYING;
            emit stateChanged(state);

            break;
    }
}


void AudioRecorder::stop() {
    switch (state) {
        case AudioRecorderState::INITIAL:
            throw QString("Cannot stop : no recording or playing is being run.");
            break;
        case AudioRecorderState::RECORDING:
            timer.stop();
            audioInput->stop();
            tempAudioFile.close();
            emit lengthChanged(static_cast<int>(audioInput->processedUSecs()/1000));
            state=AudioRecorderState::IDLE;
            emit stateChanged(state);
            break;
        case AudioRecorderState::PLAYING:
            audioOutput->stop();
            tempAudioFile.close();
            state=AudioRecorderState::IDLE;
            emit stateChanged(state);
            break;
        case AudioRecorderState::IDLE:
            break;
    }
}
