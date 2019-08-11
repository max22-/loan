#include "recordingsmodel.h"

recordingsModel::recordingsModel(QObject *parent) : QAbstractTableModel (parent)
{

}

int recordingsModel::rowCount(const QModelIndex &parent) const {
    return 3;
}

int recordingsModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

QVariant recordingsModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        return QString::number(5);
    }
    return QVariant();
}
