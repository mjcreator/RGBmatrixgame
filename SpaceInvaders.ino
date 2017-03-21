void spaceInvaders() {
  JXValue = 0;
  buttonValue = 0;
  oldButtonValue = 0;
  score = 0;
  level = 1;
  frame = 0;

start:
  int enemyData[81]; // 28 enemies (x,y,type)
  int shotData[18];  //6 possible shots at once (x,y,Direction)
  byte place = 8;
  int xPos = place;
  int yPos = 0;
  int bonusPos = 64;
  int time = 200;
  byte myShipPos = 31;
  int countDown = 100;
  int arrayPos = 0;
  int lives = 3;
  int enemies = 27;
  int row = 0;
  int column = 0;
  boolean eDirection = 1;
  byte choice = 0;


  const PROGMEM boolean invader3 [5][5] = {
    {0, 1, 1, 1, 0},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1},
    {0, 1, 0, 1, 0},
    {0, 1, 1, 1, 0}
  };
  const PROGMEM boolean invader2 [5][5] = {
    {0, 1, 1, 1, 0},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0}
  };
  const PROGMEM boolean invader1 [5][5] = {
    {0, 1, 1, 1, 0},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 1}
  };
  const PROGMEM boolean bonusShip [5][9] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 1, 0, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 0, 1, 1, 1, 0}
  };


  for (int i = 0; i < 18; i++) {
    shotData[i] = 0;
  }

  for (int c = 1; c < 4; c++) {
    for (int i = 2; i < 51; i = i + 6) { //21 + 6
      enemyData[arrayPos] =  i;
      if (c == 1) {
        enemyData[arrayPos + 1] = 10;
        enemyData[arrayPos + 2] = 1;
      } if (c == 2) {
        enemyData[arrayPos + 1] = 16;
        enemyData[arrayPos + 2] = 2;
      } if (c == 3) {
        enemyData[arrayPos + 1] = 22;
        enemyData[arrayPos + 2] = 3;
      }
      //      Serial.print(arrayPos);
      //      Serial.print(":");
      //      Serial.print(enemyData[arrayPos]);
      //      Serial.print(",");
      //      Serial.print(enemyData[arrayPos + 1]);
      //      Serial.print(",");
      //      Serial.print(enemyData[arrayPos + 2]);
      //      Serial.println();
      arrayPos = arrayPos + 3;

    }
  }

  backgroundLayer.fillScreen({0, 0, 0});
  backgroundLayer.setFont(font6x10);
  String(level).toCharArray(str, 50);
  backgroundLayer.drawString(0, 12, {51, 255, 51}, "level");
  backgroundLayer.drawString(34, 12, {51, 255, 51}, str);
  backgroundLayer.swapBuffers(false);
  delay(2000);

  while (true) {
    JXValue = getAxis(0, 1);
    buttonValue = getButton(0, 0);
    backgroundLayer.fillScreen({0, 0, 0});

    //user movement code
    if (JXValue < 500) {
      myShipPos--;
      if (myShipPos < 2) { //keeps ship on screen
        myShipPos = 2;
      }
    } else if (JXValue > 600) {
      myShipPos++;
      if (myShipPos > 61) { //keeps ship on screen
        myShipPos = 61;
      }
    } if (buttonValue == 1) {
      if (oldButtonValue == 0) {
        if (shotData[2] == 0) {
          shotData[0] = myShipPos;
          shotData[1] = 29;
          shotData[2] = 1;
        }
      }
    }
    oldButtonValue = buttonValue;

    backgroundLayer.setFont(font3x5);
    String(lives).toCharArray(str, 50);
    backgroundLayer.drawString(56, 27, {0, 0, 127}, "x");
    backgroundLayer.drawString(60, 27, {0, 0, 127}, str);

    //draws the ship pixel by pixel
    backgroundLayer.drawPixel(myShipPos, 30, {0, 255, 36});
    backgroundLayer.drawPixel(myShipPos, 29, {0, 255, 36});
    backgroundLayer.drawPixel(myShipPos + 1, 31, {0, 255, 36});
    backgroundLayer.drawPixel(myShipPos + 1, 30, {0, 255, 36});
    backgroundLayer.drawPixel(myShipPos - 1, 31, {0, 255, 36});
    backgroundLayer.drawPixel(myShipPos - 1, 30, {0, 255, 36});

    //tests if shots have hit something
    for (int i = 0; i < 18; i = i + 3) {
      if (shotData[i + 2] > 0) {
        if (shotData[i + 2] == 1) {
          shotData[i + 1] = shotData[i + 1] - 1;
          if (shotData[i + 1] == 0) {
            shotData[i + 2] = 3;
          } else {
            arrayPos = 0;
            for (int y = 0; y < 27; y++) {
              if (enemyData[arrayPos + 1] + yPos == shotData[1]) {
                if (enemyData[arrayPos] + 3 + place > shotData[0]) {
                  if (enemyData[arrayPos] - 3 + place < shotData[0]) {
                    shotData[i + 2] = 3;
                    enemyData[arrayPos] = 0;
                    enemyData[arrayPos + 1] = 0;
                    enemyData[arrayPos + 2] = 0;
                    enemies--;
                    score = score + 50;

                    Serial.print(enemies);
                    Serial.print(" ");
                    Serial.println(score);
                    goto fire;
                  }
                }
              }
              arrayPos = arrayPos + 3;
            }
            if (shotData[i + 1] < 5) {
              if (bonusPos + 9 > shotData[0]) {
                if (bonusPos - 1 < shotData[0]) {
                  shotData[i + 2] = 3;
                  bonusPos = -9;
                  score = score + 100 * int(random(1, 10));
                  Serial.print(enemies);
                  Serial.print(" ");
                  Serial.println(score);
                  goto fire;
                }
              }
            }
          }
        } else if (shotData[i + 2] == 2) {
          shotData[i + 1] = shotData[i + 1] + 1;
          if (shotData[i + 1] == 31) {
            shotData[i + 2] = 3;
          } else {
            if (shotData[i + 1] > 29) {
              if (myShipPos + 2 > shotData[i]) {
                if (myShipPos - 2 < shotData[i]) {
                  shotData[i + 2] = 3;
                  lives--;
                  Serial.print("lives");
                  Serial.println(lives);
                  backgroundLayer.fillRectangle(myShipPos - 2, 31, 5, 3, {0, 0, 0});
                  backgroundLayer.drawPixel(myShipPos , 31, {255, 36, 0});
                  backgroundLayer.drawPixel(myShipPos - 1, 30, {255, 36, 0});
                  backgroundLayer.drawPixel(myShipPos, 29, {255, 36, 0});
                  backgroundLayer.drawPixel(myShipPos + 1, 30, {255, 36, 0});
                  backgroundLayer.swapBuffers(false);
                  delay(1000);
                  myShipPos = 31;
                }
              }
            }
          }
        }
fire:
        if (shotData[i + 2] == 3) { //displays shot explosion
          backgroundLayer.drawPixel(shotData[i], shotData[i + 1], {255, 36, 0});
          backgroundLayer.drawPixel(shotData[i] + 1, shotData[i + 1], {255, 36, 0});
          backgroundLayer.drawPixel(shotData[i] - 1, shotData[i + 1], {255, 36, 0});
          backgroundLayer.drawPixel(shotData[i], shotData[i + 1] - 1, {255, 36, 0});
          shotData[i + 2] = 0;
        } else {  //draws shot if it is in space
          backgroundLayer.drawPixel(shotData[i], shotData[i + 1], {255, 255, 255});
        }

      }
    }

    //guys shooting
    if (countDown < 1) {
      choice = int(random(0, 26)) * 3;
      if (enemyData[choice + 2] > 0) {
        for (int t = 3; t < 18; t = t + 3) {
          if (shotData[t + 2] == 0) {
            shotData[t] = enemyData[choice] + place;
            shotData[t + 1] = enemyData[choice + 1] + yPos;
            shotData[t + 2] = 2;
            countDown = int(random(10, 50));
            goto shoot;
          }
        }
      }
    }
shoot:

    if (enemies == 0) {
      backgroundLayer.setFont(font6x10);
      backgroundLayer.fillScreen({0, 0, 0});
      backgroundLayer.drawString(0, 2, {255, 0, 0}, "Score: ");
      String(score).toCharArray(str, 50);
      backgroundLayer.drawString(0, 16, {255, 0, 0}, str);
      backgroundLayer.swapBuffers(false);
      delay(2000);
      boolean c = true;
      while (true) {
        backgroundLayer.fillScreen({0, 0, 0});
        backgroundLayer.drawString(4, 2, {255, 0, 0}, "continue?");
        JYValue = getAxis(0, 0);
        if (oldJYValue < 600 && oldJYValue > 500) {
          if (JYValue > 600) {
            c = !c;
          } else if (JYValue < 500) {
            c = !c;
          }
        }
        if (c) {
          backgroundLayer.drawString(22, 15, {0, 255, 0}, "Yes");
        } else {
          backgroundLayer.drawString(25, 15, {255, 0, 0}, "No");
        }
        oldJYValue = JYValue;
        if (getButton(0, 0)) {
          if (c) {
            level++;
            goto start;
          } else {
            lives = 0;
            enemies--;
            backgroundLayer.fillScreen({0, 0, 0});
            goto shoot;
          }
        }
        backgroundLayer.swapBuffers(false);
        delay(100);

      }
    }
    //    //testing if you lose
    if (lives < 0) {
      backgroundLayer.setFont(font8x13);
      backgroundLayer.drawString(0, 10, {255, 0, 0}, "GameOver");
      backgroundLayer.swapBuffers(false);
      delay(2000);
      backgroundLayer.fillScreen({0, 0, 0});
      backgroundLayer.drawString(0, 2, {255, 0, 0}, "Score: ");
      String(score).toCharArray(str, 50);
      backgroundLayer.drawString(0, 16, {255, 0, 0}, str);
      backgroundLayer.swapBuffers(false);
      delay(2000);
      if (score > IHighScore) {
        IHighScore = score;
        backgroundLayer.fillScreen({0, 0, 0});
        backgroundLayer.drawString(0, 2, {0, 180, 0}, "New High");
        backgroundLayer.drawString(8, 16, {0, 180, 0}, "Score!");
        backgroundLayer.swapBuffers(false);
        delay(2000);
      } else {
        backgroundLayer.fillScreen({0, 0, 0});
        backgroundLayer.drawString(16, 0, {0, 180, 0}, "High");
        backgroundLayer.drawString(12, 10, {0, 180, 0}, "Score");
        String(IHighScore).toCharArray(str, 50);
        backgroundLayer.drawString(5, 20, {255, 0, 0}, str);
        backgroundLayer.swapBuffers(false);
        delay(2000);
      }
      return;
    }

    //move guys
    if (frame == 20) {
      if (eDirection) {
        place--;
      } else {
        place++;
      }
      if (place == 10) {
        eDirection = true;
        yPos++;
      }
      if (place == 1) {
        eDirection = false;
        yPos++;
      }
    }

    //Display guys
    arrayPos = 0;
    xPos = place;
    for (int i = 0; i < 9; i++) {
      row = 0;
      column = 0;
      for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
          if (enemyData[arrayPos + 2] == 1) {
            if (invader1[row][column]) {
              backgroundLayer.drawPixel(xPos, y + 6 + yPos, {0, 255, 36});
            }
          }
          if (enemyData[arrayPos + 29] == 2) {
            if (invader2[row][column]) {
              backgroundLayer.drawPixel(xPos, y + 12 + yPos, {0, 255, 36});
            }
          }
          if (enemyData[arrayPos + 56] == 3) {
            if (invader3[row][column]) {
              backgroundLayer.drawPixel(xPos, y + 18 + yPos, {0, 255, 36});
            }
          }
          row++;
        }
        xPos++;
        column++;
        row = 0;
      }
      xPos++;
      arrayPos = arrayPos + 3;
    }

    //bonus ship rendering
    if (time == 0) {
      for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 9; y++) {
          if (bonusShip[x][y]) {
            backgroundLayer.drawPixel(y + bonusPos, x, {153, 51, 255});
          }
        }
      }
      if (frame % 2 == 0) {
        bonusPos--;
      }
    } else {
      time--;
    }
    if (bonusPos <= -9) {
      time = 100 * int(random(2, 10));
      bonusPos = 64;
    }

    countDown--;
    frame++;
    if (frame > 20) {
      frame = 1;
    }
    backgroundLayer.swapBuffers(true); //refreshes the screen
  }
}

