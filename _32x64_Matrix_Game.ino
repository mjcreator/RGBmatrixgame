#include <Adafruit_ADS1015.h>
#include <SmartMatrix3.h>
#include <avr/pgmspace.h>


#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
#define kMatrixWidth  64       // known working: 32, 64, 96, 128
#define kMatrixHeight  32      // known working: 16, 32, 48, 64
#define kRefreshDepth  36       // known working: 24, 36, 48
#define kDmaBufferRows  4       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer1, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer2, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

//int defaultBrightness  100*(255 / 100) // full brightness
int brightness = 100 * (255 / 100); // dim: 15% brightness

const rgb24 defaultBackgroundColor = {0x40, 0, 0};

Adafruit_ADS1015 ads[] = {0x48, 0x49};

const char* text[] PROGMEM = {
  "Snake",
  "Space Invaders",
  "Pong"
};

//make the variables to store data.

#define choices 3
int choice = 0;
int oldChoice = -1;
boolean buttonValue = 0;
int JXValue = 0;
int JYValue = 0;
boolean oldButtonValue = 0;
int oldJXValue = 0;
int oldJYValue = 0;
int score = 0;
int IHighScore = 24800;
int SHighScore = 244;
String texts = "";
char str[50];
int level = 0;
int frame = 0;

void setup() {
  Serial.begin(115200);

  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer1);
  matrix.addLayer(&scrollingLayer2);
  matrix.begin();

  matrix.setBrightness(brightness);

  scrollingLayer1.setOffsetFromTop(2);
  scrollingLayer2.setOffsetFromTop(16);
  scrollingLayer1.setMode(wrapForward);
  scrollingLayer2.setMode(wrapForward);
  scrollingLayer1.setFont(font8x13);
  scrollingLayer2.setFont(font8x13);

  scrollingLayer1.start(text[choice], -1);
  scrollingLayer2.start(text[choice + 1], -1);

  backgroundLayer.enableColorCorrection(true);
  beginJoysticks();
}

void loop() {
  //read inputs
  JYValue = getAxis(0, 0);
  buttonValue = getButton(0, 0);
  backgroundLayer.fillScreen({0, 0, 0});
  if (getButton(0, 1)) {
    backgroundLayer.setFont(font5x7);
    while (!getButton(0, 0)) {
      brightness = map(1100 - getAxis(0, 0), 0, 1100, 0, 255);
      char percent[] = "100%";
      percent[0] = '0' + (brightness * 100.0 / 255) / 100;
      percent[1] = '0' + (int)(brightness * 100.0 / 255) % 100 / 10;
      percent[2] = '0' + (int)(brightness * 100.0 / 255) % 10;

      backgroundLayer.fillScreen({0, 0, 0});
      backgroundLayer.fillRectangle(0, 0, 8, 8, {255, 255, 255});

      backgroundLayer.drawString(0, 16, {0xff, 0, 0}, "Brightness");
      backgroundLayer.drawString(12, 24, {0xff, 0, 0}, percent);
      matrix.setBrightness(brightness);
      backgroundLayer.swapBuffers();
    }
    backgroundLayer.fillScreen({0, 0, 0});
    backgroundLayer.swapBuffers();
    while (getButton(0, 0)) {}
  }

  //test the inputs and change the sellected choice
  if (oldJYValue < 600 && oldJYValue > 500) {
    if (JYValue > 600) {
      choice++;
      if (choice > choices - 1) { //check that choice isn't bigger then the options and set it to the next value.
        choice = 0;
      }
    } else if (JYValue < 500) {
      choice--;
      if (choice < 0) {//check that choice isn't smaller then the options and set it to the next value.
        choice = choices - 1;
      }
    }
    backgroundLayer.fillScreen({0, 0, 0});
    backgroundLayer.swapBuffers(true);
  }

  if (!oldButtonValue) {
    if (buttonValue) {
      Serial.println("Button Pressed");
      if (choice == 0) {      //runs choice 1
        runGame(snake);
      } else if (choice == 1) {    //runs choice 2
        runGame(spaceInvaders);
      } else {             //runs choice 3
        runGame(pong);
      }
      level = 0;
      score = 0;
    }
  }


  if (choice != oldChoice) {
    Serial.println(choice);
    scrollingLayer1.stop();
    scrollingLayer2.stop();
    scrollingLayer1.setColor({0, 255, 108});
    scrollingLayer2.setColor({36, 0, 255});

    texts = text[choice];
    if (texts.length() > 8) {
      scrollingLayer1.setMode(wrapForward);
    } else {
      scrollingLayer1.setMode(stopped);
    }
    texts.toCharArray(str, 20);
    scrollingLayer1.start(str, -1);

    texts = text[choice + 1];
    if (texts.length() > 8) {
      scrollingLayer2.setMode(wrapForward);
    } else {
      scrollingLayer2.setMode(stopped);
    }
    if (choice + 1 > choices - 1) {
      texts = text[0];
    }
    texts.toCharArray(str, 20);
    scrollingLayer2.start(str, -1);
  }

  //sets the the value to the old value so the input is only trigerd once
  oldButtonValue = buttonValue;
  oldJXValue = JXValue;
  oldJYValue = JYValue;
  oldChoice = choice;
  delay(100);
}
void runGame(void game()) {
  scrollingLayer1.update("");
  scrollingLayer2.update("");
  scrollingLayer1.stop();
  scrollingLayer2.stop();
  delay(1000);
  game();
  backgroundLayer.fillScreen({0, 0, 0});
  backgroundLayer.swapBuffers(true);
  oldChoice = 5;
  delay(100);
}

void beginJoysticks() {
  Wire.setSDA(17);
  Wire.setSCL(16);
  ads[0].begin();
  ads[1].begin();
}
void JoyDebug(int joy) {
  Serial.print(ads[joy].readADC_SingleEnded(0));
  Serial.print(" ");
  Serial.print(ads[joy].readADC_SingleEnded(1));
  Serial.print(" ");
  Serial.print(ads[joy].readADC_SingleEnded(2));
  Serial.print(" ");
  Serial.println(ads[joy].readADC_SingleEnded(3));
}

bool isConnected(int joy) {
  if (ads[joy].readADC_SingleEnded(0) > 4000) {
    backgroundLayer.fillScreen({0, 0, 0});
    backgroundLayer.setFont(font6x10);
    backgroundLayer.drawString(0, 2, {255, 0, 0}, "Connect");
    backgroundLayer.drawString(0, 16, {255, 0, 0}, "controller");
    backgroundLayer.swapBuffers(true);
  } else {
  }

}
int getAxis(int joy, int num) {
  //JoyDebug(joy);
  isConnected(joy);
  int adc = ads[joy].readADC_SingleEnded(num + 1);
  if (adc < 4000) {
    return adc;
  } else {
    return 550;
  }
}
bool getButton(int joy, int num) {
  //JoyDebug(joy);
  isConnected(joy);
  int adc = ads[joy].readADC_SingleEnded(num * 3);
  if (adc > 800 && adc < 4000) {
    return true;
  } else {
    return false;
  }
}

