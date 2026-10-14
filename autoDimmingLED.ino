const int LDR_PIN = A0;
const int LED_PIN = 9;
const int BUTTON_PIN = 7;

// 0 = ON
// 1 = OFF
// 2 = AUTO
int mode = 0;

// Untuk moving average LDR
const int NUM_SAMPLES = 10;
int readings[NUM_SAMPLES];
int index = 0;
long total = 0;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  // Isi buffer awal LDR
  int initialValue = analogRead(LDR_PIN);

  for (int i = 0; i < NUM_SAMPLES; i++) {
    readings[i] = initialValue;
    total += initialValue;
  }

  // Kondisi awal = ON
  analogWrite(LED_PIN, 255);
}

// Mulai looping, *NOTHING LAST FOREVER :)
void loop() {

  // Cek button
  if (digitalRead(BUTTON_PIN) == LOW) {

    // Tunggu button dilepas
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(10);
    }

    // Pindah mode
    mode++;

    if (mode > 2) {
      mode = 0;
    }

    // Jalankan indikator sesuai mode
    if (mode == 0) {
      blinkMode(2);
      analogWrite(LED_PIN, 255);
    }

    else if (mode == 1) {
      blinkMode(3);
      analogWrite(LED_PIN, 0);
    }

    else if (mode == 2) {
      blinkMode(4);
    }
  }

  // MODE AUTO
  if (mode == 2) {

    int rawValue = analogRead(LDR_PIN);

    // Moving average
    total -= readings[index];
    readings[index] = rawValue;
    total += readings[index];

    index++;

    if (index >= NUM_SAMPLES) {
      index = 0;
    }

    int filteredValue = total / NUM_SAMPLES;

    // Penyesuaian dengan hasil kalibrasi pencahayaan di ruangan partikular
    int brightness = map(filteredValue, 330, 700, 255, 0);

    brightness = constrain(brightness, 0, 255);

    analogWrite(LED_PIN, brightness);

    Serial.print("LDR: ");
    Serial.print(filteredValue);

    Serial.print(" | Brightness: ");
    Serial.println(brightness);
  }

  delay(20);
}


// FUNGSI blinking
void blinkMode(int jumlah) {
  digitalWrite (LED_PIN, 0);
  delay(150);
  for (int i = 0; i < jumlah; i++) {

    analogWrite(LED_PIN, 255);
    delay(150);

    analogWrite(LED_PIN, 0);
    delay(150);
  }
  digitalWrite (LED_PIN, 0);
  delay(150);
}