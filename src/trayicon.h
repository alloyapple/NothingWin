#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

    void setGuardMode(bool armed);
    bool isGuardMode() const;

    void showMessage(const QString &title, const QString &message);

signals:
    void guardModeToggled(bool enabled);
    void showSettingsRequested();
    void showAboutRequested();
    void quitRequested();

private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onGuardModeActionTriggered();
    void onSettingsActionTriggered();
    void onAboutActionTriggered();
    void onQuitActionTriggered();

private:
    void updateIcon();
    void updateMenu();

    QSystemTrayIcon m_trayIcon;
    QMenu m_menu;
    QAction *m_guardModeAction;
    QAction *m_settingsAction;
    QAction *m_aboutAction;
    QAction *m_quitAction;
    bool m_guardMode;
};

#endif
