#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// ==============================
// PIN DEFINISI
// ==============================
#define LED_HIJAU   1
#define LED_KUNING  3
#define LED_MERAH   2
#define BUZZER      4
#define LED_PINTU   15   // pengganti servo (indikator pintu)

#define CLK 6
#define DT  7
#define SW  8

// ==============================
// OBJEK
// ==============================
Encoder enc(CLK, DT);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

long lastNumber = 0;
long currentNumber = 0;

bool loketBuka = false;
long lastPos = 0;

// ==============================
// SETUP
// ==============================
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

// ==============================
// LOOP UTAMA
// ==============================
void loop() {
  handleEncoder();
  handleButtonSW();
  updateLED();
  updateDisplay();
}

// ==============================
// Encoder Putar → Toggle Buka/Tutup
// ==============================
void handleEncoder() {
  long pos = enc.read() / 4;

  if (pos != lastPos) {
    lastPos = pos;

    loketBuka = !loketBuka;   // toggle loket
    tone(BUZZER, 2000, 120);

    Serial.print("Loket: ");
    Serial.println(loketBuka ? "BUKA" : "TUTUP");
  }
}

// ==============================
// Button SW → Panggil Nomor
// ==============================
void handleButtonSW() {
  static unsigned long lastPress = 0;

  if (digitalRead(SW) == LOW && millis() - lastPress > 300) {
    lastPress = millis();

    if (loketBuka) {
      lastNumber++;
      currentNumber = lastNumber;
      panggilNomor();
    } else {
      tone(BUZZER, 600, 200);
    }
  }
}

// ==============================
// Efek Pemanggilan Nomor
// ==============================
void panggilNomor() {

  for (int i = 0; i < 4; i++) {

    digitalWrite(LED_KUNING, HIGH);     // LED kuning nyala saat panggil nomor
    digitalWrite(LED_PINTU, HIGH);
    tone(BUZZER, 1500, 80);
    delay(120);

    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_PINTU, LOW);
    delay(120);
  }
}

// ==============================
// Update LED Status Loket
// ==============================
void updateLED() {

  if (loketBuka) {
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_MERAH, LOW);
    digitalWrite(LED_KUNING, LOW);  // default mati, hanya nyala saat panggil
  } 
  else {
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_MERAH, HIGH);
  }
}

// ==============================
// OLED DISPLAY
// ==============================
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

// ==============================
// Fungsi untuk pesan awal
// ==============================
void displayInfo(String a, String b) {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.println(a);
  oled.println(b);
  oled.display();
}
