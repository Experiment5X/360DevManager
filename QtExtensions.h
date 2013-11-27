#ifndef QTEXTENSIONS_H
#define QTEXTENSIONS_H

#include <QStandardPaths>

#define qs(s) (QString::fromStdString(s))
#define qhex(n) ("0x" + QString::number(n, 16).toUpper())

#define DESKTOP_LOCATION (QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).replace("\\", "/") + "/")

#endif // QTEXTENSIONS_H
