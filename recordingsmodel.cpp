#include "recordingsmodel.h"

RecordingsModel::RecordingsModel(QObject *parent) : QAbstractTableModel (parent)
{

}

int RecordingsModel::rowCount(const QModelIndex &parent) const {
    return 3;
}

int RecordingsModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

QVariant RecordingsModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        return QString::number(5);
    }
    return QVariant();
}
