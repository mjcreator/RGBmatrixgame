void snake() {
  JXValue = 0;
  JYValue = 0;
  buttonValue = 0;
  score = 4;
  frame = 0;

  int bodyPixelsX[400];
  int bodyPixelsY[400];
  int myLength = 4;
  byte Direction = 2;
  int oldDirection = 2;
  int food[20];//(X,Y)


  for (int i = 0; i < 20; i += 2) {
    food[i] = int(random(0, 63));
    food[i + 1] = int(random(0, 31));
  }
  food[0] = 16;
  food[1] = 16;

  for (int i = 0; i < 400; i++) {
    bodyPixelsX[i] = -1;
    bodyPixelsY[i] = -1;
  }
  for (int i = 0; i < myLength; i++) {
    bodyPixelsX[i] = myLength - 1 - i;
    bodyPixelsY[i] = 16;
  }

  while (!buttonValue) {
    JXValue = getAxis(0, 1); //read inputs
    JYValue = getAxis(0, 0);
    buttonValue = getButton(0, 0);

    if (JYValue < 500) { //go the direction we say
      Direction = 3;
    } else if (JYValue > 600) {
      Direction = 1;
    } if (JXValue < 500) {
      Direction = 4;
    } else if (JXValue > 600) {
      Direction = 2;
    }
    //protects you from going back on yourself
    if (Direction == oldDirection + 2) {
      Direction = oldDirection;
    }
    if (Direction == oldDirection - 2) {
      Direction = oldDirection;
    }
    if (frame == 10) {
      switch (Direction) {// keeps it going forward
        case 1:
          bodyPixelsY[0]++;
          break;
        case 2:
          bodyPixelsX[0]++;
          break;
        case 3:
          bodyPixelsY[0]--;
          break;
        case 4:
          bodyPixelsX[0]--;
          break;
      }
      oldDirection = Direction;
      for (int i = 0; i < 20; i += 2) {
        if (bodyPixelsX[0] == food[i]) {
          if (bodyPixelsY[0] == food[i + 1]) {
            food[i] = int(random(0, 63));
            food[i + 1] = int(random(0, 31));
            myLength = myLength + 3;
            score = score + 3;
            Serial.println(score);
          }
        }
      }

      //keeps you in bounds
      if (bodyPixelsY[0] < 0) {
        goto GameOver;
      } if (bodyPixelsY[0] > 31) {
        goto GameOver;
      } if (bodyPixelsX[0] < 0) {
        goto GameOver;
      } if (bodyPixelsX[0] > 63) {
        goto GameOver;
      }

      for (int i = 1; i < myLength; i++) {
        if (bodyPixelsX[i] == bodyPixelsX[0]) {
          if (bodyPixelsY[i] == bodyPixelsY[0]) {
            goto GameOver;
          }
        }
      }

      //moves the snake
      backgroundLayer.fillScreen({0, 0, 0});
      for (int i = 0; i < myLength; i++) { //makes varuble i, makes it repeate, increses i by 1
        backgroundLayer.drawPixel(bodyPixelsX[i], bodyPixelsY[i], {0, 255, 36}); //turns on the first pixel of the snake
      }
      for (int i = 0; i < 20; i += 2) {
        backgroundLayer.drawPixel(food[i], food[i + 1], {255, 255, 0});
      }
      for (int i = 0; i < myLength - 1; i++) {
        bodyPixelsX[myLength - 1 - i] = bodyPixelsX[myLength - 2 - i];
        bodyPixelsY[myLength - 1 - i] = bodyPixelsY[myLength - 2 - i];
      }
      //updates display
      backgroundLayer.swapBuffers(false);
    }

    frame++;
    if (frame == 11) {
      frame = 1;
    }
    delay(10);
  }
GameOver:
  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(0, 10, {255, 0, 0}, "You Died");
  backgroundLayer.swapBuffers(false);
  delay(2000);
  backgroundLayer.fillScreen({0, 0, 0});
  backgroundLayer.drawString(0, 2, {255, 0, 0}, "Score: ");
  String(score).toCharArray(str, 50);
  backgroundLayer.drawString(0, 16, {255, 0, 0}, str);
  backgroundLayer.swapBuffers(false);
  delay(2000);
  if (score > SHighScore) {
    SHighScore = score;
    backgroundLayer.fillScreen({0, 0, 0});
    backgroundLayer.drawString(0, 2, {0, 180, 0}, "New High");
    backgroundLayer.drawString(8, 16, {0, 180, 0}, "Score!");
    backgroundLayer.swapBuffers(false);
    delay(2000);
  } else {
    backgroundLayer.fillScreen({0, 0, 0});
    backgroundLayer.drawString(16, 0, {0, 180, 0}, "High");
    backgroundLayer.drawString(12, 10, {0, 180, 0}, "Score");
    String(SHighScore).toCharArray(str, 50);
    backgroundLayer.drawString(20, 20, {255, 0, 0}, str);
    backgroundLayer.swapBuffers(false);
    delay(2000);
  }
  return;
}

