#ifndef LOGGER_H
#define LOGGER_H

#include <QtGlobal>

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOGGER_H
