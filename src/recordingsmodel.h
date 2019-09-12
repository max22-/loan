#ifndef RECORDINGSMODEL_H
#define RECORDINGSMODEL_H

#include <QAbstractTableModel>
#include <QDir>
#include "jsonfile.h"
#include <QFileSystemWatcher>

class RecordingsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RecordingsModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void update();

private:
    QFileSystemWatcher filesWatcher;
    QList<JsonFile> jsonFiles;
    QDir inbox;
};

#define DATE_COLUMN 0
#define NICKNAME_COLUMN 1
#define AGE_COLUMN 2
#define CITY_COLUMN 3
#define EVALUATION_COLUMN 4

#define LAST_COLUMN 4

#endif // RECORDINGSMODEL_H
