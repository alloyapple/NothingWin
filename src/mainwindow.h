#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

class TrayIcon;
class GlobalHotkey;
class KeyboardHook;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private slots:
    void onHotkeyPressed();
    void onGuardModeToggled(bool enabled);
    void onShowSettings();
    void onShowAbout();
    void onQuit();
    void onSelectCoverDocument();
    void onSaveSettings();

private:
    void setupUi();
    void setupConnections();
    void loadSettings();
    void setGuardMode(bool enabled);

    TrayIcon *m_trayIcon;
    GlobalHotkey *m_hotkey;
    KeyboardHook *m_keyboardHook;

    QWidget *m_settingsWidget;
    QLineEdit *m_hotkeyEdit;
    QLineEdit *m_coverDocEdit;
    QPushButton *m_browseBtn;
    bool m_guardMode;
};

#endif
