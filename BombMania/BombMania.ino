#include "LedControl.h"  // Include LedControl library for controlling the LED matrix
#include <LiquidCrystal.h>
#include <EEPROM.h>
const int dinPin = 13;
const int clockPin = 12;
const int loadPin = 3;
const int xPin = A0;
const int yPin = A1;
const int pinSW = 2;
const int brightnessPin = 10;
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const int buzzerPin = 11;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

// Create an LedControl object to manage the LED matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER
// Variables to track the current and previous positions of the joystick-controlled LED
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
byte playerLedState = LOW;
byte bombLedState = LOW;
byte buttonState = LOW;
byte reading = HIGH;
byte lastReading = HIGH;

int totalElements;
int numWalls;  // Adjust the number of walls as needed
int randomRow;
int randomCol;
int checkX;
int checkY;
int bombX;
int bombY;
int counter = 0;
int numOfBombs = 0;
int option = 0;
int setting = 0;
int lives = 3;
int lcdBrightness = 4;
int matrixBrightness = 4;
int timeIndex = 0;
int index = 1;
int letter1;
int letter2;
int letter3;
int name[] = { 0, 0, 0 };  //AAA
int allNames[] = { 97, 97, 97, 97, 97, 97, 97, 97, 97 };
int currentLetter = 0;
int frequencies[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };  // Frequencies for the boom effect
int numFrequencies = 10;                                                    // Number of frequencies in the array
int currentFrequency = 0;

char firstLet;
char secondLet;
char thirdLet;

unsigned long bombTime = 0;               // Time of the bomb activation
const unsigned long bombDuration = 2000;  // Duration before bomb explodes in milliseconds

const int start = 0;
const int one = 1;
const int fillPercent1 = 30;
const int fillPercent2 = 40;
const int fillPercent3 = 50;
const int fillPercent4 = 60;
const int xInitialPos = 1;
const int yInitialPos = 6;
const int twoSecondsDelay = 2000;
const int fourSecondsDelay = 4000;
const int lcdBrightnessAddress = 0;
const int matrixBrightnessAddress = 1;
const int soundAddress = 2;
const int highScoreAddress = 3;
const int playerAddress = 6;
const int minLvl = 0;
const int maxLvl = 5;
const int startLives = 3;
const int first = 0;
const int second = 1;
const int third = 2;
const int fourth = 3;
const int fifth = 4;

// Thresholds for detecting joystick movement
const int minThreshold = 200;
const int maxThreshold = 600;
const int playerBlink = 400;
const int bombBlink = 100;

const byte moveInterval = 100;  // Timing variable to control the speed of LED movement
const byte matrixSize = 8;      // Size of the LED matrix

unsigned long long lastMoved = 0;        // Tracks the last time the LED moved
unsigned long long lastPlayerBlink = 0;  // Tracks the last time the LED for player blinked
unsigned long long lastBombBlink = 0;    // Tracks the last time the LED for a bomb blinked
unsigned long timeTaken;                 //Traks the time from the beggining of the game till the end
unsigned long startTime;                 //Traks the time when the game started
unsigned long previousTime = 0;

bool matrixChanged = true;    // Flag to track if the matrix display needs updating
bool bombPlaced = false;      // Flag to track if the bomb is placed
bool gameLost = false;        // Flag to track if the game is lost
bool gameStarted = false;     // Flag to track if the game started
bool restartMode = false;     // Flag to track if we are in the restart mode after loosing
bool winner = false;          // Flag to track if the game is won
bool sMessage = true;         // Flag to track if the start message displays
bool inMainMenu = true;       //default, flag to track if we are in main menu
bool inHighScore = false;     // Flag to track if we are in high score display
bool inSettings = false;      // Flag to track if we are in settings submenu
bool inAbout = false;         // Flag to track if we are in about option
bool inInfo = false;          // Flag to track if we are in info option
bool lcdSettings = false;     // Flag to track if we are in lcd settings
bool matrixSettings = false;  // Flag to track if we are in matrix settings
bool soundSettings = false;   // Flag to track if we are in sound settings
bool inReset = false;         // Flag to track if we are in reset settings
bool reset = false;           // Flag to track if we reset the highscore
bool matrix1 = false;         // Flag to track if we are in first room
bool matrix2 = false;         // Flag to track if we are in second room
bool matrix3 = false;         // Flag to track if we are in third room
bool matrix4 = false;         // Flag to track if we are in forth room
bool sound = false;           // Flag to track if the sound is on or off
bool nameChange = false;      // Flag to track if the name changed

int lcdBrightnessLevels[] = { 0, 51, 102, 153, 204, 255 };  //lcd brightness levels
int matrixBrightnessLevels[] = { 1, 3, 6, 9, 12, 15 };      //matrix brightness levels

String const mainMenuOptions[] = { "START", "INFO", "SETTINGS", "HIGH SCORE", "ABOUT" };          //main menu options
String const settingsOptions[] = { "LCD BRIGHTNESS", "MATRIX LIGHT", "SOUND", "RESET", "EXIT" };  //settings options
unsigned long int topTimes[] = { 0, 0, 0 };                                                       // Initialize with 0

const int firstPos = 0;
const int numTimes = 3;
const int nameLet = 3;
const int allNamesLet = 9;
const int lastMainPos = sizeof(mainMenuOptions) / sizeof(String) - 1;
const int lastSettingsPos = sizeof(settingsOptions) / sizeof(String) - 1;

// 2D array representing the state of each LED (on/off) in the matrix
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixLvl1[matrixSize][matrixSize] = {
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 1, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1 }
};

byte matrixLvl2[matrixSize][matrixSize] = {
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 0, 0, 1, 1, 1 }
};

byte matrixLvl3[matrixSize][matrixSize] = {
  { 1, 1, 1, 0, 0, 1, 1, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1 }
};

byte matrixLvl4[matrixSize][matrixSize] = {
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 1, 1, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1 }
};

byte happyFace[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte sadFace[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixBomb[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 0, 0, 1, 1, 1, 0 },
  { 1, 1, 0, 1, 0, 0, 0, 1 },
  { 0, 0, 1, 1, 0, 0, 0, 1 },
  { 0, 0, 0, 1, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixBombExplode[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 1, 0, 1, 1, 0, 1 },
  { 0, 0, 1, 1, 0, 1, 1, 0 },
  { 0, 0, 0, 1, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 1, 0, 1, 1 },
  { 0, 1, 1, 0, 1, 0, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 1, 0 }
};

byte matrixBulb[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matSettings[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 1, 1 },
  { 0, 0, 0, 0, 0, 1, 1, 1 },
  { 0, 0, 0, 0, 1, 1, 1, 0 },
  { 1, 1, 1, 1, 1, 1, 0, 0 },
  { 1, 1, 0, 1, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 }
};

byte matrixAbout[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 1, 1, 0, 1 },
  { 1, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixInfo[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 0, 1, 1, 1, 1, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixSound[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixHighScore[matrixSize][matrixSize] = {
  { 1, 1, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 0, 0, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0 }
};

byte matrixReset[matrixSize][matrixSize] = {
  { 1, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 1, 0, 1, 1, 1, 1 },
  { 0, 0, 0, 0, 1, 0, 0, 1 },
  { 0, 0, 0, 0, 1, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 1, 1 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 0, 0, 0 }
};

byte matrixWon1[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixWon2[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 1, 1, 0, 1, 0 },
  { 0, 1, 0, 1, 1, 0, 1, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte matrixWon3[matrixSize][matrixSize] = {
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 0, 1, 0, 1, 1, 0, 1, 0 },
  { 0, 1, 0, 1, 1, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 0, 1, 0, 1, 1, 0, 1, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 }
};

byte full[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000
};

byte empty[8] = {
  0b00000,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte upDown[8] = {
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000,
  0b10001,
  0b01010,
  0b00100
};

void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);  // turn off power saving, enables display
  lc.clearDisplay(0);     // Clear the matrix display

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  lcd.createChar(1, empty);  // empty
  lcd.createChar(2, full);   //full
  lcd.createChar(3, heart);
  lcd.createChar(4, upDown);  // up/down arrows

  //lcdBrightness = EEPROM.read(lcdBrightnessAddress);
  // matrixBrightness = EEPROM.read(matrixBrightnessAddress);

  analogWrite(brightnessPin, lcdBrightnessLevels[lcdBrightness]);
  lc.setIntensity(0, matrixBrightnessLevels[matrixBrightness]);

  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {

  if (inMainMenu == true && sMessage == false) {
    menu();
  }

  if (inSettings == true) {
    settings();
  }

  if (inMainMenu == true && sMessage == true) {
    startMessage();
    lcd.clear();
    sMessage = false;
  }

  if (millis() - lastMoved > moveInterval) {  // Check if it's time to move the LED
    // game logic
    updatePositions();     // Update the position of the LED based on joystick input
    lastMoved = millis();  // Update the time of the last move
  }

  if (matrixChanged == true) {  // Check if the matrix display needs updating
    updateMatrix();             // Update the LED matrix display
    matrixChanged = false;      // Reset the update flag
  }

  reading = digitalRead(pinSW);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {

        if (inMainMenu == true) {
          mainMenu();
        }

        else if (inHighScore == true) {
          lcd.clear();
          menu();
          inHighScore = false;
        }

        else if (inSettings == true) {
          settingsMenu();
        }

        else if (lcdSettings == true) {
          lcd.clear();
          settings();
          lcdSettings = false;
        }

        else if (matrixSettings == true) {
          lcd.clear();
          settings();
          matrixSettings = false;
        }

        else if (soundSettings == true) {
          lcd.clear();
          settings();
          soundSettings = false;
        }

        else if (inReset == true) {
          if (reset == true) {
            lcd.clear();
            clearEEPROM();
            settings();
            inReset = false;
            reset = false;
          } else {
            lcd.clear();
            settings();
            inReset = false;
          }
        }

        else if (inAbout == true) {
          about();
          lcd.clear();
          inMainMenu = true;
        }

        else if (inInfo == true) {
          info();
          lcd.clear();
          inMainMenu = true;
        }

        else if (gameStarted == true) {

          if (numOfBombs < 1) {
            placeBomb(xPos, yPos);  // Place the bomb at the current joystick position
          }
        }

        else if (gameLost == true) {
          lcd.clear();
          clearMatrix();
          lives = startLives;
          inMainMenu = true;
          gameLost = false;
        }

        else if (winner == true) {
          updateTopTimes();
          lcd.clear();
          clearMatrix();
          lives = startLives;
          inMainMenu = true;
          winner = false;
        }
      }
    }
  }

  lastReading = reading;

  // Check for bomb activation
  if (bombPlaced) {
    blinkForBomb();
  }

  if (!bombPlaced) {
    noTone(buzzerPin);
  }

  if (gameLost == false && gameStarted == true) {
    timeTaken = millis() - startTime;

    lcd.setCursor(0, 0);  //title
    lcd.print(F("Lives:"));
    for (int i = 0; i < lives; i++) {
      lcd.write(3);
    }
    lcd.setCursor(0, 1);
    lcd.print(F("Score: "));
    displayTopTimes(timeTaken);
    checkGameWon();
  }

  if (winner == true) {
    gameStarted = false;
    gameWon();
  }

  if (gameLost == true) {
    gameStarted = false;
    if (restartMode == true) {
      lcd.clear();
      clearMatrix();
      gameOverAnimation();
      lcd.setCursor(3, 0);
      lcd.print(F("Game over!"));
      delay(twoSecondsDelay);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(F("Press joystick"));
      lcd.setCursor(0, 1);
      lcd.print(F("to go main menu."));
      clearMatrix();
      restartMode = false;
    }

    gameOver();
  }

  if (gameStarted == true) {

    blinkForPlayer();
  }
}

void menu() {
  inMainMenu = true;

  menuAnimation();

  lcd.setCursor(1, 0);  //title
  lcd.print(F("MAIN MENU:"));

  lcd.setCursor(15, 0);  //directions
  lcd.write(4);

  lcd.setCursor(1, 1);  //current option
  lcd.print(F(">"));
  lcd.print(mainMenuOptions[option]);
}

void mainMenu() {
  //enter in current selected option
  if (option == first)  // start
  {
    //start the game
    startTime = millis();
    Serial.print(startTime);
    inMainMenu = false;
    gameStarted = true;
    inSettings = false;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lives = startLives;
    matrix1 = true;
    clearMatrix();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Lives:"));
    for (int i = 0; i < lives; i++) {
      lcd.write(3);
    }
    lcd.setCursor(0, 1);
    lcd.print(F("Score: "));
    displayTopTimes(timeTaken);

    randomSeed(analogRead(A2));
    generateRoom();
    xPos = xInitialPos;
    yPos = yInitialPos;
    matrixChanged == true;

  } else if (option == second)  // info
  {                             //enter info
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    inInfo = true;
    gameLost = false;
    winner = false;
  } else if (option == third)  // settings
  {                            //enter settings
    inMainMenu = false;
    gameStarted = false;
    inSettings = true;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lcd.clear();
    clearMatrix();
  } else if (option == fourth)  // high score
  {                             //enter high score
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inHighScore = true;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lcd.clear();
    clearMatrix();
    highScore();
  } else if (option == fifth)  // about
  {                            //enter about
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = true;
    inInfo = false;
    gameLost = false;
    winner = false;
  }
}

void settings() {
  inSettings = true;
  settingsAnimation();
  lcd.setCursor(1, 0);  //title
  lcd.print(F("SETTINGS:"));

  lcd.setCursor(15, 0);  //directions
  lcd.write(4);

  lcd.setCursor(1, 1);  //current option
  lcd.print(F(">"));
  lcd.print(settingsOptions[setting]);
}

void settingsMenu() {
  //enter in current selected option
  if (setting == first)  // lcd brightness
  {
    //enter lcd brightness settings
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lcdSettings = true;
    matrixSettings = false;
    lcdBrightnessFunc();

  }

  else if (setting == second)  // matrix light
  {                            //enter matrix lightening settings
    clearMatrix();
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lcdSettings = false;
    matrixSettings = true;
    matrixBrightnessFunc();

  }

  else if (setting == third)  // sound
  {                           //enter sound settings
    clearMatrix();
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lcdSettings = false;
    matrixSettings = false;
    soundSettings = true;
    soundFunc();

  }

  else if (setting == fourth)  // reset highscore
  {                            //enter reset high score settings
    clearMatrix();
    inMainMenu = false;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    lcdSettings = false;
    matrixSettings = false;
    soundSettings = false;
    inReset = true;
    resetHSFunc();

  }

  else if (setting == fifth)  // exit
  {                           //go back to main menu
    inMainMenu = true;
    gameStarted = false;
    inSettings = false;
    inAbout = false;
    inInfo = false;
    gameLost = false;
    winner = false;
    clearMatrix();
  }
}

void lcdBrightnessFunc() {
  int level = lcdBrightness;
  lcd.clear();
  lcd.setCursor(0, 0);  //title
  lcd.print(F("LCD BRIGHTNESS"));

  lcd.setCursor(4, 1);
  lcd.print(F("-"));
  for (int lvl = 0; lvl < 6; lvl++) {
    if (level >= 0) {
      lcd.write(2);
    } else {
      lcd.write(1);
    }
    level--;
  }
  lcd.print(F("+"));

  lcd.setCursor(14, 1);
  lcd.print(F("<"));
  lcd.print(F(">"));

  analogWrite(brightnessPin, lcdBrightnessLevels[lcdBrightness]);  // make change on lcd

  //update in EEPROM

  EEPROM.update(lcdBrightnessAddress, lcdBrightness);
  lcdBrightness = EEPROM.read(lcdBrightnessAddress);
}

void matrixBrightnessFunc() {
  int level = matrixBrightness;
  brightnessAnimation();
  lcd.clear();
  lcd.setCursor(0, 0);  //title
  lcd.print(F("MATRIX LIGHT"));

  lcd.setCursor(4, 1);
  lcd.print(F("-"));
  for (int lvl = 0; lvl < 6; lvl++) {
    if (level >= 0) {
      lcd.write(2);
    } else {
      lcd.write(1);
    }
    level--;
  }
  lcd.print(F("+"));

  lcd.setCursor(14, 1);
  lcd.print(F("<"));
  lcd.print(F(">"));

  lc.setIntensity(0, matrixBrightnessLevels[matrixBrightness]);

  //update in EEPROM

  EEPROM.update(matrixBrightnessAddress, matrixBrightness);
  matrixBrightness = EEPROM.read(matrixBrightnessAddress);
}

void soundFunc() {
  soundAnimation();
  lcd.clear();
  lcd.setCursor(5, 0);  //title
  lcd.print(F("SOUND"));

  lcd.setCursor(4, 1);
  if (sound == true) {
    lcd.print(F("ON"));
    lcd.write(2);
    lcd.write(" ");
    lcd.print(F("OFF"));
    lcd.write(1);
  }

  else {
    lcd.print(F("ON"));
    lcd.write(1);
    lcd.write(" ");
    lcd.print(F("OFF"));
    lcd.write(2);
  }

  lcd.setCursor(14, 1);
  lcd.print(F("<"));
  lcd.print(F(">"));

  //update in EEPROM
  EEPROM.update(soundAddress, sound);
  sound = EEPROM.read(soundAddress);
}

void resetHSFunc() {
  resetAnimation();
  lcd.clear();
  lcd.setCursor(0, 0);  //title
  lcd.print(F("RESET HIGH SCORE"));

  lcd.setCursor(4, 1);
  if (reset == true) {
    lcd.print(F("YES"));
    lcd.write(2);
    lcd.write(" ");
    lcd.print(F("NO"));
    lcd.write(1);
  }

  else {
    lcd.print(F("YES"));
    lcd.write(1);
    lcd.write(" ");
    lcd.print(F("NO"));
    lcd.write(2);
  }

  lcd.setCursor(14, 1);
  lcd.print(F("<"));
  lcd.print(F(">"));
}

void highScore() {
  inHighScore = true;
  highScoreAnimation();
  lcd.setCursor(3, 0);  //title
  lcd.print(F("HIGH SCORE"));

  lcd.setCursor(15, 0);  //directions
  lcd.write(4);

  lcd.setCursor(0, 1);
  lcd.print(index);
  lcd.print(F(". "));
  if (allNames[3 * timeIndex] >= 97 && allNames[3 * timeIndex] <= 122) {
    firstLet = allNames[3 * timeIndex] - 32;
  } else {
    firstLet = allNames[3 * timeIndex];
  }

  if (allNames[3 * timeIndex + 1] >= 97 && allNames[3 * timeIndex + 1] <= 122) {
    secondLet = allNames[3 * timeIndex + 1] - 32;
  } else {
    secondLet = allNames[3 * timeIndex + 1];
  }

  if (allNames[3 * timeIndex + 2] >= 97 && allNames[3 * timeIndex + 2] <= 122) {
    thirdLet = allNames[3 * timeIndex + 2] - 32;
  } else {
    thirdLet = allNames[3 * timeIndex + 2];
  }

  lcd.print(firstLet);
  lcd.print(secondLet);
  lcd.print(thirdLet);
  lcd.print(F(": "));
  displayTopTimes(topTimes[timeIndex]);
}

void saveTopTimesToEEPROM() {
  // Save the top times to EEPROM
  for (int i = 0; i < numTimes; i++) {
    EEPROM.update(highScoreAddress + i, topTimes[i]);
  }
  for (int i = 0; i < allNamesLet; i = i + 3) {
    EEPROM.update(playerAddress + i, allNames[i]);
    EEPROM.update(playerAddress + i + 1, allNames[i + 1]);
    EEPROM.update(playerAddress + i + 2, allNames[i + 2]);
  }
}

void updateTopTimes() {
  // Shift elements to make space for the new time taken
  for (int j = numTimes - 1; j > firstPos; j--) {
    topTimes[j] = topTimes[j - 1];
  }
  topTimes[firstPos] = timeTaken;  // Save the new time taken

  // Shift elements to make space for the new name
  for (int j = allNamesLet - 1; j > firstPos; j = j - 3) {
    allNames[j] = allNames[j - 3];
    allNames[j - 1] = allNames[j - 1 - 3];
    allNames[j - 2] = allNames[j - 2 - 3];
  }
  allNames[firstPos] = firstLet;
  allNames[firstPos + 1] = secondLet;
  allNames[firstPos + 2] = thirdLet;

  saveTopTimesToEEPROM();  // Save updated top times to EEPROM

  // Sort the top times in ascending order
  for (int i = 0; i < numTimes - 1; i++) {
    for (int j = i + 1; j < numTimes; j++) {
      if (topTimes[i] > topTimes[j] && topTimes[j] != 0) {
        unsigned long int temp = topTimes[i];
        topTimes[i] = topTimes[j];
        topTimes[j] = temp;
        for (int k = 0; k < 3; k++) {
          unsigned long int nameSet = allNames[3 * i + k];
          allNames[3 * i + k] = allNames[3 * j + k];
          allNames[3 * j + k] = nameSet;
        }
      }
    }
  }
}

bool newScore() {
  for (int i = 0; i < numTimes; ++i) {
    if (timeTaken < topTimes[i] || topTimes[i] == 0) {  // better than last score
      return true;
    }
  }
  return false;
}

void setName() {
  nameChange = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set name & press");

  lcd.setCursor(5, 1);

  if (currentLetter == first) {  //keep the current letter big and the rest small
    letter1 = name[0];
    letter2 = name[1] + 32;
    letter3 = name[2] + 32;

  } else if (currentLetter == second) {  //keep the current letter big and the rest small

    letter1 = name[0] + 32;
    letter2 = name[1];
    letter3 = name[2] + 32;
  } else {  //keep the current letter big and the rest small

    letter1 = name[0] + 32;
    letter2 = name[1] + 32;
    letter3 = name[2];
  }

  firstLet = letter1;  //convert to String
  secondLet = letter2;
  thirdLet = letter3;

  lcd.print(F("<"));
  lcd.print(firstLet);
  lcd.print(secondLet);
  lcd.print(thirdLet);
  lcd.print(F(">"));
}


void displayTopTimes(unsigned long int time) {
  // Display the top times
  // unsigned long int time = topTimes[i];
  unsigned long int minutes = time / 60000;  // Convert milliseconds to minutes
  time %= 60000;
  unsigned long int seconds = time / 1000;  // Convert remaining milliseconds to seconds
  time = time % 1000;
  unsigned long int milliseconds = time / 10;  // Remaining milliseconds

  lcd.print(minutes);
  lcd.print(F(":"));

  if (seconds < 10) {
    lcd.print(F("0"));  // Add leading zero if seconds < 10
  }

  lcd.print(seconds);
  lcd.print(F(":"));

  if (milliseconds < 10) {
    lcd.print(F("0"));  // Add leading zeros if milliseconds < 10
  }

  lcd.print(milliseconds);
}

void about() {
  clearMatrix();
  aboutAnimation();
  int scrollDelay = 350;
  int characters = 25;
  int oneSecondDelay = 1000;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Title: BombMania!"));  //title
  lcd.setCursor(1, 1);
  lcd.print(F("By Lemnaru Amedeea, Git: lemnaruamedeea"));

  for (int i = 0; i < characters; i++) {  // autoscroll to left

    lcd.scrollDisplayLeft();

    delay(scrollDelay);
  }

  delay(oneSecondDelay);
  inMainMenu = true;
  inAbout = false;
}

void info() {
  clearMatrix();
  infoAnimation();
  int scrollDelay = 350;
  int characters = 25;
  int oneSecondDelay = 1000;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Explode the walls as fast as you can!"));  //title
  lcd.setCursor(1, 1);
  lcd.print(F("Don't let the bomb chatches you!"));

  for (int i = 0; i < characters; i++) {  // autoscroll to left

    lcd.scrollDisplayLeft();

    delay(scrollDelay);
  }

  delay(oneSecondDelay);
  inMainMenu = true;
  inInfo = false;
}

void startMessage() {
  menuAnimation();
  int characters = 21;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("BombMania!"));
  lcd.setCursor(0, 1);
  lcd.print(F("Let's bomb this!"));

  delay(fourSecondsDelay);
}

void placeBomb(int x, int y) {
  // Place the bomb at the specified position
  numOfBombs = 1;
  bombX = x;
  bombY = y;
  bombPlaced = true;
  bombTime = millis();  // Start the timer for bomb activation
}

void explode() {
  if (bombPlaced) {
    // Explode the walls adjacent to the bomb
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        checkX = bombX + i;
        checkY = bombY + j;

        // Check if the position is within the matrix bounds
        if (checkX >= 0 && checkX < matrixSize && checkY >= 0 && checkY < matrixSize) {

          if (matrix[checkX][checkY] == 1 && (checkX == 0 || checkY == 0 || (checkX == matrixSize - 1) || (checkY == matrixSize - 1))) {
            matrix[checkX][checkY] = 1;  // don't destroy the wall
          } else {
            matrix[checkX][checkY] = 0;  // Destroy the wall
          }
        }

        if (checkX == xPos && checkY == yPos) {
          // Player loses a live if the bomb catches him

          lives--;
          lcd.clear();
          lcd.setCursor(0, 0);  //title
          lcd.print(F("Lives:"));
          for (int i = 0; i < lives; i++) {
            lcd.write(3);
          }
          lcd.setCursor(0, 1);
          lcd.print(F("Score: "));
          displayTopTimes(timeTaken);
          // Player loses the game if he has no more lives
          if (lives == 0) {
            gameLost = true;
          }

          restartMode = true;
        }
      }
      numOfBombs = 0;
    }

    matrix[bombX][bombY] = 0;  // Clear the bomb position
    bombPlaced = false;        // Reset bomb state
    matrixChanged = true;      // Set the flag to update the matrix display
  }
}

void blinkForPlayer() {

  if (millis() - lastPlayerBlink > playerBlink) {  // Check if it's time to blink for player
    playerLedState = !playerLedState;
    matrix[xPos][yPos] = playerLedState;
    lastPlayerBlink = millis();  // Update the time of the last blink for player
    matrixChanged = true;
  }
}

void blinkForBomb() {
  unsigned long currentTime = millis();

  if (millis() - bombTime < bombDuration) {

    previousTime = currentTime;

    if (sound) {
      if (currentFrequency < numFrequencies) {
        tone(buzzerPin, frequencies[currentFrequency]);
        currentFrequency++;
      } else {
        // Reset to the beginning of the frequencies array
        currentFrequency = 0;
      }
    }

    if (millis() - lastBombBlink >= bombBlink) {  // Check if it's time to blink for the bomb
      bombLedState = !bombLedState;
      matrix[bombX][bombY] = bombLedState;
      lastBombBlink = millis();  // Update the time of the last blink for the bomb
      matrixChanged = true;
    }

  }

  else {
    explode();
  }
}

void generateRoom() {
  // Reset the entire matrix
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix[row][col] = 0;  // Set all positions as empty initially
    }
  }

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {

      if (matrix1) {
        matrix[row][col] = matrixLvl1[row][col];
      }

      else if (matrix2) {
        matrix[row][col] = matrixLvl2[row][col];
      }

      else if (matrix3) {
        matrix[row][col] = matrixLvl3[row][col];
      }

      else if (matrix4) {
        matrix[row][col] = matrixLvl4[row][col];
      }
    }
  }

  if (matrix1) {
    totalElements = (matrixSize - 2) * (matrixSize - 2);
    numWalls = (fillPercent1 * totalElements) / 100;

    for (int i = 0; i < numWalls; i++) {
      randomRow = random(1, matrixSize - 1);  // Generate random row index
      randomCol = random(1, matrixSize - 2);  // Generate random column index
      matrix[randomRow][randomCol] = 1;       // Set the position as a wall
    }
  }

  else if (matrix2) {
    totalElements = (matrixSize - 2) * (matrixSize - 2);
    numWalls = (fillPercent2 * totalElements) / 100;

    for (int i = 0; i < numWalls; i++) {
      randomRow = random(1, matrixSize - 1);  // Generate random row index
      randomCol = random(1, matrixSize - 2);  // Generate random column index
      matrix[randomRow][randomCol] = 1;       // Set the position as a wall
    }
  }

  else if (matrix3) {
    totalElements = (matrixSize - 2) * (matrixSize - 2);
    numWalls = (fillPercent3 * totalElements) / 100;

    for (int i = 0; i < numWalls; i++) {
      randomRow = random(2, matrixSize - 1);  // Generate random row index
      randomCol = random(1, matrixSize - 1);  // Generate random column index
      matrix[randomRow][randomCol] = 1;       // Set the position as a wall
    }
  }

  else if (matrix4) {
    totalElements = (matrixSize - 2) * (matrixSize - 2);
    numWalls = (fillPercent4 * totalElements) / 100;

    for (int i = 0; i < numWalls; i++) {
      randomRow = random(1, matrixSize - 1);  // Generate random row index
      randomCol = random(2, matrixSize - 1);  // Generate random column index
      matrix[randomRow][randomCol] = 1;       // Set the position as a wall
    }
  }

  matrixChanged = true;
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);  // set each led individually
    }
  }
}
// Function to read joystick input and update the position of the LED
void updatePositions() {
  counter = 0;
  for (int row = 1; row < matrixSize - 1; row++) {
    for (int col = 1; col < matrixSize - 1; col++) {
      if (matrix[row][col] == 0) {
        counter++;
      }
    }
  }
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  // Store the last positions of the LED
  xLastPos = xPos;
  yLastPos = yPos;
  // Update xPos based on joystick movement (X-axis)
  if (xValue < minThreshold) {

    if (lcdSettings == true) {  //move the level bar to left

      if (lcdBrightness > minLvl) {
        lcdBrightness--;
        lcdBrightnessFunc();
      }
    }

    else if (matrixSettings == true) {  //move the level bar to left

      if (matrixBrightness > minLvl) {
        matrixBrightness--;
        Serial.println(matrixBrightness);
        matrixBrightnessFunc();
      }
    }

    else if (soundSettings == true) {  //move from off to on

      if (!sound) {
        sound = true;
        soundFunc();
      }
    }

    else if (inReset == true) {  //move from no to yes

      if (!reset) {
        reset = true;
        resetHSFunc();
      }
    }

    else if (gameStarted == true) {  //Left

      if (xPos > start && matrix[xPos - one][yPos] == 0) {
        xPos--;
      }
    }

    else if (winner && newScore() == true) {
      if (currentLetter > 0) {
        currentLetter--;
        nameChange = true;
        setName();
      }
    }
  }

  if (xValue > maxThreshold) {

    if (lcdSettings == true) {  //move the level bar to right

      if (lcdBrightness < maxLvl) {
        lcdBrightness++;
        lcdBrightnessFunc();
      }
    }

    else if (matrixSettings == true) {  //move the level bar to right

      if (matrixBrightness < maxLvl) {
        matrixBrightness++;
        Serial.println(matrixBrightness);
        matrixBrightnessFunc();
      }
    }

    else if (soundSettings == true) {  //move from on to off

      if (sound) {
        sound = false;
        soundFunc();
      }
    }

    else if (inReset == true) {  //move from yes to no

      if (reset) {
        reset = false;
        resetHSFunc();
      }
    }

    else if (gameStarted == true) {  //right

      if (xPos < matrixSize - 1 && matrix[xPos + one][yPos] == 0) {
        xPos++;
      }

      else if (xPos == matrixSize - 1 && !bombPlaced) {

        if (matrix2 && counter == (matrixSize - 2) * (matrixSize - 2)) {
          matrix2 = false;
          matrix3 = true;
          generateRoom();
          xPos = start;
        }
      }
    }

    else if (winner && newScore() == true) {
      if (currentLetter < 2) {
        currentLetter++;
        nameChange = true;
        setName();
      }
    }
  }

  if (yValue > maxThreshold) {

    if (inMainMenu == true) {  //in main menu

      if (option < lastMainPos) {  //UP
        option++;
        lcd.clear();
        menu();
      }

    }

    else if (inSettings == true) {  //in settings

      if (setting < lastSettingsPos) {  // UP
        setting++;
        lcd.clear();
        settings();
      }

    }

    else if (inHighScore == true) {  //in high score

      if (index < numTimes) {  // UP
        timeIndex++;
        index++;
        lcd.clear();
        highScore();
      }

    }

    else if (gameStarted == true) {  //down

      if (yPos < matrixSize - 1 && matrix[xPos][yPos + one] == 0) {
        yPos++;
      }

      else if (yPos == matrixSize - 1 && !bombPlaced) {

        if (matrix3 && counter == (matrixSize - 2) * (matrixSize - 2)) {
          matrix3 = false;
          matrix4 = true;
          generateRoom();
          yPos = start;
        }
      }
    }

    else if (winner && newScore() == true) {
      if (name[currentLetter] < 90) {  // Z code in ASCII
        name[currentLetter]++;
        nameChange = true;
      } else {
        name[currentLetter] = 65;  //A
        nameChange = true;
      }
      setName();
    }
  }

  if (yValue < minThreshold) {

    if (inMainMenu == true) {  //in main menu

      if (option > firstPos) {  //DOWN
        option--;
        lcd.clear();
        menu();
      }

    }

    else if (inSettings == true) {  //in setting

      if (setting > firstPos) {  //DOWN
        setting--;
        lcd.clear();
        settings();
      }

    }

    else if (inHighScore == true) {  //in high score

      if ((index - 1) > firstPos) {  // down
        index--;
        timeIndex--;
        lcd.clear();
        highScore();
      }

    }

    else if (gameStarted == true) {  // up

      if (yPos > start && matrix[xPos][yPos - one] == 0) {
        yPos--;
      }

      else if (yPos == start && !bombPlaced) {

        if (matrix1 && (counter == (matrixSize - 2) * (matrixSize - 2))) {
          matrix1 = false;
          matrix2 = true;
          generateRoom();
          yPos = matrixSize - 1;
        }
      }
    }

    else if (winner && newScore() == true) {
      if (name[currentLetter] > 65) {  // A code in ASCII
        name[currentLetter]--;
        nameChange = true;
      } else {
        name[currentLetter] = 90;  //Z
        nameChange = true;
      }
      setName();
    }
  }
  // Check if the position has changed and update the matrix if necessary
  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    if ((yPos == matrixSize - 1 && yLastPos == start) || (xPos == matrixSize - 1 && xLastPos == start) || (yLastPos == matrixSize - 1 && yPos == start) || (xLastPos == matrixSize - 1 && xPos == start)) {
      matrix[xLastPos][yLastPos] = 1;
      matrix[xPos][yPos] = 1;
    }

    else {
      matrix[xLastPos][yLastPos] = 0;
      matrix[xPos][yPos] = 1;
    }
  }
}

void clearMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix[row][col] = 0;
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void clearEEPROM() {
  for (int i = timeIndex; i < numTimes; i++) {
    topTimes[i] = 0;  // reset the high sores vectors
  }

  for (int i = firstPos; i < nameLet; i++) {
    name[i] = 0;  // reset the high sores vectors
  }

  for (int i = firstPos; i < allNamesLet; i++) {
    allNames[i] = 97;  // reset the name vector
  }

  for (int i = highScoreAddress; i < highScoreAddress + numTimes; i++) {
    EEPROM.update(i, 0);  // Write 0 to each address in the EEPROM
  }

  for (int i = playerAddress; i < playerAddress + allNamesLet; i++) {
    EEPROM.update(i, 0);  // Write 0 to each address in the EEPROM
  }
}

void checkGameWon() {
  int count = 0;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == 0) {
        count++;
      }
    }
  }

  if (matrix4) {
    if (count == (matrixSize - 2) * (matrixSize - 2) + 2) {
      startTime = 0;
      newScore();
      lcd.clear();
      clearMatrix();
      gameWonAnimation();
      lcd.setCursor(4, 0);
      lcd.print(F("You won!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Score: "));
      displayTopTimes(timeTaken);
      delay(fourSecondsDelay);
      if (newScore() == true) {
        nameChange = true;
        lcd.clear();
        highScoreAnimation();
        lcd.setCursor(1, 0);
        lcd.print(F("New HighScore!"));
        delay(twoSecondsDelay);
        currentLetter = 0;  //reset the name
        name[0] = 65;
        name[1] = 65;
        name[2] = 65;
        setName();
      } else {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print(F("Press joystick"));
        lcd.setCursor(0, 1);
        lcd.print(F("to go main menu."));
      }
      winner = true;
      matrix4 = false;
    }
  }
}

void gameWon() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, happyFace[row][col]);
    }
  }
}

void gameWonAnimation() {
  int delayAnimation = 20;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixWon3[row][col]);
      delay(delayAnimation);
    }
  }
}

void gameOver() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, sadFace[row][col]);
    }
  }
}

void gameOverAnimation() {
  int fifty = 50;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true);
      delay(fifty);
    }
  }
}

void menuAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixBomb[row][col]);
    }
  }
}

void aboutAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixAbout[row][col]);
    }
  }
}

void infoAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixInfo[row][col]);
    }
  }
}

void settingsAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matSettings[row][col]);
    }
  }
}

void brightnessAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixBulb[row][col]);
    }
  }
}

void soundAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixSound[row][col]);
    }
  }
}

void highScoreAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixHighScore[row][col]);
    }
  }
}

void resetAnimation() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixReset[row][col]);
    }
  }
}