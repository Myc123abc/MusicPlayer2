#include "MusicPlayer.h"
#include <panel.h>

void MusicPlayer::caculateListPanelInformation() {
  getmaxyx(stdscr, screenHighth, screenWidth);
  listPanelHighth = screenHighth / 8 * 6;
  listPanelWidth = screenWidth / 2;
  listPanelStarty = (screenHighth - listPanelHighth) / 2;
  listPanelStartx = (screenWidth - listPanelWidth) / 2;
}

void MusicPlayer::initializeListPanel() {
  caculateListPanelInformation();
  listPanel =
      newwin(listPanelHighth, listPanelWidth, listPanelStarty, listPanelStartx);
}

void MusicPlayer::printListPanel() {
  wattron(listPanel, COLOR_PAIR(Green));

  box(listPanel, 0, 0);

  wattroff(listPanel, COLOR_PAIR(Green));
  wrefresh(listPanel);
}
