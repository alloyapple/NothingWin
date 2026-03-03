#include "panicbutton.h"
#include "platform/windows/hidewindows.h"
#include "platform/windows/systemvolume.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDebug>

PanicButton::PanicButton(QObject *parent)
    : QObject(parent)
{
}

PanicButton::~PanicButton()
{
}

void PanicButton::execute()
{
    qDebug() << "Executing panic action...";
    emit panicStarted();

    hideAllWindows();
    muteSystem();
    pauseMedia();

    if (!m_coverDocument.isEmpty()) {
        QFileInfo file(m_coverDocument);
        if (file.exists()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_coverDocument));
        }
    }

    emit panicFinished();
    qDebug() << "Panic action completed";
}

void PanicButton::setCoverDocument(const QString &path)
{
    m_coverDocument = path;
}

QString PanicButton::coverDocument() const
{
    return m_coverDocument;
}
