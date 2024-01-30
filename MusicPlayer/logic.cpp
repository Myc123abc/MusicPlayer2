#include "MusicPlayer.h"
#include <unistd.h>

using namespace std::chrono_literals;

void MusicPlayer::commandDriven() {
  while (true) {
    int inputChar;
    read(STDIN_FILENO, &inputChar, 1);
    switch (inputChar) {
    case 'q':
      return;
    case ' ': {
      if (_music.isPaused()) {
        _music.start();
      } else {
        _music.pause();
      }
    } break;
    case 'k': {
      _music.increaseVolume(10);
    } break;
    case 'j': {
      _music.decreaseVolume(10);
    } break;
    case 'm': {
      _music.muteVolume();
    } break;
    case 'l': {
      _music.progressForward(10);
    } break;
    case 'h': {
      _music.progressBackward(10);
    } break;
    case 'c': {
      _music.loop();
    } break;
    case 'd': {
      printListPanel();
      commandForList();
    } break;
    case 'n': {
      nextMusic();
    } break;
    case 'p': {
      prevMusic();
    } break;
    case 'a': {
      randomArrange();
    } break;
    }
    inputChar = 0;
  }
}

void MusicPlayer::nextMusic() {
  if (_index == musicList.size() - 1) {
    _index = 0;
  } else {
    ++_index;
  }
  load(musicList[_index].c_str());
}

void MusicPlayer::prevMusic() {
  if (_index == 0) {
    _index = musicList.size() - 1;
  } else {
    --_index;
  }
  load(musicList[_index].c_str());
}

void MusicPlayer::commandForList() {
  while (true) {
    int inputChar;
    read(STDIN_FILENO, &inputChar, 1);
    switch (inputChar) {
    case 'd':
      return;
    }
    inputChar = 0;
  }
  clear();
}
