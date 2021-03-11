#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>

byte s1[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

byte s2[] = {
  B10001,
  B01010,
  B00100,
  B00000,
  B10001,
  B01010,
  B00100,
  B00000
};

byte s3[] = {
  B10101,
  B01110,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

byte s4[] = {
  B00000,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000,
  B00000,
};

byte s5[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B00000
};

byte s6[] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01110,
  B00000
};

byte s7[] = {
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B00000
};

byte s8[] = {
  B00000,
  B00100,
  B01110,
  B11011,
  B10001,
  B00000,
  B00000,
  B00000
};

byte s9[] = {
  B00100,
  B01010,
  B10001,
  B10001,
  B10001,
  B01010,
  B00100,
  B00000
};

byte s10[] = {
  B11100,
  B11100,
  B01000,
  B11100,
  B11110,
  B10110,
  B10111,
  B00000
};

byte s11[] = {
  B00100,
  B01110,
  B11011,
  B10101,
  B11111,
  B10101,
  B10101,
  B00000
};

byte ch[] = {
  B10001,
  B10001,
  B10001,
  B01111,
  B00001,
  B00001,
  B00001,
  B00000
};

// Keyboard prop
const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'A','B','C'},
  {'D','E','F'},
  {'G','H','O'},
  {'L','W','?'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; 
byte colPins[COLS] = {4, 3, 2}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD prop
#define LCD_CONTRAST_PIN 10
#define LCD_CONTRAST 100
#define LCD_BRIGHT_PIN 9
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

//protocol 
#define CONN_BOUND 9600 //19200
#define BEGIN_CHAR '$'
#define END_CHAR ';'
#define RESET_CHAR '&'
#define LINE_SEP '#'
#define SCHAR_INIT '^'
#define ARDUINO_INIT "@INIT"
#define SIG_OK "@OK"
String buff = ""; 
byte curSlot = 0;

//header
void sendComandToPC(String command);
void sendComandToPC(char command);
void printBuffer();
void printSpecialChar(char value, byte c_col, byte c_row);
boolean readCommand();


// ===== MAIN =====

void setup() {
  Serial.begin(CONN_BOUND);
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(LCD_CONTRAST_PIN, OUTPUT);
  analogWrite(LCD_CONTRAST_PIN, LCD_CONTRAST);
  pinMode(LCD_BRIGHT_PIN, OUTPUT);
  digitalWrite(LCD_BRIGHT_PIN, HIGH);
  sendComandToPC(ARDUINO_INIT);
}

void loop(){
  char key = keypad.getKey();
  if (readCommand()) {
    printBuffer();
  } 
  if (key) {
    sendComandToPC(key);
  }
}


// ===== IMPL =====

void sendComandToPC(String command) {
  Serial.print(BEGIN_CHAR);
  Serial.print(command);
  Serial.print(END_CHAR);
}

void sendComandToPC(char command) {
  Serial.print(BEGIN_CHAR);
  Serial.print(command);
  Serial.print(END_CHAR);
}

boolean readCommand() {
  static bool parseStart = false;
  if (Serial.available()) {
    char in = Serial.read();    
    if (in == '\n' || in == '\r') return false; 
    if (in == RESET_CHAR) {
      buff = "";
      parseStart = false;
      return false;
    }
    if (in == END_CHAR) { 
      parseStart = false;
      return true;
    }    
    if (in == BEGIN_CHAR) {  
      parseStart = true;
      buff = "";
      return false;
    }    
    if (parseStart) {       
      buff += in;
    }
  }
  return false;
}

void printBuffer() {
  lcd.clear();
  byte c_row = 0;
  byte c_col = 0;
  int maxIndex = buff.length();
  for (int i = 0; i < maxIndex; i++) {
    if (buff[i] == LINE_SEP) {
      lcd.setCursor(0, 1);
      c_row = 1;
      c_col = 0;
    } else if (buff[i] == SCHAR_INIT) {
      i++;
      printSpecialChar(buff[i], c_col, c_row);
      c_col++;
    } else {
      lcd.print(buff[i]);
      c_col++;
    }
  }
  sendComandToPC(SIG_OK);
}

void printSpecialChar(char value, byte c_col, byte c_row) {
  if (value == 'A') {
    lcd.createChar(curSlot, s1);
  } else if (value == 'B') {
    lcd.createChar(curSlot, s2);
  } else if (value == 'C') {
    lcd.createChar(curSlot, s3);
  } else if (value == 'D') {
    lcd.createChar(curSlot, s4);
  } else if (value == 'E') {
    lcd.createChar(curSlot, s5);
  } else if (value == 'F') {
    lcd.createChar(curSlot, s6);
  } else if (value == 'G') {
    lcd.createChar(curSlot, s7);
  } else if (value == 'H') {
    lcd.createChar(curSlot, s8);
  } else if (value == 'O') {
    lcd.createChar(curSlot, s9);
  } else if (value == 'L') {
    lcd.createChar(curSlot, s10);
  } else if (value == 'W') {
    lcd.createChar(curSlot, s11);
  } else if (value == 'Y') {
    lcd.createChar(curSlot, ch);
  } else {
    return;
  }
  lcd.setCursor(c_col, c_row);
  lcd.write(curSlot);
  curSlot++;
  if (curSlot > 7) curSlot = 0;
}

// $^A^W#C^YET: 100;
// $^A^W#SCORE: 100;