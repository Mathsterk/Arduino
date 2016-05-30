unsigned long ticktime = 0;
<<<<<<< HEAD
unsigned long currentMicros = 0;
unsigned long shift = 0;
=======
unsigned long currentMillis = 0;
>>>>>>> parent of eebea3f... stuff


int fancolor = 0;

void setup() {

  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);


}

void loop() {

// if(!digitalRead(2) && fancolor == 1) {
  if (fancolor == 0) {
    ticktime = millis();
  }
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
<<<<<<< HEAD
  currentMicros = micros();
=======

  currentMillis = millis();
>>>>>>> parent of eebea3f... stuff

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  // if (abs(ticktime-millis()) <= 10000 || ticks) {
  // if (!triggered) {
  //   ticktime = millis();
  // }

  // if (currentMillis - ticktime <= 5) {
  //   pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
  //   triggered = true;
  // } else {
  //   pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  // }
  // if (currentMillis - ticktime >= 9) {
  //       triggered = false;
  // }


  if (currentMillis - ticktime >= 25) {
    fancolor = 0;
  }
  if (currentMillis - ticktime <= 24) {
    fancolor = 1;
  }
  if (currentMillis - ticktime <= 12) {
    fancolor = 2;
  }
  //  if (currentMillis - ticktime <= 33) {
  //     fancolor = 1;
  // }


  switch (fancolor) {
  case 0: showled(0, 0, 0);
    break;
  case 1: showled(255, 0, 0);
    break;
  case 2: showled(0, 255, 0);
    break;
  case 3: showled(0, 0, 255);
    break;
  }

}

void showled(int red, int green, int blue) {
  analogWrite(9, red);
  analogWrite(10, green);
  analogWrite(11, blue);
}