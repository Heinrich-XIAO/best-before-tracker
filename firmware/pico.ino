const uint8_t ROWS = 4;
const uint8_t COLS = 4;

const uint8_t rowPins[ROWS] = {7, 6, 5, 4};
const uint8_t colPins[COLS] = {3, 2, 1, 0};

const char keymap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void setup() {
  Serial1.setTX(12);  // GP12
  Serial1.setRX(13);  // GP13
  Serial1.begin(115200);
  Serial.begin(115200);

  // Rows are outputs
  for (int r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  // Columns are inputs with pullups
  for (int c = 0; c < COLS; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }

  Serial.println("Ready");
}

char scanKeypad() {
  for (int r = 0; r < ROWS; r++) {
    // Activate this row
    digitalWrite(rowPins[r], LOW);

    delayMicroseconds(5);

    for (int c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        digitalWrite(rowPins[r], HIGH);
        return keymap[r][c];
      }
    }

    digitalWrite(rowPins[r], HIGH);
  }

  return '\0';
}

int date[8] = {2, 0, -1, -1, -1, -1, -1, -1};

#define START_MENU 0
#define CAMERA 1
#define ENTER_DATE 2

int state = START_MENU;

void enterDate(char key) {
  int digit = key - '0';
  int i = 0;
  while (date[i] != -1) {
    i++;
  }
  date[i] = digit;

  for (int i = 0; i < 8; i++) {
    if (date[i] != -1) {
      Serial.print(date[i]);
      if (i == 3 || i == 5) {
        Serial.print('-');
      }
    }
  }
  Serial.println("");
}

void loop() {
  static char lastKey = '\0';

  char key = scanKeypad();

  if (key != '\0' && key != lastKey) {
    if (key >= '0' && key <= '9' && state == ENTER_DATE) {
      enterDate(key);
    }

    if (key == 'D' && state == ENTER_DATE && date[2] != -1) {
      int i = 7;
      while (date[i] == -1) {
        i--;
      }
      date[i] = -1; 


      for (int i = 0; i < 8; i++) {
        if (date[i] != -1) {
          Serial.print(date[i]);
          if (i == 3 || i == 5) {
            Serial.print('-');
          }
        }
      }
      Serial.println("");
    }

    if (key == 'A' && state == START_MENU) {
      state = CAMERA;
      Serial1.write(0x01);
    }
    if (key == 'A' && state == CAMERA) {
      state = ENTER_DATE;
      Serial1.print("SHOOT");
    }
    
    if (key == 'A' && state == ENTER_DATE) {
      state = START_MENU;
      int resetVals[8] = {2, 0, -1, -1, -1, -1, -1, -1};

      for (int i = 0; i < 8; i++) {
          date[i] = resetVals[i];
      }
    }
  }

  lastKey = key;

  delay(50);
}