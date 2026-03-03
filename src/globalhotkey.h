#ifndef GLOBALHOTKEY_H
#define GLOBALHOTKEY_H

#include <QObject>
#include <QKeySequence>
#include <QAbstractNativeEventFilter>
#include <windows.h>

class GlobalHotkey : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit GlobalHotkey(QObject *parent = nullptr);
    ~GlobalHotkey();

    bool registerHotkey(const QString &hotkey, HWND hwnd = nullptr);
    void unregisterHotkey(HWND hwnd = nullptr);
    bool isRegistered() const;

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

signals:
    void hotkeyPressed();

private:
    int m_modifiers;
    int m_key;
    bool m_registered;
    HHOOK m_hook;
};

#endif
