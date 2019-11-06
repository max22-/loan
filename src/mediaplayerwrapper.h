#ifndef MEDIAPLAYERWRAPPER_H
#define MEDIAPLAYERWRAPPER_H

#include <QObject>
#include <QList>
#include <QMediaPlayer>
#include "jsonfile.h"

class MediaPlayerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayerWrapper(QObject *parent = nullptr);
    void add(const JsonFile&);
    QList<JsonFile> getFiles() const;
    int playlistLength() const;

signals:
    void playingNewMessage(int messageNumber);
    void playerStopped();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void playlistChanged();

public slots:
    void play(int messageNumber = 0);
    void togglePause();
    void stop();
    void clear();
    void setVolume(int volume);

private slots:
    void nextMessage(QMediaPlayer::State mpstate);

private:
    void playMessage(const int messageNumber);
    QMediaPlayer player;
    QList<JsonFile> files;
    int index;
};

#endif // MEDIAPLAYERWRAPPER_H
