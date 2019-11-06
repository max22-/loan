#include "playlistmodel.h"

PlaylistModel::PlaylistModel(MediaPlayerWrapper& mediaPlayer, QObject *parent) : QAbstractListModel(parent), mediaPlayer(mediaPlayer)
{
    connect(&mediaPlayer, &MediaPlayerWrapper::playlistChanged, [this]() {
        emit dataChanged(createIndex(0,0), createIndex(this->mediaPlayer.playlistLength(), 0));
    });
}


int PlaylistModel::rowCount(const QModelIndex&) const {
    return mediaPlayer.getFiles().length();
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const {
    if(role == Qt::DisplayRole) {
        JsonFile m = mediaPlayer.getFiles().at(index.row());
        return m.getNickname() + ", " + QString::number(m.getAge()) + " ans, de " + m.getCity() + ", le " + m.getTimeStamp().toString("d MMMM yyyy à h:mm:ss");
    }
    return QVariant();
}
