#include <Adafruit_NeoPixel.h>

#define N_PIXELS  119  // Number of pixels in strand
#define MIC_PIN   A5  // Microphone is attached to this analog pin
#define LED_PIN    53  // NeoPixel LED strand is connected to this pin

Adafruit_NeoPixel
strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

byte val;// Data received from the serial port
int b = 0;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

const int numSamples = N_PIXELS;

int red[numSamples];      // the readings from the analog input
int green[numSamples];      // the readings from the analog input
int blue[numSamples];      // the readings from the analog input
int sampleIndex = 0;                  // the index of the current reading
int SampleTotal = 0;                  // the running total

int forSample = 0;

int idleCounter = 0;
int idleCounter2 = 0;
int idleCounter3 = 0;



void setup()
{

	// This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
	// Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
	// line.  Audio samples are 'cleaner' at 3.3V.
	// COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
	//analogReference(DEFAULT);

	strip.setBrightness(255);
	strip.begin();
	Serial.begin(115200);


	for (int thisReading = 0; thisReading < numReadings; thisReading++)
		readings[thisReading] = 0;

	for (int i = 0; i < numSamples; i++) {
		red[i] = 0;
		green[i] = 0;
		blue[i] = 0;
	}

}

int test = 0;

void loop()
{
	uint8_t  i;
	uint16_t minLvl, maxLvl;
	int      n, height;


	if (Serial.available())   // If data is available to read,
	{
		val = Serial.read(); // read it and store it in val
		b = val;
	}

	if (n != b)
	{

		n = b;                  // Raw reading from mic

		idleCounter = 0;
		idleCounter2 = 0;
		idleCounter3 = 0;

	} else if (idleCounter <= 32000 && idleCounter3 < 6000) {
		idleCounter++;
	}
	if(idleCounter >= 32000) idleCounter2++;
	if(idleCounter2 >= 32000) idleCounter3++;


	total = total - readings[index];
	// read from the sensor:
	readings[index] = b;
	// add the reading to the total:
	total = total + readings[index];
	// advance to the next position in the array:
	index = index + 1;

	// if we're at the end of the array...
	if (index >= numReadings)
		// ...wrap around to the beginning:
		index = 0;

	// calculate the average:
	average = total / numReadings;

	// send it to the computer as ASCII digits

	// Color pixels based on rainbow gradient
	if (test >= numReadings) {
		for (i = numSamples - 1; i > 0; i--) {
			red[i] = red[i - 1];
			green[i] = green[i - 1];
			blue[i] = blue[i - 1];

		}
		// if (average < 120) {
		// 	red[0] = 0;
		// 	green[0] = average;
		// 	blue[0] = 0;
		// } else if (average < 190) {
		// 	red[0] = average;
		// 	green[0] = average;
		// 	blue[0] = 0;
		// } else if (average < 256) {
		// 	red[0] = average;
		// 	green[0] = 0;
		// 	blue[0] = 0;
		// }

		
		red[0] = WheelR(((256 / strip.numPixels()) - (average + 2)) & 255);
		green[0] = WheelG(((256 / strip.numPixels()) - (average + 2)) & 255);
		blue[0] = 0; //WheelB(((256 / strip.numPixels()) - average) & 255);

		// red[0] = constrain(rgbColour[0] - map(average, 0, 255, 255, 0), 0, 255);
		// green[0] = constrain(rgbColour[1] - map(average, 0, 255, 255, 0), 0, 255);
		// blue[0] = constrain(rgbColour[2] - map(average, 0, 255, 255, 0), 0, 255);


		for (i = 0; i < N_PIXELS; i++)
		{
			// strip.setPixelColor(i, samples[i], samples[i], samples[i]);
			strip.setPixelColor(i, red[i], green[i], blue[i]);
			// strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) - rainbowCounter) & 255, i));
		}
	
		test = -1000;
		strip.show(); // Update strip
	} else {
		test++;
	}

	if (idleCounter3 > 5000) {
		 rainbow(10);
	}



}

uint32_t WheelR(byte WheelPos)
{
	if (WheelPos < 85)
	{
		return WheelPos * 3;
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return 255 - WheelPos * 3;
	}
	else
	{
		WheelPos -= 170;
		return 0;
	}
}
uint32_t WheelG(byte WheelPos)
{
	if (WheelPos < 85)
	{
		return 0;
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return WheelPos * 3;
	}
	else
	{
		WheelPos -= 170;
		return 255 - WheelPos * 3;
	}
}
uint32_t WheelB(byte WheelPos)
{
	if (WheelPos < 85)
	{
		return 255 - WheelPos * 3;
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return  0;
	}
	else
	{
		WheelPos -= 170;
		return 0;
	}
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=255; j>0; j--) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}