#include "MusicPlayer.h"
#include <ncurses.h>

using namespace std::chrono_literals;

void MusicPlayer::printPlayPanel() {
  char statusBar[playPanelWidth];

  wattron(playPanel, COLOR_PAIR(Green));

  mvwhline(playPanel, 1, 1, ' ', playPanelWidth - 2);
  _sem.acquire();
  mvwprintw(playPanel, 1, 1, " %s", getMusicName());

  box(playPanel, 0, 0);

  sprintf(statusBar, " %s  %s | %s %3u%% [%02u:%02u/%02u:%02u]",
          _music.isPaused() ? "\uf04b" : "\uf04c",
          _music.isLoop() ? "\U000f0458" : "\U000f0456",
          _music.isMute() ? "\uf026"
                          : (_music.getVolume() > 50 ? "\uf028" : "\uf027"),
          _music.getVolume(), (unsigned)_music.getCurrentTime() / 60,
          (unsigned)_music.getCurrentTime() % 60,
          (unsigned)_music.getTime() / 60, (unsigned)_music.getTime() % 60);
  _sem.release();
  mvwprintw(playPanel, 2, 1, "%s", statusBar);

  wattroff(playPanel, COLOR_PAIR(Green));
  wrefresh(playPanel);
}

void MusicPlayer::printPlayPanelWithThread() {
  threadForPrintPlayPanel = std::thread([this] {
    bool tooSimal = false;
    int currentHighth, currentWidth;

    getmaxyx(stdscr, currentHighth, currentWidth);

    if (currentHighth < playPanelHighth || currentWidth < playPanelWidth)
      tooSimal = true;

    while (!stopPrintThread) {
      std::this_thread::sleep_for(50ms);
      refresh();

      getmaxyx(stdscr, currentHighth, currentWidth);
      if (currentHighth != screenHighth || currentWidth != screenWidth) {
        getmaxyx(stdscr, screenHighth, screenWidth);
        if (screenHighth < playPanelHighth || screenWidth < playPanelWidth) {
          tooSimal = true;
        } else {
          tooSimal = false;
          updatePlayPanelPosition();
        }
      }

      if (tooSimal) {
        clear();
        mvprintw(0, 0, "The size of terminal is too small.");
        refresh();
      } else {
        printPlayPanel();
      }

      if ((int)_music.getTime() == (int)_music.getCurrentTime() &&
          !_music.isPaused() && !_music.isLoop()) {
        nextMusic();
      }
    }
  });
}

void MusicPlayer::caculatePlayPanelPosition() {
  getmaxyx(stdscr, screenHighth, screenWidth);
  playPanelStarty = (screenHighth - playPanelHighth) / 2;
  playPanelStartx = (screenWidth - playPanelWidth) / 2;
}

void MusicPlayer::initializePlayPanel() {
  caculatePlayPanelPosition();
  playPanel =
      newwin(playPanelHighth, playPanelWidth, playPanelStarty, playPanelStartx);
}

void MusicPlayer::updatePlayPanelPosition() {
  clear();
  refresh();
  caculatePlayPanelPosition();
  wresize(playPanel, playPanelHighth, playPanelWidth);
  mvwin(playPanel, playPanelStarty, playPanelStartx);
}
