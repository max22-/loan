#include "recordingsmodel.h"
#include "config.h"
#include <QDir>
#include "jsonfile.h"
#include <QDebug>

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

        JsonFile jsonFile(inbox.absoluteFilePath(jsonFilesList.at(index.row())));
        try {
            jsonFile.load();
        } catch (QString s) {
            qCritical() << "Caught an exception : " + s;
            return "Erreur";
        }

        switch(index.column()) {
            case DATE_COLUMN: {
                try {
                    return jsonFile.getTimeStamp().toString("dddd d MMMM yyyy à h:mm:ss");
                } catch (QString s) {
                    qCritical() << "Caught an exception : " + s;
                    return "Erreur";
                }

            }
            case NICKNAME_COLUMN: {
                try {
                    return jsonFile.getNickname();
                } catch (QString s) {
                    qCritical() << "Caught an exception : " + s;
                    return "Erreur";
                }
            }
            case AGE_COLUMN: {
                try {
                    return jsonFile.getAge();
                } catch (QString s) {
                    qCritical() << "Caught an exception : " + s;
                    return "Erreur";
                }
            }
            case CITY_COLUMN: {
                try {
                    return jsonFile.getCity();
                } catch (QString s) {
                    qCritical() << "Caught an exception : " + s;
                    return "Erreur";
                }
            }
            case EVALUATION_COLUMN: {
                try {
                    return jsonFile.getEvaluation();
                } catch (QString s) {
                    qCritical() << "Caught an exception : " + s;
                    return "Erreur";
                }
            }
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
