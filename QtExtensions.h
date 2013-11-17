#ifndef QTEXTENSIONS_H
#define QTEXTENSIONS_H

#define qs(s) (QString::fromStdString(s))
#define qhex(n) ("0x" + QString::number(n, 16).toUpper())

#endif // QTEXTENSIONS_H
