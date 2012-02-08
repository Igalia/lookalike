#ifndef LOOKALIKEMAIN_H
#define LOOKALIKEMAIN_H

#include <QObject>

class LookAlikeMainPrivate;

class LookAlikeMain : public QObject
{
    Q_OBJECT
public:
    explicit LookAlikeMain(QObject *parent = 0);

signals:

public slots:

private:
    //! Private class instance
    LookAlikeMainPrivate *d_ptr;
    Q_DECLARE_PRIVATE(LookAlikeMain);
    Q_DISABLE_COPY(LookAlikeMain);
};

#endif // LOOKALIKEMAIN_H
