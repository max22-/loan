#include "recordingsmodel.h"
#include "config.h"
#include <QDir>

RecordingsModel::RecordingsModel(QObject *parent) : QAbstractTableModel (parent)
{
    inbox = Config::getInstance().inboxDirectory();
    jsonFilesList = inbox.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);
}

int RecordingsModel::rowCount(const QModelIndex &parent) const {
    return jsonFilesList.length();
}

int RecordingsModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

QVariant RecordingsModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        if(index.column() == 0)
            return jsonFilesList.at(index.row());
        return QString::number(5);
    }
    return QVariant();
}

QVariant RecordingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            switch(section) {
                case 0:
                    return QString("Date");
                case 1:
                    return QString("Pseudonyme");
                case 2:
                    return QString("Âge");
                case 3:
                    return QString("Ville");
                case 4:
                    return QString("Evaluation");
            }
        }
    }
    return QVariant();
}
