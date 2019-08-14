#ifndef RECORDINGSMODEL_H
#define RECORDINGSMODEL_H

#include <QAbstractTableModel>
#include <QDir>

class RecordingsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RecordingsModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QStringList jsonFilesList;
    QDir inbox;
};

#endif // RECORDINGSMODEL_H
