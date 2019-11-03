#include "mediaplayerwrapper.h"
#include "config.h"

MediaPlayerWrapper::MediaPlayerWrapper(QObject *parent) : QObject(parent), index(0)
{
    player.setNotifyInterval(10);
    connect(&player, &QMediaPlayer::stateChanged, this, &MediaPlayerWrapper::nextMessage);
    connect(&player, &QMediaPlayer::durationChanged, [this](qint64 duration) {
        emit durationChanged(duration);
    });
    connect(&player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        emit positionChanged(position);
    });
}

void MediaPlayerWrapper::add(const JsonFile& file) {
    qDebug() << "Adding " + Config::getInstance().inboxDirectory().absoluteFilePath(file.getMP3FileName()) + " to the playlist.";
    files.append(file);
}

QList<JsonFile> MediaPlayerWrapper::getFiles() {
    return files;
}

void MediaPlayerWrapper::play(int messageNumber) {
    index = messageNumber;
    if(index > files.length()-1) {
        qWarning() << "Trying to play a file, but the playlist is empty.";
        return;
    }
    playMessage(messageNumber);

}

void MediaPlayerWrapper::togglePause() {
    switch(player.state()) {
        case QMediaPlayer::PlayingState:
            qDebug() << "Pausing the media player.";
            player.pause();
            break;
        case QMediaPlayer::PausedState:
            qDebug() << "Resuming the media player.";
            player.play();
            break;
        default:
            break;
    }
}

void MediaPlayerWrapper::stop() {
    switch(player.state()) {
        case QMediaPlayer::PlayingState:
        case QMediaPlayer::PausedState:
            qDebug() << "Stopping the media player.";
            player.stop();
            emit playerStopped();
            break;
        default:
            break;
    }
}

void MediaPlayerWrapper::clear() {
    qDebug() << "Clearing the playlist.";
    stop();
    files = QList<JsonFile>();
}

void MediaPlayerWrapper::setVolume(int volume) {
    player.setVolume(volume);
}

void MediaPlayerWrapper::nextMessage(QMediaPlayer::State mpstate) {
    if(mpstate == QMediaPlayer::StoppedState) {
        index++;
        if(index > files.length() - 1) {
            emit playerStopped();
            qDebug("Finished playing the messages in the playlist");
        }
        else playMessage(index);
    }
}

void MediaPlayerWrapper::playMessage(const int messageNumber) {
    QString absoluteMP3FilePath = Config::getInstance().inboxDirectory().absoluteFilePath(files.at(messageNumber).getMP3FileName());
    qDebug() << "Playing " + absoluteMP3FilePath;
    player.setMedia(QUrl::fromLocalFile(absoluteMP3FilePath));
    emit playingNewMessage(messageNumber);
    player.play();
}
