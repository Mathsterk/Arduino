
unsigned long ticktime = 0;
unsigned long currentMicros = 0;
unsigned long shift = 0;


int fancolor = 0;

int fanCount = 0;


void setup() {

  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), fantrip, FALLING);

}

void loop() {

// if(!digitalRead(2)) {
//   // if (fancolor == 0) {
//    fanCount++;
//   }

  // if (fanCount >= 2 && fancolor == 0) {
  if (fanCount >= 2) {
    // if (fancolor == 0) {
    ticktime = micros();
    fanCount = 0;
  }
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  currentMicros = micros();

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  // if (abs(ticktime-millis()) <= 10000 || ticks) {
  // if (!triggered) {
  //   ticktime = millis();
  // }

  // if (currentMicros - ticktime <= 5) {
  //   pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Moderately bright green color.
  //   triggered = true;
  // } else {
  //   pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  // }
  // if (currentMicros - ticktime >= 9) {
  //       triggered = false;
  // }


  if (currentMicros - ticktime >= 2100) {
    fancolor = 0;
    // ticktime = micros();
  }

  if (currentMicros - ticktime <= 6000 && currentMicros - ticktime >= 2000) {
    fancolor = 1;
  }
    if (currentMicros - ticktime <= 2000 && currentMicros - ticktime >= 1500) {
    fancolor = 4;
  }
    if (currentMicros - ticktime <= 1500 && currentMicros - ticktime >= 500) {
    fancolor = 3;
  }
    if (currentMicros - ticktime <= 500 && currentMicros - ticktime >= 0) {
    fancolor = 4;
  }


  switch (fancolor) {
  case 0: showled(0,  0,  0);
    break;
  case 1: showled(1, 0, 0);
    break;
  case 2: showled(0, 1, 0);
    break;
  case 3: showled(0, 0, 1);
    break;
  case 4: showled(1, 1, 1);
    break;
  }

}

void showled(int red, int green, int blue) {
  digitalWrite(9, 1 - red);
  digitalWrite(10, 1 - green);
  digitalWrite(11, 1 - blue);
}

void fantrip() {
  fanCount++;
}