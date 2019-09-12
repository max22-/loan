#include "recordingsmodel.h"
#include "config.h"
#include <QDir>
#include "jsonfile.h"
#include <QDebug>

RecordingsModel::RecordingsModel(QObject *parent) : QAbstractTableModel (parent)
{
    inbox = Config::getInstance().inboxDirectory();
    update();
    qDebug() << "Adding " + inbox.absolutePath() + " to the file watcher.";
    filesWatcher.addPath(inbox.absolutePath());
    connect(&filesWatcher, &QFileSystemWatcher::directoryChanged, this, &RecordingsModel::update);
}

int RecordingsModel::rowCount(const QModelIndex &parent) const {
    return jsonFiles.length();
}

int RecordingsModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

QVariant RecordingsModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {

        try {
            switch(index.column()) {
                case DATE_COLUMN:
                    return jsonFiles.at(index.row()).getTimeStamp().toString("dddd d MMMM yyyy à h:mm:ss");
                case NICKNAME_COLUMN:
                    return jsonFiles.at(index.row()).getNickname();
                case AGE_COLUMN:
                    return jsonFiles.at(index.row()).getAge();
                case CITY_COLUMN:
                    return jsonFiles.at(index.row()).getCity();
                case EVALUATION_COLUMN:
                    return jsonFiles.at(index.row()).getEvaluation();
            }
        } catch (QString s) {
            qCritical() << "Caught exception : " + s;
            return "Erreur";
        }

    }
    return QVariant();
}

QVariant RecordingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            switch(section) {
                case DATE_COLUMN:
                    return QString("Date");
                case NICKNAME_COLUMN:
                    return QString("Pseudonyme");
                case AGE_COLUMN:
                    return QString("Âge");
                case CITY_COLUMN:
                    return QString("Ville");
                case EVALUATION_COLUMN:
                    return QString("Evaluation");
            }
        }
    }
    return QVariant();
}

void RecordingsModel::update() {
    beginResetModel();

    QStringList jsonFileNames = inbox.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);
    jsonFiles = QList<JsonFile>();

    QListIterator<QString> i(jsonFileNames);
    while (i.hasNext()) {
        JsonFile j(inbox.absoluteFilePath(i.next()));
        try {
            j.load();
            jsonFiles << j;
        } catch (QString s) {
            qCritical() << "Caught exception : " + s;
        }
    }
    endResetModel();
}
