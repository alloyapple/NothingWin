#include "systemvolume.h"

#include <windows.h>
#include <QDebug>

void muteSystem()
{
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    qDebug() << "System muted";
}

void unmuteSystem()
{
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void pauseMedia()
{
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    qDebug() << "Media paused";
}
