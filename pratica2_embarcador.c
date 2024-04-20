/*****************************************************************************
* Matrix Keypad 16 Keys
* Code to display the pressed keys on the serial monitor of the IDE.
*
* Author: Profa. Kelen C. Teixeira Vivaldini
* Modification: Kleber e Lucas
* DC/UFSCar
*
*******************************************************************************/

#include <Keypad.h> // Library for keypad
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#define SERVO_PIN 11 
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2
#define SERVO_LOCK_POS   0   // Pose 0 degree 
#define SERVO_UNLOCK_POS 90  // Pose 90 degree
Servo servoMotor; // Create a servo object


LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

const byte lines = 4; // Number of keypad rows
const byte columns = 4; // Number of keypad columns

const char KEYPAD_MATRIX[lines][columns] = { // Keypad character matrix (keypad mapping)
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte ROW_PINS[lines] = {9, 8, 7, 6}; // Connection pins for keypad rows
const byte COLUMN_PINS[columns] = {5, 4, 3, 2}; // Connection pins for keypad columns

Keypad custom_keypad = Keypad(makeKeymap(KEYPAD_MATRIX), ROW_PINS, COLUMN_PINS, lines, columns); // Initialize keypad

void setup() {
  Serial.begin(9600); // Initialize serial port
  lcd.init();
  lcd.backlight();
  servoMotor.attach(SERVO_PIN); // Attach servo to the defined pin
  servoMotor.write(SERVO_LOCK_POS); // Set initial position of the servo

}

void loop() {
  int i = 0;
  int j = 0;
  char password[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};;
  char reset_password = 'R';
  //Serial.println(i);
  reset_password = custom_keypad.getKey();
  //Serial.println((strcmp(key_print, 'A')));
  
  if(strcmp(reset_password, 'A') == 0)
  { 
    while(i<5)
    {
      lcd.setCursor(1, 0);
      lcd.print("Insert the new password");
      lcd.setCursor(1, 1);
      lcd.print("password:");

      password[i] = custom_keypad.getKey();
      lcd.setCursor(10+i,1);
      lcd.print("*");
      if(password[i])
      {
        i++;
      }
    }
    reset_password = 'K';
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Senha Atualizada");
    Serial.println(password); // Print the pressed key to the serial monitor
  }
  
  if(reset_password == 'K')  {
    char inserted_password[6] = {'\0', '\0', '\0', '\0', '\0', '\0'}; 
    while(j < 5)
    { 
      inserted_password[j] = custom_keypad.getKey();
      if(inserted_password[j])
      {
        j++;
      }
    }

    bool passwordsMatch = true;
    for (int k = 0; k < 5; k++) {
      if (password[k] != inserted_password[k]) {
        passwordsMatch = false;
        break; // Exit the loop early if a mismatch is found
      }
    }

    if(passwordsMatch)
    {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Senha Correta");
      servoMotor.write(SERVO_UNLOCK_POS);
      delay(500); // Wait for 0.5 seconds
    }
    else
    {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Senha Incorreta");
      servoMotor.write(SERVO_LOCK_POS);
      delay(500);
    }
  }
}
