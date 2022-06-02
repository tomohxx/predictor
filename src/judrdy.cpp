#include "judwin.hpp"
#include "judrdy.hpp"

int isrh2(int* t)
{
  int p = 0, wait = 0;

  for (int i = 0; i < 9; ++i) p += i * t[i];

  for (int i = p * 2 % 3; i < 9; i += 3) {
    if (t[i] < 4) {
      ++t[i];

      if (iswh0(t)) {
        wait ^= 1 << i;
      }
      --t[i];
    }
  }
  return wait;
}

int isrh1(int* t)
{
  int p = 0, wait = 0;

  for (int i = 0; i < 9; ++i) p += i * t[i];

  for (int i = 0; i < 9; ++i) {
    if (t[i] < 4) {
      ++t[i];

      for (int j = (2 * p - i) % 3; j < 9; j += 3) {
        if (t[j] >= 2) {
          t[j] -= 2;

          if (iswh0(t)) {
            t[j] += 2;
            wait ^= 1 << i;
            break;
          }
          else t[j] += 2;
        }
      }
      --t[i];
    }
  }
  return wait;
}
