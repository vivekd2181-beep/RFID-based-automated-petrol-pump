#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

// RFID pins
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Output pins
int pump   = 8;
int buzzer = A0;

// Button pins
int btn50  = A1;
int btn100 = A2;
int btn150 = A3;
int btn200 = A4;

// Selected amount
int selectedAmount = 0;

// ---- CHANGE THIS TO YOUR CARD UID ----
// Example 4-byte UID (you MUST replace with your card UID)
byte validCard[4] = {0xDE, 0xAD, 0xBE, 0xEF};
// --------------------------------------

// Pump times for each amount (in milliseconds)
// Adjust based on your pump speed
int time50  = 5000;   // 5 seconds for Rs 50
int time100 = 10000;  // 10 seconds for Rs 100
int time150 = 15000;  // 15 seconds for Rs 150
int time200 = 20000;  // 20 seconds for Rs 200

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.begin(16, 2);

  pinMode(pump, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(pump, LOW);

  // Buttons with internal pull-up
  pinMode(btn50,  INPUT_PULLUP);
  pinMode(btn100, INPUT_PULLUP);
  pinMode(btn150, INPUT_PULLUP);
  pinMode(btn200, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("RFID PETROL PUMP");
  lcd.setCursor(0, 1);
  lcd.print("Select Amt & Scan");
  delay(2000);
  lcd.clear();
  showSelection();
}

void loop() {
  // 1) Check buttons for amount selection
  checkButtons();

  // 2) Check for RFID card
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial())  return;

  lcd.clear();
  lcd.print("Card Detected");
  tone(buzzer, 2000, 100);
  delay(1000);

  // Check if it is the valid card
  if (compareUID(rfid.uid.uidByte, validCard)) {
    if (selectedAmount == 0) {
      lcd.clear();
      lcd.print("Select amount");
      lcd.setCursor(0, 1);
      lcd.print("before scanning");
      tone(buzzer, 1500, 500);
      delay(2500);
    } else {
      // Valid card and amount selected
      int pumpTime = getPumpTime(selectedAmount);

      if (pumpTime > 0) {
        lcd.clear();
        lcd.print("Amt: Rs ");
        lcd.print(selectedAmount);
        delay(1500);

        lcd.clear();
        lcd.print("Dispensing...");
        digitalWrite(pump, HIGH);
        delay(pumpTime);
        digitalWrite(pump, LOW);

        tone(buzzer, 2500, 150);
        lcd.clear();
        lcd.print("Done: Rs ");
        lcd.print(selectedAmount);
        delay(3000);
      } else {
        lcd.clear();
        lcd.print("Invalid amount");
        tone(buzzer, 1000, 500);
        delay(2000);
      }
    }
  } else {
    lcd.clear();
    lcd.print("Invalid Card");
    tone(buzzer, 1500, 800);
    delay(2000);
  }

  lcd.clear();
  showSelection(); // show current selected amount

  // Halt PICC
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// ---------------- Helper Functions ----------------

// Check which button is pressed to set amount
void checkButtons() {
  if (digitalRead(btn50) == LOW) {
    selectedAmount = 50;
    beepAndShow();
  } 
  else if (digitalRead(btn100) == LOW) {
    selectedAmount = 100;
    beepAndShow();
  } 
  else if (digitalRead(btn150) == LOW) {
    selectedAmount = 150;
    beepAndShow();
  } 
  else if (digitalRead(btn200) == LOW) {
    selectedAmount = 200;
    beepAndShow();
  }
}

// Small function to refresh LCD when amount changes
void beepAndShow() {
  tone(buzzer, 1800, 100);
  lcd.clear();
  showSelection();
  delay(300);   // debounce delay
}

// Display selected amount on LCD
void showSelection() {
  lcd.setCursor(0, 0);
  lcd.print("Select Amount:");
  lcd.setCursor(0, 1);
  if (selectedAmount == 0) {
    lcd.print("None (press btn)");
  } else {
    lcd.print("Rs ");
    lcd.print(selectedAmount);
  }
}

// Compare scanned UID to stored UID
boolean compareUID(byte *scanUID, byte *storedUID) {
  for (byte i = 0; i < 4; i++) {
    if (scanUID[i] != storedUID[i]) {
      return false;
    }
  }
  return true;
}

// Convert amount to pump time
int getPumpTime(int amount) {
  switch (amount) {
    case 50:  return time50;
    case 100: return time100;
    case 150: return time150;
    case 200: return time200;
    default:  return 0;
  }
}