#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include "mediaplayerwrapper.h"


class PlaylistModel : public QAbstractListModel
{
public:
    PlaylistModel(MediaPlayerWrapper& mediaPlayer,  QObject *parent);
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    const MediaPlayerWrapper& mediaPlayer;

};

#endif // PLAYLISTMODEL_H
