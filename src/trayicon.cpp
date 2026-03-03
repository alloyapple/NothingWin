#include "trayicon.h"

#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

TrayIcon::TrayIcon(QObject *parent)
    : QObject(parent)
    , m_guardModeAction(new QAction(this))
    , m_settingsAction(new QAction(this))
    , m_aboutAction(new QAction(this))
    , m_quitAction(new QAction(this))
    , m_guardMode(false)
{
    m_guardModeAction->setText(tr("Guard Mode"));
    m_settingsAction->setText(tr("Settings"));
    m_aboutAction->setText(tr("About"));
    m_quitAction->setText(tr("Quit"));

    m_menu.addAction(m_guardModeAction);
    m_menu.addSeparator();
    m_menu.addAction(m_settingsAction);
    m_menu.addAction(m_aboutAction);
    m_menu.addSeparator();
    m_menu.addAction(m_quitAction);

    m_trayIcon.setContextMenu(&m_menu);

    connect(m_guardModeAction, &QAction::triggered, this, &TrayIcon::onGuardModeActionTriggered);
    connect(m_settingsAction, &QAction::triggered, this, &TrayIcon::onSettingsActionTriggered);
    connect(m_aboutAction, &QAction::triggered, this, &TrayIcon::onAboutActionTriggered);
    connect(m_quitAction, &QAction::triggered, this, &TrayIcon::onQuitActionTriggered);
    connect(&m_trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::onTrayIconActivated);

    updateIcon();
    m_trayIcon.show();
}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::setGuardMode(bool armed)
{
    m_guardMode = armed;
    updateIcon();
    updateMenu();
}

bool TrayIcon::isGuardMode() const
{
    return m_guardMode;
}

void TrayIcon::showMessage(const QString &title, const QString &message)
{
    m_trayIcon.showMessage(title, message);
}

void TrayIcon::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        emit guardModeToggled(!m_guardMode);
    }
}

void TrayIcon::onGuardModeActionTriggered()
{
    emit guardModeToggled(!m_guardMode);
}

void TrayIcon::onSettingsActionTriggered()
{
    emit showSettingsRequested();
}

void TrayIcon::onAboutActionTriggered()
{
    emit showAboutRequested();
}

void TrayIcon::onQuitActionTriggered()
{
    emit quitRequested();
}

void TrayIcon::updateIcon()
{
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    if (m_guardMode) {
        painter.setBrush(QColor(255, 80, 80));
        painter.setPen(QColor(200, 50, 50));
    } else {
        painter.setBrush(QColor(100, 200, 100));
        painter.setPen(QColor(50, 150, 50));
    }

    painter.drawEllipse(4, 4, 24, 24);

    QIcon icon(pixmap);
    m_trayIcon.setIcon(icon);
    m_trayIcon.setToolTip(m_guardMode ? "Guard Mode - Armed" : "NothingHere");
}

void TrayIcon::updateMenu()
{
    m_guardModeAction->setText(m_guardMode ? tr("Disarm Guard Mode") : tr("Arm Guard Mode"));
}
