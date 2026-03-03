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
    setWindowTitle("NothingHere");
    resize(400, 300);
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
    qDebug() << "Initializing NothingHere...";

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

    qDebug() << "NothingHere initialized";
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
    QMessageBox::about(this, tr("About NothingHere"),
        tr("NothingHere v1.0\n\n") +
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

    QVBoxLayout *mainLayout = new QVBoxLayout(m_settingsWidget);

    QLabel *hotkeyLabel = new QLabel(tr("Hotkey:"), this);
    m_hotkeyEdit = new QLineEdit(this);
    m_hotkeyEdit->setText("Ctrl+Alt+H");

    QHBoxLayout *hotkeyLayout = new QHBoxLayout();
    hotkeyLayout->addWidget(hotkeyLabel);
    hotkeyLayout->addWidget(m_hotkeyEdit);

    QLabel *coverLabel = new QLabel(tr("Cover Document:"), this);
    m_coverDocEdit = new QLineEdit(this);
    m_browseBtn = new QPushButton(tr("Browse..."), this);

    QHBoxLayout *coverLayout = new QHBoxLayout();
    coverLayout->addWidget(m_coverDocEdit);
    coverLayout->addWidget(m_browseBtn);

    QPushButton *saveBtn = new QPushButton(tr("Save"), this);
    QPushButton *cancelBtn = new QPushButton(tr("Cancel"), this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->addLayout(hotkeyLayout);
    mainLayout->addLayout(coverLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLayout);

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
        m_trayIcon->showMessage("NothingHere", "Guard Mode enabled - Press any key to trigger panic!");
    } else {
        m_keyboardHook->uninstall();
        m_trayIcon->showMessage("NothingHere", "Guard Mode disabled");
    }
}
