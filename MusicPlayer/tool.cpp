#include "MusicPlayer.h"
#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <random>
#include <sys/stat.h>

const char *MusicPlayer::getMusicName() {
  const char *musicPath = _music.getMusicName();
  const char *result = std::find_if(std::make_reverse_iterator(
                                        musicPath + std::strlen(musicPath)),
                                    std::make_reverse_iterator(musicPath),
                                    [](char c) { return c == '/'; })
                           .base();
  return result;
}

bool MusicPlayer::isDirectory(const char *path) {
  struct stat fileInfo;

  if (stat(path, &fileInfo) != 0)
    printErrMsgAndExit("Error getting file information.");

  if (S_ISDIR(fileInfo.st_mode))
    return true;
  else
    return false;
}

void MusicPlayer::printErrMsgAndExit(const char *msg) {
  destructHandler();
  std::fprintf(stderr, "%s\n", msg);
  std::exit(EXIT_FAILURE);
}

void MusicPlayer::load(const char *musicName) {
  int volume = _music.getVolume();
  bool pause = _music.isPaused();
  bool loop = _music.isLoop();
  bool mute = _music.isMute();

  _sem.acquire();
  _music.end();

  _music.load(musicName);
  _sem.release();

  _music.setVolume(volume);

  _music.play();
  if (pause)
    _music.pause();
  if (loop)
    _music.loop();
  if (mute)
    _music.muteVolume();
}

void MusicPlayer::loadMusicList(const char *path) {
  DIR *dir;
  if ((dir = opendir(path)) == NULL)
    printErrMsgAndExit("Music directory error.");

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      std::string musicName;
      musicName = std::string(path) + "/" + entry->d_name;
      musicList.push_back(musicName);
      continue;
    }
    if (entry->d_type == DT_DIR && std::strcmp(entry->d_name, ".") &&
        std::strcmp(entry->d_name, "..")) {
      std::string musicPath;
      musicPath = std::string(path) + "/" + entry->d_name;
      loadMusicList(musicPath.c_str());
    }
  }

  if (musicList.empty()) {
    std::fprintf(stderr, "This directory is empty for music.\n");
    std::exit(EXIT_FAILURE);
  }
}

void MusicPlayer::destructHandler() {
  delwin(playPanel);
  delwin(listPanel);
  _music.end();
  endwin();
}

void MusicPlayer::randomArrange() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(musicList.begin(), musicList.end(), rng);

  _index = 0;
  load(musicList[0].c_str());
}
