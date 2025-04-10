#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int KEY_ESC = 27;

const int GREEN_COLOR = 32;
const int YELLOW_COLOR = 33;

void lesson_6() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  int prev_key;
  int clicked;
  clock_t start_time;

  int key;
  int end = 0;
  while (!end) {
    refresh();
    key = getch();

    println("");

    switch (key) {
    case KEY_F(1):
      println("návod na použitie");
      break;
    case KEY_F(2):
      start_time = clock();
      break;
    case KEY_ESC:
      end = 1;
      break;
    case KEY_F(10):
      end = 1;
      break;
    default:
      if (!start_time) {
        break;
      }

      if (!prev_key) {
        prev_key = key;
        break;
      }

      if (prev_key == key) {
        clicked = 1;
        break;
      }

      if ((prev_key != key) && (clicked = 1)) {
        clicked = 0;
        prev_key = key;
        printf("%f\n", (float)(clock() - start_time));
      }

      break;
    }
  }
  endwin();
}
long max_str(char *arr[], int count) {
  int max = 0;
  for (int i = 0; i < count; i++) {
    int len = strlen(arr[i]);
    if (len > max)
      max = len;
  }
  return max;
}

char *gen_str(char symbol, int size) {
  char *str = malloc(size + 1);

  for (int i = 0; i < size; i++) {
    str[i] = symbol;
  }

  str[size] = '\0';

  return str;
}

char *colorized(char *str, int color) {
  char color_start[10];
  char color_end[10];

  static char buffer[sizeof(str) + sizeof(color_start) + sizeof(color_end)];

  sprintf(color_start, "\033[%dm", color);
  sprintf(color_end, "\033[0m");

  buffer[0] = '\0';
  strcat(buffer, color_start);
  strcat(buffer, str);
  strcat(buffer, color_end);

  return buffer;
}

void lesson_3() {
  char *title = "Array 001";

  char *name_title = "Name";
  char *name = "David Tsofin asdasdasdasd";

  char *height_title = "Height";
  char *height = "175cm";

  char *wight_title = "Weight";
  char *weight = "85kg";

  char *phone_title = "Phone";
  char *phone = "0950 710 314";

  int rows = 2;
  int columns = 3;

  int title_width = max_str(
      (char *[]){name_title, height_title, phone_title, wight_title}, 4);

  int value_width = max_str((char *[]){name, height, phone, weight}, 4);

  int pad = (title_width + value_width + 1 - strlen(title)) / 2;

  printf(colorized("+- ", YELLOW_COLOR));
  printf(colorized("%*s%s%*s", GREEN_COLOR), pad, "", title, pad, "");
  printf(colorized(" -+\n", YELLOW_COLOR));

  printf(colorized("+-%*s-+-%*s-+\n", YELLOW_COLOR), title_width,
         gen_str('-', title_width), value_width, gen_str('-', value_width));

  printf(colorized("| ", YELLOW_COLOR));
  printf(colorized("%-*s", GREEN_COLOR), title_width, name_title);
  printf(colorized(" | ", YELLOW_COLOR));
  printf(colorized("%*s", GREEN_COLOR), value_width, name);
  printf(colorized(" |\n", YELLOW_COLOR));

  printf(colorized("| ", YELLOW_COLOR));
  printf(colorized("%-*s", GREEN_COLOR), title_width, height_title);
  printf(colorized(" | ", YELLOW_COLOR));
  printf(colorized("%*s", GREEN_COLOR), value_width, height);
  printf(colorized(" |\n", YELLOW_COLOR));

  printf(colorized("| ", YELLOW_COLOR));
  printf(colorized("%-*s", GREEN_COLOR), title_width, wight_title);
  printf(colorized(" | ", YELLOW_COLOR));
  printf(colorized("%*s", GREEN_COLOR), value_width, weight);
  printf(colorized(" |\n", YELLOW_COLOR));

  printf(colorized("| ", YELLOW_COLOR));
  printf(colorized("%-*s", GREEN_COLOR), title_width, phone_title);
  printf(colorized(" | ", YELLOW_COLOR));
  printf(colorized("%*s", GREEN_COLOR), value_width, phone);
  printf(colorized(" |\n", YELLOW_COLOR));

  printf(colorized("+-%*s-+-%*s-+\n", YELLOW_COLOR), title_width,
         gen_str('-', title_width), value_width, gen_str('-', value_width));
}

int main() {
  lesson_6();
  // lesson_3();
  return 0;
}