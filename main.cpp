#include "MusicPlayer/MusicPlayer.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::fprintf(stderr, "Usage: ./a.out MusicPath MusicDirectory ...\n");
    std::exit(EXIT_FAILURE);
  }

  MusicPlayer player;
  for (int i = 1; i != argc; ++i) {
    player.loadMusic(argv[i]);
  }
  player.run();

  return 0;
}
