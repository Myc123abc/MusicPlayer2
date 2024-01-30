#include "MusicPlayer.h"

MusicPlayer::MusicPlayer() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak(); // not use raw, cbreak allow interrupte service
  noecho();
  curs_set(0);
  // keypad(stdscr, TRUE);  // can use F1 key, arrow
  // mousemask(ALL_MOUSE_EVENTS, NULL); // can use mouse
  // nodelay(stdscr, TRUE); // set stand input non-blocking

  use_default_colors();
  start_color();
  init_pair(Green, COLOR_GREEN, -1);
}

MusicPlayer::~MusicPlayer() {
  stopPrintThread = true;
  threadForPrintPlayPanel.join();
  destructHandler();
}

void MusicPlayer::loadMusic(const char *path) {
  if (isDirectory(path)) {
    loadMusicList(path);
  } else {
    loadSingleMusic(path);
  }
}

void MusicPlayer::run() {
  _music.load(musicList[0].c_str());
  _music.play();
  _music.pause();
  
  initializeListPanel();
  initializePlayPanel();

  printPlayPanelWithThread();

  commandDriven();
}
