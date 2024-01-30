#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "../Music/Music.h"
#include <ncurses.h>
#include <semaphore>
#include <thread>
#include <vector>

enum Color { Green = 1 };

class MusicPlayer {
public:
  MusicPlayer();
  ~MusicPlayer();

  void run();
  void loadMusic(const char *path);

private:
  // playPanel
  void printPlayPanel();
  void printPlayPanelWithThread();
  void initializePlayPanel();
  void updatePlayPanelPosition();
  void caculatePlayPanelPosition();

  // listPanel
  void printListPanel();
  void initializeListPanel();
  void caculateListPanelInformation();

  // user command
  void commandDriven();
  void commandForList();
  void nextMusic();
  void prevMusic();

  // load music
  void load(const char *musicName);
  void loadSingleMusic(const char *musicName) {
    musicList.push_back(musicName);
  }
  void loadMusicList(const char *path);

  // tool
  bool isDirectory(const char *path);

  void printErrMsgAndExit(const char *msg);

  const char *getMusicName();
  const char *getMusicNameFromList();

  void destructHandler();

  void randomArrange();

private:
  // For terminal
  int screenHighth = 0, screenWidth = 0;

  // For playPanel
  WINDOW *playPanel = nullptr;
  int playPanelStarty = 0, playPanelStartx = 0;
  const unsigned playPanelHighth = 4, playPanelWidth = 30;
  std::thread threadForPrintPlayPanel;
  bool stopPrintThread = false;

  // For ListPanel
  WINDOW *listPanel = nullptr;
  int listPanelStarty = 0, listPanelStartx = 0;
  int listPanelHighth = 0, listPanelWidth = 0;

  Music _music;
  std::vector<std::string> musicList;
  unsigned _index = 0;

  std::binary_semaphore _sem{1};
};

#endif
