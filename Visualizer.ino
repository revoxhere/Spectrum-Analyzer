#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <fix_fft.h>
#include <Servo.h>
#define LCHAN 0
#define greenLed 9
const int channels = 1;
const int xres = 16;
const int yres = 8;
const int gain = 2;
//int decayrate = 2;
int decaytest = 1;
char im[64], data[64];
char Rim[64], Rdata[64];
char data_avgs[32];
float peaks[32];
int valLED;
int i = 0, val, Rval;
int x = 0, y = 0, z = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ZNAKI
byte v1[8] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111
};
byte v2[8] = {
  B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111
};
byte v3[8] = {
  B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111
};
byte v4[8] = {
  B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111
};
byte v5[8] = {
  B00000, B00000, B00000, B11111, B11111, B11111, B11111, B11111
};
byte v6[8] = {
  B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111
};
byte v7[8] = {
  B00000, B11111, B11111, B11111, B11111, B11111, B11111, B11111
};
byte v8[8] = {
  B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111
};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, v1);
  lcd.createChar(2, v2);
  lcd.createChar(3, v3);
  lcd.createChar(4, v4);
  lcd.createChar(5, v5);
  lcd.createChar(6, v6);
  lcd.createChar(7, v7);
  lcd.createChar(8, v8);
  startup();
}

void startup() {
  lcd.clear();
  delay(50);
  lcd.setCursor(0, 0);
  lcd.print("Booting up");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up.");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up..");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up...");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up.");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up..");
  delay(200);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Booting up...");
  delay(200);

  lcd.clear();
  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("SpectrumAnalyzer");
  lcd.setCursor(0, 1);
  lcd.print("  by revox, 2019");
  delay(4500);
}

void loop() {
  for (i = 0; i < 64; i++) {
    val = ((analogRead(LCHAN) / 4 ) - 128);
    data[i] = val;
    im[i] = 0;
  };
  fix_fft(data, im, 6, 0); // Send the data through fft
  for (i = 0; i < 32 ; i++) {
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
  }
  for (i = 0; i < 32; i++) {
    data_avgs[i] = (data[i]);// + data[i*2+1]);// + data[i*3 + 2]);// + data[i*4 + 3]);  // add 3 samples to be averaged, use 4 when yres < 16
    data_avgs[i] = constrain(data_avgs[i], 0, 9 - gain); //data samples * range (0-9) = 9
    data_avgs[i] = map(data_avgs[i], 0, 9 - gain, 0, yres);      // remap averaged values
  }
  decay(1);
  mono();
  leds();
} // end loop

void leds() {
  valLED = analogRead(LCHAN) * 0.7;
  analogWrite(greenLed, valLED);
}

void decay(int decayrate) {
  //// reduce the values of the last peaks by 1
  if (decaytest == decayrate) {
    for (x = 0; x < 32; x++) {
      peaks[x] = peaks[x] - 1;  // subtract 1 from each column peaks
      decaytest = 0;
    }
  }
  decaytest++;
}

void mono() {
  for (x = 0; x < xres; x++) { // repeat for each column of the display horizontal resolution
    y = data_avgs[x];  // get current column value
    z = peaks[x];
    if (y > z) {
      peaks[x] = y;
    }
    y = peaks[x];
    if (y <= 8) {
      lcd.setCursor(x, 0); // clear first row
      lcd.print(" ");
      lcd.setCursor(x, 1); // draw second row
      if (y == 0) {
        lcd.print(" "); // save a glyph
      }
      else {
        lcd.write(y);
      }
    }
    else {
      lcd.setCursor(x, 0); // draw first row
      if (y == 9) {
        lcd.write(" ");
      }
      else {
        lcd.write(y - 8); // same chars 1-8 as 9-16
      }
      lcd.setCursor(x, 1);
      lcd.write(8);
    }
  } // end display
}  // end xres
