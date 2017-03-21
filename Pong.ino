#define paddleSize 2
void pong() {
  JXValue = 0;
  buttonValue = 0;
  oldButtonValue = 0;
  frame = 0;

  byte p1S = 0;
  byte p2S = 0;

  byte pos1 = 18;
  byte pos2 = 18;
  int bx = 31;
  int by = 18;
  int bxv = 1;
  int byv = 1;
  int speeds = 6;

  backgroundLayer.setFont(font3x5);
  //matrix.setBrightness((int)(brightness / 2));
  while (!buttonValue) {
    JYValue = getAxis(0, 0);
    buttonValue = getButton(0, 0);
    backgroundLayer.fillScreen({0, 0, 0});
    if (frame % 4 == 0) {
      if (JYValue < 500) {
        pos2--;
        if (pos2 < 8) { //keeps ship on screen
          pos2 = 8;
        }
      } else if (JYValue > 600) {
        pos2++;
        if (pos2 > 28) { //keeps ship on screen
          pos2 = 28;
        }
      }
      if (bx < 40 && bxv < 0) {
        if (by > pos1) {
          pos1++;
          if (pos1 > 28) { //keeps ship on screen
            pos1 = 28;
          }
        }
        if (by < pos1) {
          pos1--;
          if (pos1 < 8) { //keeps ship on screen
            pos1 = 8;
          }
        }
      }
    }
    //pos1 = pos2;

    if (frame % speeds == 0) {
      bx += bxv;
      by += byv;

      if (by >= 30) {
        byv = -byv;
      }
      if (by <= 6) {
        byv = -byv;
      }
      if (bx == 63) {
        p1S++;
        bx = 31;
        by = 18;
        speeds = 6;
        frame = 0;
      }
      if (bx == 0) {
        p2S++;
        bx = 31;
        by = 18;
        speeds = 6;
        frame = 0;
      }

      if (bx >= 61) {
        if (by >= pos2 - paddleSize && by <= pos2 + paddleSize && bxv > 0) {
          bxv = -bxv;
        }
      }
      if (bx <= 2) {
        if (by >= pos1 - paddleSize && by <= pos1 + paddleSize && bxv < 0) {
          bxv = -bxv;
        }
      }
    }

    //boarders and center line
    backgroundLayer.drawFastVLine(31, 5, 31, {255, 255, 255});
    for (int i = 5; i < 31; i += 2) {
      backgroundLayer.drawPixel(31, i, {0, 0, 0});
    }
    backgroundLayer.drawFastHLine(0, 63, 5, {102, 255, 51});
    backgroundLayer.drawFastHLine(0, 63, 31, {102, 255, 51});

    backgroundLayer.drawFastVLine(1, pos1 - paddleSize, pos1 + paddleSize, {102, 255, 51});
    backgroundLayer.drawFastVLine(62, pos2 - paddleSize, pos2 + paddleSize, {102, 255, 51});
    //ball
    backgroundLayer.drawPixel(bx, by, {0, 0, 255});
    //score
    String(p1S).toCharArray(str, 50);
    backgroundLayer.drawString(23, 0, {255, 255, 255}, str);
    String(p2S).toCharArray(str, 50);
    backgroundLayer.drawString(33, 0, {255, 255, 255}, str);

    backgroundLayer.swapBuffers(false);

    frame++;
    if (frame % 1500 == 0) {
      speeds--;
    }
    delay(5);
  }
  backgroundLayer.fillScreen({0, 0, 0});
  backgroundLayer.swapBuffers(false);
  delay(10);
  //matrix.setBrightness((int)(brightness * 2));
}
