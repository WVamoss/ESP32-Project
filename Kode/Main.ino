#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

#define LED_HIJAU   1
#define LED_KUNING  3
#define LED_MERAH   2
#define BUZZER      4
#define LED_PINTU   15   // pengganti servo

// Encoder
#define CLK 6
#define DT  7
#define SW  8

Encoder enc(CLK, DT);

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

long lastNumber = 0;
long currentNumber = 0;

bool loketBuka = false;

void setup() {
  Serial.begin(115200);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_PINTU, OUTPUT);

  pinMode(SW, INPUT_PULLUP);

  // OLED
  Wire.begin(38, 39);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  displayInfo("Sistem Antrian", "Loket TUTUP");
}

void loop() {
  handleEncoder();
  handleButtonSW();
  updateLED();
  updateDisplay();
}

// ----------------------
// Handle Encoder Putar
// ----------------------
long lastPos = 0;

void handleEncoder() {
  long pos = enc.read() / 4;

  if (pos != lastPos) {
    lastPos = pos;
    loketBuka = !loketBuka;  // toggle status
    tone(BUZZER, 2000, 100);
  }
}

// ----------------------
// Tombol SW → Next
// ----------------------
void handleButtonSW() {
  static unsigned long lastPress = 0;

  if (digitalRead(SW) == LOW && millis() - lastPress > 300) {
    lastPress = millis();

    if (loketBuka) {
      // Ambil nomor baru, simulasi
      lastNumber++;
      currentNumber = lastNumber;

      panggilNomor();
    } else {
      tone(BUZZER, 500, 200);
    }
  }
}

// ----------------------
// Efek pemanggilan nomor
// ----------------------
void panggilNomor() {
  // LED pintu berkedip cepat
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINTU, HIGH);
    tone(BUZZER, 1500, 80);
    delay(120);

    digitalWrite(LED_PINTU, LOW);
    delay(120);
  }
}

// ----------------------
// Status LED loket
// ----------------------
void updateLED() {
  if (loketBuka) {
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_MERAH, LOW);

    if (currentNumber < lastNumber)
      digitalWrite(LED_KUNING, HIGH);
    else
      digitalWrite(LED_KUNING, LOW);

  } else {
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_MERAH, HIGH);
  }
}

// ----------------------
// OLED Display
// ----------------------
void updateDisplay() {
  oled.clearDisplay();
  oled.setCursor(0, 0);

  oled.setTextSize(1);
  oled.print("Loket: ");
  oled.println(loketBuka ? "BUKA" : "TUTUP");

  oled.setTextSize(2);
  oled.print("Now: ");
  oled.println(currentNumber);

  oled.setTextSize(1);
  oled.print("Last: ");
  oled.println(lastNumber);

  oled.display();
}

void displayInfo(String a, String b) {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.println(a);
  oled.println(b);
  oled.display();
}

