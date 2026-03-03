#ifndef PANICBUTTON_H
#define PANICBUTTON_H

#include <QObject>
#include <QString>
#include <QTimer>

class PanicButton : public QObject
{
    Q_OBJECT
public:
    explicit PanicButton(QObject *parent = nullptr);
    ~PanicButton();

    void execute();
    void setCoverDocument(const QString &path);
    QString coverDocument() const;

signals:
    void panicStarted();
    void panicFinished();

private:
    QString m_coverDocument;
    QTimer m_timer;
};

#endif
