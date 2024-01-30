#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include "bass.h"

using namespace std::chrono_literals;

int main() {
  BASS_Init(-1, 44100, 0, 0, 0);
  BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, 10000);

  HSTREAM music = BASS_StreamCreateFile(FALSE,
      "/home/myc/裸体舞曲.mp3", 0, 0, BASS_STREAM_AUTOFREE);
  BASS_ChannelPlay(music, TRUE);

  initscr();
  cbreak();
  noecho();

  int width, highth;
  getmaxyx(stdscr, highth, width);

  WINDOW *win = newwin(4, 30, (highth - 4) / 2, (width - 30) / 2);
  box(win, 0, 0);
  wrefresh(win);

  bool stop = false;

  int ohighth, owidth;
  getmaxyx(stdscr, ohighth, owidth);

  std::thread t([&] {
    while (!stop) {
      std::this_thread::sleep_for(50ms);

      getmaxyx(stdscr, highth, width);
      if (highth != ohighth || width != owidth) {
        clear();
        refresh();

        wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        delwin(win);

        ohighth = highth;
        owidth = width;

        win = newwin(4, 30, (highth - 4) / 2, (width - 30) / 2);
        box(win, 0, 0);
      }
      wrefresh(win);
    }
  });

  int ch = 0;
  while (ch != 'q') {
    read(STDIN_FILENO, &ch, 1);
  }

  stop = true;
  t.join();
  endwin();

  BASS_StreamFree(music);
  BASS_Free();

  return 0;
}
