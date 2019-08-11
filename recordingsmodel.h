#ifndef RECORDINGSMODEL_H
#define RECORDINGSMODEL_H

#include <QAbstractTableModel>

class recordingsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    recordingsModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // RECORDINGSMODEL_H
