#ifndef FACEAUTORECOGNIZERMAIN_H
#define FACEAUTORECOGNIZERMAIN_H

#include <QObject>

class FaceAutorecognizerMainPrivate;

class FaceAutorecognizerMain : public QObject
{
    Q_OBJECT
public:
    explicit FaceAutorecognizerMain(QObject *parent = 0);

signals:

public slots:

private:
    //! Private class instance
    FaceAutorecognizerMainPrivate *d_ptr;
    Q_DECLARE_PRIVATE(FaceAutorecognizerMain);
    Q_DISABLE_COPY(FaceAutorecognizerMain);
};

#endif // FACEAUTORECOGNIZERMAIN_H
