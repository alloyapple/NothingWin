#include "globalhotkey.h"

#include <QDebug>
#include <QKeySequence>
#include <QAbstractEventDispatcher>

GlobalHotkey::GlobalHotkey(QObject *parent)
    : QObject(parent)
    , m_modifiers(0)
    , m_key(0)
    , m_registered(false)
    , m_hook(nullptr)
{
}

GlobalHotkey::~GlobalHotkey()
{
    unregisterHotkey();
}

bool GlobalHotkey::registerHotkey(const QString &hotkey)
{
    unregisterHotkey();

    QKeySequence seq(hotkey);
    if (seq.isEmpty()) {
        return false;
    }

    int key = seq[0] & ~Qt::KeyboardModifierMask;
    m_modifiers = seq[0] & Qt::KeyboardModifierMask;

    m_key = 0;
    switch (key) {
    case Qt::Key_A: m_key = 'A'; break;
    case Qt::Key_B: m_key = 'B'; break;
    case Qt::Key_C: m_key = 'C'; break;
    case Qt::Key_D: m_key = 'D'; break;
    case Qt::Key_E: m_key = 'E'; break;
    case Qt::Key_F: m_key = 'F'; break;
    case Qt::Key_G: m_key = 'G'; break;
    case Qt::Key_H: m_key = 'H'; break;
    case Qt::Key_I: m_key = 'I'; break;
    case Qt::Key_J: m_key = 'J'; break;
    case Qt::Key_K: m_key = 'K'; break;
    case Qt::Key_L: m_key = 'L'; break;
    case Qt::Key_M: m_key = 'M'; break;
    case Qt::Key_N: m_key = 'N'; break;
    case Qt::Key_O: m_key = 'O'; break;
    case Qt::Key_P: m_key = 'P'; break;
    case Qt::Key_Q: m_key = 'Q'; break;
    case Qt::Key_R: m_key = 'R'; break;
    case Qt::Key_S: m_key = 'S'; break;
    case Qt::Key_T: m_key = 'T'; break;
    case Qt::Key_U: m_key = 'U'; break;
    case Qt::Key_V: m_key = 'V'; break;
    case Qt::Key_W: m_key = 'W'; break;
    case Qt::Key_X: m_key = 'X'; break;
    case Qt::Key_Y: m_key = 'Y'; break;
    case Qt::Key_Z: m_key = 'Z'; break;
    case Qt::Key_F1: m_key = VK_F1; break;
    case Qt::Key_F2: m_key = VK_F2; break;
    case Qt::Key_F3: m_key = VK_F3; break;
    case Qt::Key_F4: m_key = VK_F4; break;
    case Qt::Key_F5: m_key = VK_F5; break;
    case Qt::Key_F6: m_key = VK_F6; break;
    case Qt::Key_F7: m_key = VK_F7; break;
    case Qt::Key_F8: m_key = VK_F8; break;
    case Qt::Key_F9: m_key = VK_F9; break;
    case Qt::Key_F10: m_key = VK_F10; break;
    case Qt::Key_F11: m_key = VK_F11; break;
    case Qt::Key_F12: m_key = VK_F12; break;
    default: return false;
    }

    int winMod = 0;
    if (m_modifiers & Qt::ControlModifier) winMod |= MOD_CONTROL;
    if (m_modifiers & Qt::AltModifier) winMod |= MOD_ALT;
    if (m_modifiers & Qt::ShiftModifier) winMod |= MOD_SHIFT;
    if (m_modifiers & Qt::MetaModifier) winMod |= MOD_WIN;

    m_registered = RegisterHotKey(nullptr, 1, winMod, m_key);
    qDebug() << "Hotkey registered:" << hotkey << "Key:" << m_key << "Modifiers:" << m_modifiers << "Success:" << m_registered;
    return m_registered;
}

void GlobalHotkey::unregisterHotkey()
{
    if (m_registered) {
        UnregisterHotKey(nullptr, 1);
    }
    m_registered = false;
    m_key = 0;
    m_modifiers = 0;
}

bool GlobalHotkey::isRegistered() const
{
    return m_registered;
}

bool GlobalHotkey::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG *msg = static_cast<MSG *>(message);
    if (msg->message == WM_HOTKEY) {
        qDebug() << "Hotkey pressed!";
        emit hotkeyPressed();
        return true;
    }
    return false;
}
