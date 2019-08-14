#include "audiorecorder.h"
#include "config.h"
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QDebug>
#include <QProcess>

AudioRecorder::AudioRecorder()
{
    Config& config = Config::getInstance();

    tempAudioFile.setFileName(config.tempAudioFileName());
    tempMP3File.setFileName(config.tempMP3FileName());
    clear();

    format.setSampleRate(config.desiredSampleRate());
    format.setChannelCount(config.desiredChannelCount());
    format.setSampleSize(config.desiredSampleSize());
    format.setCodec(config.desiredAudioCodec());
    format.setByteOrder(config.desiredByteOrder());
    format.setSampleType(config.desiredSampleType());
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qDebug() << "Desired format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
        qDebug() << "Using sampleRate =" << format.sampleRate();
        qDebug() << "Using channelCount =" << format.channelCount();
        qDebug() << "Using sampleSize =" << format.sampleSize();
        qDebug() << "Using codec =" << format.codec();
        qDebug() << "Using byteOrder =" << format.byteOrder();
        qDebug() << "Using sampleType =" << format.sampleType();
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
    delete audioInput;
    delete audioOutput;
}

void AudioRecorder::changeState(AudioRecorderState newState) {
    state = newState;
    emit stateChanged(state);
}

void AudioRecorder::clear() {
    qDebug() << "AudioRecorder::clear()";
    switch (state) {
        case AudioRecorderState::INITIAL:
            break;
        case AudioRecorderState::RECORDING:
            throw QString("Cannot clear while AudioRecorder is recording.");
        case AudioRecorderState::PLAYING:
            throw  QString("Cannot clear while AudioRecorder is playing.");
        case AudioRecorderState::IDLE:
        case AudioRecorderState::MP3STATE:
            tempAudioFile.remove();
            tempMP3File.remove();
            changeState(AudioRecorderState::INITIAL);
            emit lengthChanged(Config::getInstance().maxRecordingTimeS()*1000);
            emit positionChanged(0);
            break;





    }
}

void AudioRecorder::startRecording() {
    qDebug() << "AudioRecorder::startRecording()";
    switch (state) {
        case AudioRecorderState::INITIAL:
            tempAudioFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            audioInput->start(&tempAudioFile);
            timer.start(Config::getInstance().maxRecordingTimeS()*1000);
            changeState(AudioRecorderState::RECORDING);
            emit lengthChanged(Config::getInstance().maxRecordingTimeS()*1000);
            break;
        case AudioRecorderState::RECORDING:
            throw QString("Cannot record : AudioRecorder is already recording.");
        case AudioRecorderState::PLAYING:
            throw QString("Cannot record : AudioRecorder is currently playing.");
        case AudioRecorderState::IDLE:
            throw QString("You have to call AudioRecorder::clear() before recording again.");
        case AudioRecorderState::MP3STATE:
            throw QString("Cannot record when a file has been converted to MP3. Call AudioRecorder::clear() first.");
    }
}

void AudioRecorder::startPlaying() {
    qDebug() << "AudioRecorder::startPlaying()";
    switch (state) {
        case AudioRecorderState::INITIAL:
            throw QString("Cannot play : no message has been recorded yet.");
        case AudioRecorderState::RECORDING:
            throw QString("Cannot play : AudioRecorder is currently recording.");
        case AudioRecorderState::PLAYING:
            throw QString("Cannot play : AudioRecorder is already playing.");
        case AudioRecorderState::IDLE:
            tempAudioFile.open(QIODevice::ReadOnly);
            audioOutput->start(&tempAudioFile);
            changeState(AudioRecorderState::PLAYING);
            break;
        case AudioRecorderState::MP3STATE:
            throw QString("Cannot play a file converted to MP3.");
    }
}


void AudioRecorder::stop() {
    qDebug() << "AudioRecorder::stop()";
    switch (state) {
        case AudioRecorderState::INITIAL:
            throw QString("Cannot stop : no recording or playing is being run.");
        case AudioRecorderState::RECORDING:
            timer.stop();
            audioInput->stop();
            tempAudioFile.close();
            emit lengthChanged(static_cast<int>(audioInput->processedUSecs()/1000));
            changeState(AudioRecorderState::IDLE);
            break;
        case AudioRecorderState::PLAYING:
            audioOutput->stop();
            tempAudioFile.close();
            changeState(AudioRecorderState::IDLE);
            break;
        case AudioRecorderState::IDLE:
            break;
        case AudioRecorderState::MP3STATE:
            break;
    }
}

void AudioRecorder::convertToMP3() {
    qDebug() << "AudioRecorder::convertToMP3()";
    switch(state) {
        case AudioRecorderState::INITIAL:
            throw QString("Cannot convert to MP3 : no recording has been made.");
        case AudioRecorderState::RECORDING:
            throw QString("Cannot convert to MP3 while recording.");
        case AudioRecorderState::PLAYING:
            throw QString("Cannot convert to MP3 while playing.");
        case AudioRecorderState::IDLE: {
            int retCode = QProcess::execute(Config::getInstance().MP3ConversionCommand(format));
            if(retCode != 0)
                throw("MP3 conversion failed. retCode = " + QString::number(retCode));
            if(!QFile(Config::getInstance().tempMP3FileName()).exists())
                throw("MP3 conversion failed : no MP3 file has been produced.");
            changeState(AudioRecorderState::MP3STATE);
            break; }
        case AudioRecorderState::MP3STATE:
            throw QString("File already converted to MP3.");
    }
}
