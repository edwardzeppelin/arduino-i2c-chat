// M_3 - Slave device для Arduino Mega (Адрес 3)
// Идентичен M_2
#include <Wire.h>

#define MY_ADDRESS 2
#define M1_ADDRESS 1
#define M3_ADDRESS 3

String inputString = "";
boolean stringComplete = false;

void setup() {
  Serial.begin(9600);
  Wire.begin(MY_ADDRESS);
  Wire.onReceive(receiveEvent);
  inputString.reserve(200);
  
  Serial.println("=== ЧАТ - ПОЛЬЗОВАТЕЛЬ 2 ===");
  Serial.println("Формат: @номер сообщение (например: @1 привет)");
  Serial.println("Доступные получатели: @1, @3");
  Serial.println("Просто текст - отправить всем");
  Serial.println("Введите сообщение:");
}

void loop() {
  if (stringComplete) {
    processInput();
    inputString = "";
    stringComplete = false;
  }
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void processInput() {
  inputString.trim();
  if (inputString.length() == 0) return;
  
  if (inputString.startsWith("@")) {
    int spaceIndex = inputString.indexOf(' ');
    if (spaceIndex != -1) {
      String recipientStr = inputString.substring(1, spaceIndex);
      String message = inputString.substring(spaceIndex + 1);
      int recipient = recipientStr.toInt();
      
      if (recipient == 1 || recipient == 3) {
        sendMessage(recipient, message);
        Serial.print(">>> Отправлено пользователю ");
        Serial.print(recipient);
        Serial.print(": ");
        Serial.println(message);
      } else {
        Serial.println("Ошибка: Неверный номер получателя. Используйте @1 или @3");
      }
    } else {
      Serial.println("Ошибка: Неверный формат. Используйте: @номер сообщение");
    }
  } else {
    // Broadcast to all
    sendMessage(M1_ADDRESS, inputString);
    sendMessage(M3_ADDRESS, inputString);
    Serial.print(">>> Broadcast: ");
    Serial.println(inputString);
  }
  Serial.print("Введите сообщение: ");
}

void sendMessage(int recipient, String message) {
  Wire.beginTransmission(recipient);
  Wire.write(MY_ADDRESS); // Отправитель
  Wire.write(':'); // Разделитель
  for (int i = 0; i < message.length(); i++) {
    Wire.write(message[i]);
  }
  Wire.write('\0'); // Конец сообщения
  Wire.endTransmission();
}

void receiveEvent(int howMany) {
  String message = "";
  int sender = Wire.read();
  char separator = Wire.read(); // Читаем разделитель ':'
  
  while (Wire.available()) {
    char c = Wire.read();
    if (c != '\0') {
      message += c;
    }
  }
  
  Serial.println();
  Serial.print("<<< Сообщение от пользователя ");
  Serial.print(sender);
  Serial.print(": ");
  Serial.println(message);
  Serial.print("Введите сообщение: ");
}
