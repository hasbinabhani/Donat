#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ROTATE(mul, shift, x, y) \
  _ = x; \
  x -= mul * y >> shift; \
  y += mul * _ >> shift; \
  _ = 3145728 - x * x - y * y >> 11; \
  x = x * _ >> 10; \
  y = y * _ >> 10;

int8_t buffer[1760], zbuffer[1760];

void main() {
  int sinA = 1024, cosA = 0, sinB = 1024, cosB = 0, _;
  for (;;) {
    memset(buffer, 32, 1760);  // text buffer
    memset(zbuffer, 127, 1760);   // z buffer
    int sinJ = 0, cosJ = 1024;
    for (int j = 0; j < 90; j++) {
      int sinI = 0, cosI = 1024;  // sine and cosine of angle i
      for (int i = 0; i < 324; i++) {
        int R1 = 1, R2 = 2048, K2 = 5120 * 1024;

        int x0 = R1 * cosJ + R2,
            x1 = cosI * x0 >> 10,
            x2 = cosA * sinJ >> 10,
            x3 = sinI * x0 >> 10,
            x4 = R1 * x2 - (sinA * x3 >> 10),
            x5 = sinA * sinJ >> 10,
            x6 = K2 + R1 * 1024 * x5 + cosA * x3,
            x7 = cosJ * sinI >> 10,
            x = 40 + 30 * (cosB * x1 - sinB * x4) / x6,
            y = 12 + 15 * (cosB * x4 + sinB * x1) / x6,
            N = (-cosA * x7 - cosB * ((-sinA * x7 >> 10) + x2) - cosI * (cosJ * sinB >> 10) >> 10) - x5 >> 7;

        int o = x + 80 * y;
        int8_t zz = (x6 - K2) >> 15;
        if (22 > y && y > 0 && x > 0 && 80 > x && zz < zbuffer[o]) {
          zbuffer[o] = zz;
          buffer[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
        }
        ROTATE(5, 8, cosI, sinI)  // rotate i
      }
      ROTATE(9, 7, cosJ, sinJ)  // rotate j
    }
    for (int k = 0; k < 1761; k++)
      putchar(k % 80 ? buffer[k] : 10);
    ROTATE(5, 7, cosA, sinA);
    ROTATE(5, 8, cosB, sinB);
    usleep(15000);
    printf("\x1b[23A");
  }
}



