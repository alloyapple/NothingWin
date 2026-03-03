#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_settings("NothingWin", "Settings")
{
}

Settings::~Settings()
{
}

QString Settings::hotkey() const
{
    return m_settings.value("hotkey", "Ctrl+Alt+H").toString();
}

void Settings::setHotkey(const QString &hotkey)
{
    m_settings.setValue("hotkey", hotkey);
    emit settingsChanged();
}

QString Settings::coverDocument() const
{
    return m_settings.value("coverDocument").toString();
}

void Settings::setCoverDocument(const QString &path)
{
    m_settings.setValue("coverDocument", path);
    emit settingsChanged();
}

bool Settings::guardModeEnabled() const
{
    return m_settings.value("guardModeEnabled", false).toBool();
}

void Settings::setGuardModeEnabled(bool enabled)
{
    m_settings.setValue("guardModeEnabled", enabled);
    emit settingsChanged();
}

bool Settings::startMinimized() const
{
    return m_settings.value("startMinimized", true).toBool();
}

void Settings::setStartMinimized(bool minimized)
{
    m_settings.setValue("startMinimized", minimized);
}

bool Settings::autoStart() const
{
    return m_settings.value("autoStart", false).toBool();
}

void Settings::setAutoStart(bool enabled)
{
    m_settings.setValue("autoStart", enabled);
}
