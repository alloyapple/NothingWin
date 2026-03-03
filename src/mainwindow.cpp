#include "mainwindow.h"
#include "settings.h"
#include "panicbutton.h"
#include "trayicon.h"
#include "globalhotkey.h"
#include "platform/windows/keyboardhook.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QAbstractEventDispatcher>
#include <QFrame>
#include <QLabel>

static MainWindow *s_instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_trayIcon(nullptr)
    , m_hotkey(nullptr)
    , m_keyboardHook(nullptr)
    , m_settingsWidget(nullptr)
    , m_hotkeyEdit(nullptr)
    , m_coverDocEdit(nullptr)
    , m_browseBtn(nullptr)
    , m_guardMode(false)
{
    s_instance = this;
    setWindowTitle("NothingWin");
    resize(650, 550);
}

MainWindow::~MainWindow()
{
    if (m_keyboardHook) {
        m_keyboardHook->uninstall();
        delete m_keyboardHook;
    }
    delete m_hotkey;
    delete m_trayIcon;
}

void MainWindow::init()
{
    qDebug() << "Initializing NothingWin...";

    m_trayIcon = new TrayIcon(this);
    m_hotkey = new GlobalHotkey(this);
    m_keyboardHook = new KeyboardHook();

    QAbstractEventDispatcher::instance()->installNativeEventFilter(m_hotkey);

    m_keyboardHook->install();
    KeyboardHook::setCallback([]() {
        if (s_instance && s_instance->m_guardMode) {
            s_instance->onHotkeyPressed();
        }
    });

    connect(m_hotkey, &GlobalHotkey::hotkeyPressed, this, &MainWindow::onHotkeyPressed);
    connect(m_trayIcon, &TrayIcon::guardModeToggled, this, &MainWindow::onGuardModeToggled);
    connect(m_trayIcon, &TrayIcon::showSettingsRequested, this, &MainWindow::onShowSettings);
    connect(m_trayIcon, &TrayIcon::showAboutRequested, this, &MainWindow::onShowAbout);
    connect(m_trayIcon, &TrayIcon::quitRequested, this, &MainWindow::onQuit);

    loadSettings();

    qDebug() << "NothingWin initialized";
}

void MainWindow::onHotkeyPressed()
{
    qDebug() << "Hotkey pressed, executing panic...";
    PanicButton pb;
    Settings s;
    pb.setCoverDocument(s.coverDocument());
    pb.execute();

    if (m_guardMode) {
        setGuardMode(false);
    }
}

void MainWindow::onGuardModeToggled(bool enabled)
{
    setGuardMode(enabled);
}

void MainWindow::onShowSettings()
{
    if (!m_settingsWidget) {
        setupUi();
    }
    loadSettings();
    show();
    raise();
    activateWindow();
}

void MainWindow::onShowAbout()
{
    QMessageBox::about(this, tr("About NothingWin"),
        tr("NothingWin v1.0\n\n") +
        tr("A panic button for Windows.\n\n") +
        tr("Press the hotkey to hide all windows, mute system, and open a cover document."));
}

void MainWindow::onQuit()
{
    QApplication::quit();
}

void MainWindow::onSelectCoverDocument()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Cover Document"),
        QString(), tr("All Files (*)"));
    if (!file.isEmpty()) {
        m_coverDocEdit->setText(file);
    }
}

void MainWindow::onSaveSettings()
{
    Settings s;
    s.setHotkey(m_hotkeyEdit->text());
    s.setCoverDocument(m_coverDocEdit->text());

    m_hotkey->registerHotkey(s.hotkey());

    hide();
}

void MainWindow::setupUi()
{
    m_settingsWidget = new QWidget();
    setCentralWidget(m_settingsWidget);
    setWindowTitle(tr("Settings - NothingWin"));
    resize(500, 400);

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(m_settingsWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Header
    QLabel *titleLabel = new QLabel(tr("Settings"));
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #333; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel(tr("Configure your panic button preferences"));
    subtitleLabel->setStyleSheet("font-size: 15px; color: #666; margin-bottom: 20px;");
    mainLayout->addWidget(subtitleLabel);

    // Hotkey section
    QFrame *hotkeyFrame = new QFrame();
    hotkeyFrame->setStyleSheet("QFrame { background: #f8f9fa; border-radius: 10px; padding: 20px; }");
    QVBoxLayout *hotkeyLayout = new QVBoxLayout(hotkeyFrame);
    hotkeyLayout->setSpacing(10);

    QLabel *hotkeyLabel = new QLabel(tr("Panic Hotkey"));
    hotkeyLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #333;");
    hotkeyLayout->addWidget(hotkeyLabel);

    QLabel *hotkeyDesc = new QLabel(tr("Press this key combination to instantly hide everything"));
    hotkeyDesc->setStyleSheet("font-size: 14px; color: #888;");
    hotkeyDesc->setWordWrap(true);
    hotkeyLayout->addWidget(hotkeyDesc);

    m_hotkeyEdit = new QLineEdit();
    m_hotkeyEdit->setText("Ctrl+Alt+H");
    m_hotkeyEdit->setStyleSheet(
        "QLineEdit { "
        "  background: white; border: 2px solid #e0e0e0; border-radius: 8px; "
        "  padding: 14px 18px; font-size: 16px; color: #333; "
        "} "
        "QLineEdit:focus { border-color: #4CAF50; }"
    );
    hotkeyLayout->addWidget(m_hotkeyEdit);

    mainLayout->addWidget(hotkeyFrame);

    // Cover document section
    QFrame *coverFrame = new QFrame();
    coverFrame->setStyleSheet("QFrame { background: #f8f9fa; border-radius: 10px; padding: 20px; }");
    QVBoxLayout *coverLayout = new QVBoxLayout(coverFrame);
    coverLayout->setSpacing(10);

    QLabel *coverLabel = new QLabel(tr("Cover Document"));
    coverLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #333;");
    coverLayout->addWidget(coverLabel);

    QLabel *coverDesc = new QLabel(tr("This document will open when panic is triggered"));
    coverDesc->setStyleSheet("font-size: 14px; color: #888;");
    coverDesc->setWordWrap(true);
    coverLayout->addWidget(coverDesc);

    QHBoxLayout *coverInputLayout = new QHBoxLayout();
    coverInputLayout->setSpacing(10);

    m_coverDocEdit = new QLineEdit();
    m_coverDocEdit->setStyleSheet(
        "QLineEdit { "
        "  background: white; border: 2px solid #e0e0e0; border-radius: 8px; "
        "  padding: 14px 18px; font-size: 16px; color: #333; "
        "} "
        "QLineEdit:focus { border-color: #4CAF50; }"
    );
    m_coverDocEdit->setPlaceholderText(tr("Select a file to use as cover..."));
    coverInputLayout->addWidget(m_coverDocEdit);

    m_browseBtn = new QPushButton(tr("Browse"));
    m_browseBtn->setStyleSheet(
        "QPushButton { "
        "  background: #4CAF50; color: white; border: none; border-radius: 8px; "
        "  padding: 14px 24px; font-size: 16px; font-weight: 600; "
        "} "
        "QPushButton:hover { background: #45a049; } "
        "QPushButton:pressed { background: #3d8b40; }"
    );
    m_browseBtn->setCursor(Qt::PointingHandCursor);
    coverInputLayout->addWidget(m_browseBtn);

    coverLayout->addLayout(coverInputLayout);
    mainLayout->addWidget(coverFrame);

    // Button section
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    QPushButton *cancelBtn = new QPushButton(tr("Cancel"));
    cancelBtn->setStyleSheet(
        "QPushButton { "
        "  background: #f5f5f5; color: #666; border: 2px solid #e0e0e0; border-radius: 8px; "
        "  padding: 14px 28px; font-size: 16px; font-weight: 600; "
        "} "
        "QPushButton:hover { background: #e8e8e8; } "
        "QPushButton:pressed { background: #ddd; }"
    );
    cancelBtn->setCursor(Qt::PointingHandCursor);
    btnLayout->addWidget(cancelBtn);

    btnLayout->addStretch();

    QPushButton *saveBtn = new QPushButton(tr("Save Settings"));
    saveBtn->setStyleSheet(
        "QPushButton { "
        "  background: #4CAF50; color: white; border: none; border-radius: 8px; "
        "  padding: 14px 36px; font-size: 16px; font-weight: 600; "
        "} "
        "QPushButton:hover { background: #45a049; } "
        "QPushButton:pressed { background: #3d8b40; }"
    );
    saveBtn->setCursor(Qt::PointingHandCursor);
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);

    // Guard Mode info
    QFrame *guardFrame = new QFrame();
    guardFrame->setStyleSheet(
        "QFrame { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "    stop:0 #e8f5e9, stop:1 #c8e6c9); "
        "  border-radius: 10px; padding: 20px; "
        "}"
    );
    QVBoxLayout *guardLayout = new QVBoxLayout(guardFrame);
    guardLayout->setSpacing(6);

    QLabel *guardTitle = new QLabel(tr("Guard Mode"));
    guardTitle->setStyleSheet("font-size: 18px; font-weight: 600; color: #2e7d32;");
    guardLayout->addWidget(guardTitle);

    QLabel *guardDesc = new QLabel(tr("When enabled, pressing any key will trigger the panic action. "
        "Right-click the tray icon and select 'Arm Guard Mode' to enable."));
    guardDesc->setStyleSheet("font-size: 14px; color: #388e3c;");
    guardDesc->setWordWrap(true);
    guardLayout->addWidget(guardDesc);

    mainLayout->addWidget(guardFrame);

    connect(m_browseBtn, &QPushButton::clicked, this, &MainWindow::onSelectCoverDocument);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveSettings);
    connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::hide);
}

void MainWindow::loadSettings()
{
    Settings s;
    if (m_hotkeyEdit) {
        m_hotkeyEdit->setText(s.hotkey());
    }
    if (m_coverDocEdit) {
        m_coverDocEdit->setText(s.coverDocument());
    }
    m_hotkey->registerHotkey(s.hotkey(), reinterpret_cast<HWND>(winId()));

    m_guardMode = s.guardModeEnabled();
    m_trayIcon->setGuardMode(m_guardMode);

    if (m_guardMode) {
        m_keyboardHook->install();
    } else {
        m_keyboardHook->uninstall();
    }
}

void MainWindow::setGuardMode(bool enabled)
{
    m_guardMode = enabled;
    Settings s;
    s.setGuardModeEnabled(enabled);
    m_trayIcon->setGuardMode(enabled);

    if (enabled) {
        m_keyboardHook->install();
        m_trayIcon->showMessage("NothingWin", "Guard Mode enabled - Press any key to trigger panic!");
    } else {
        m_keyboardHook->uninstall();
        m_trayIcon->showMessage("NothingWin", "Guard Mode disabled");
    }
}
