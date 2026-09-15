// M_1 - Master device для Arduino Mega (Адрес 1)
#include <Wire.h>

#define M1_ADDRESS 1
#define M2_ADDRESS 2
#define M3_ADDRESS 3

String inputString = "";
boolean stringComplete = false;
int currentRecipient = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(M1_ADDRESS);
  inputString.reserve(200);
  Wire.onReceive(receiveEvent);
  Serial.println("=== МНОГОПОЛЬЗОВАТЕЛЬСКИЙ ЧАТ - ПОЛЬЗОВАТЕЛЬ 1 ===");
  Serial.println("Формат сообщения: @номер_получателя сообщение");
  Serial.println("Доступные получатели:");
  Serial.println("  @2 - Отправить пользователю 2");
  Serial.println("  @3 - Отправить пользователю 3");
  Serial.println("Просто текст - отправить всем");
  Serial.println("Введите сообщение:");
}

void loop() {
  // Обработка ввода от пользователя
  if (stringComplete) {
    processInput();
    inputString = "";
    stringComplete = false;
  }
  
  // Проверка входящих сообщений по I2C
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
  
  // Проверяем, является ли сообщение командой с указанием получателя
  if (inputString.startsWith("@")) {
    int spaceIndex = inputString.indexOf(' ');
    if (spaceIndex != -1) {
      String recipientStr = inputString.substring(1, spaceIndex);
      String message = inputString.substring(spaceIndex + 1);
      
      currentRecipient = recipientStr.toInt();
      
      if (currentRecipient == 2 || currentRecipient == 3) {
        sendMessage(currentRecipient, message);
        Serial.print(">>> Отправлено пользователю ");
        Serial.print(currentRecipient);
        Serial.print(": ");
        Serial.println(message);
      } else {
        Serial.println("Ошибка: Неверный номер получателя. Используйте @2 или @3");
      }
    } else {
      Serial.println("Ошибка: Неверный формат. Используйте: @номер сообщение");
    }
  } else {
    // Сообщение без указания получателя - отправляем всем
    broadcastMessage(inputString);
    Serial.print(">>> Broadcast: ");
    Serial.println(inputString);
  }
  Serial.print("Введите сообщение: ");
}

void sendMessage(int recipient, String message) {
  Wire.beginTransmission(recipient);
  Wire.write(M1_ADDRESS); // Отправитель
  Wire.write(':'); // Разделитель
  for (int i = 0; i < message.length(); i++) {
    Wire.write(message[i]);
  }
  Wire.write('\0'); // Конец сообщения
  Wire.endTransmission();
}

void broadcastMessage(String message) {
  for (int i = 2; i <= 3; i++) {
    sendMessage(i, message);
  }
}

// Обработчик входящих сообщений по I2C
void receiveEvent(int howMany) {
  String message = "";
  int sender = Wire.read();
  
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
