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
    }
    if (key == 'A' && state == CAMERA) {
      state = ENTER_DATE;
    }
    
    if (key == 'A' && state == ENTER_DATE) {
      state = START_MENU;
      int resetVals[8] = {2, 0, -1, -1, -1, -1, -1, -1};

      for (int i = 0; i < 8; i++) {
          date[i] = resetVals[i];
      }
    }


    // switch (key) {
    //   case '1':
    //   Serial.println('1');
    //   break;
    //   case '2':
    //   Serial.println('2');
    //   break;
    //   case '3':
    //   Serial.println('3');
    //   break;
    //   case '4':
    //   Serial.println('4');
    //   break;
    //   case '5':
    //   Serial.println('5');
    //   break;
    //   case '6':
    //   Serial.println('6');
    //   break;
    //   case '7':
    //   Serial.println('7');
    //   break;
    //   case '8':
    //   Serial.println('8');
    //   break;
    //   case '9':
    //   Serial.println('9');
    //   break;
    //   case '0':
    //   Serial.println('0');
    //   break;
    //   case 'A':
    //   Serial.println('A');
    //   break;
    //   case 'B':
    //   Serial.println('B');
    //   break;
    //   case 'C':
    //   Serial.println('C');
    //   break;
    //   case 'D':
    //   Serial.println('D');
    //   break;
    //   case '#':
    //   Serial.println('#');
    //   break;
    //   case '*':
    //   Serial.println('*');
    //   break;
    // }

  }

  lastKey = key;

  delay(50);
}