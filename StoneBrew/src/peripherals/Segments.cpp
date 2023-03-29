#include "Segments.h"

int Segments::nthdigit(const int number, const int digit) {
  return ((int)trunc(number / ((int)pow(10, digit)))) % 10;
}

void Segments::displayF(float number) {
  int n = floor(number);
  int dp = -1;

  if (number < 1000 && number > 0) {
    if (number < 10) {
      n = floor(number * 100);
    } else if (number < 10) {
      n = floor(number * 10);
    } else if (number < 100) {
      n = floor(number);
    }
  }

  if (number < 0 || number >= 1000) {
    lc.setChar(0, 0, 'e', false);
    lc.setChar(0, 1, 'r', false);
    lc.setChar(0, 2, 'r', false);
    dp = -1;
  } else if (number < 10) {
    n = floor(number * 10);
    dp = 1;
  } else if (number < 100) {
    n = floor(number * 10);
    dp = 1;
  } else if (number < 1000) {
    n = floor(number);
    dp = -1;
  }


  lc.setDigit(0, 0, nthdigit(n, 2), dp == 0);
  lc.setDigit(0, 1, nthdigit(n, 1), dp == 1);
  lc.setDigit(0, 2, nthdigit(n, 0), dp == 2);


  // Final digit is the unit.
  // lc.setRow(0, 3, 0x4E);
  lc.setChar(0, 3, 'F', false);
}
