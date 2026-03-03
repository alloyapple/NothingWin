#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    ~Settings();

    QString hotkey() const;
    void setHotkey(const QString &hotkey);

    QString coverDocument() const;
    void setCoverDocument(const QString &path);

    bool guardModeEnabled() const;
    void setGuardModeEnabled(bool enabled);

    bool startMinimized() const;
    void setStartMinimized(bool minimized);

    bool autoStart() const;
    void setAutoStart(bool enabled);

signals:
    void settingsChanged();

private:
    QSettings m_settings;
};

#endif
