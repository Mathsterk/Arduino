#include <Adafruit_NeoPixel.h>

#define N_PIXELS  119  // Number of pixels in strand
#define MIC_PIN   A5  // Microphone is attached to this analog pin
#define LED_PIN    53  // NeoPixel LED strand is connected to this pin
#define SAMPLES    20  // Length of buffer for dynamic level adjustment
#define TOP       (N_PIXELS + 1) // Allow dot to go slightly off scale
#define PEAK_FALL 15  // Rate of peak falling dot

byte
peak      = 0,      // Used for falling dot
dotCount  = 0,      // Frame counter for delaying dot-falling speed
volCount  = 0;      // Frame counter for storing past volume data
int vol[SAMPLES],       // Collection of prior volume samples
    lvl       = 20,      // Current "dampened" audio level
    minLvlAvg = 0,      // For dynamic adjustment of graph low & high
    maxLvlAvg = 255;

Adafruit_NeoPixel
strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel
strip2 = Adafruit_NeoPixel(35, 34, NEO_RGB + NEO_KHZ800);

int
redBeat = 1,
greenBeat = 1,
blueBeat = 1;

int beat = 0;

byte val;// Data received from the serial port
int b = 0;
int beatCountdown = 35;
int beatUnCount = 0;
int beatCountUp = 35;
int peakHold = 0;
boolean beatUp = false;
boolean beatDown = false;
int peakTime = 5;
int peakTimeout = 0;

boolean analogColor = false;
int analogBeatBlue = 0;
int analogBeatGreen = 0;
int analogBeatRed = 0;

// this constant won't change:
const int  buttonPin = 24;    // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 8;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

const int  buttonPin2 = 22;    // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter2 = 1;   // counter for the number of button presses
int buttonState2 = 0;         // current state of the button
int lastButtonState2 = 0;


unsigned int rgbColour[3] = {255, 0, 0};

int decColour = 0;
int incColour = 1;

int rainbowCounter = 0;
boolean rainbow = false;
int rainbowTick = 0;

int strobe = 0;

boolean eightReset = true;


void setup()
{

    // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
    // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
    // line.  Audio samples are 'cleaner' at 3.3V.
    // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
    //analogReference(DEFAULT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(42, OUTPUT);
    pinMode(44, OUTPUT);
    pinMode(46, OUTPUT);
    pinMode(51, OUTPUT);

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);

    memset(vol, 0, sizeof(vol));
    strip.begin();
    strip2.begin();
    Serial.begin(115200);
    strip.setBrightness(255);
    randomSeed(analogRead(A0) * analogRead(A15));
}

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

    analogWrite(2, redBeat);
    analogWrite(3, greenBeat);
    analogWrite(4, blueBeat);

    if (n != b)
    {
        n   =  b;                  // Raw reading from mic
        n   = abs(n - 5); // Center on zero
        lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)

        // Calculate bar height based on dynamic min/max levels (fixed point):
        height = TOP * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

        if (height < 0)       height = 0;     // Clip output
        else if (height > TOP) height = TOP;
        if (height > peak)
        {
            peak   = height; // Keep 'peak' dot at top
            peakHold = 100;
        }


        if (height <= 10)
        {
            if (beatUp)
            {
                beatUnCount = 1;
                beatUp = false;
            }
        }
        else
        {
            beatUp = true;
        }
        if (height >= 50)
        {
            if (beatDown)
            {
                beatDown = false;
                beatUnCount = 1;
            }
        }
        else
        {
            beatDown = true;
        }


        if (buttonPushCounter == 7)
        {
            analogBeatBlue = analogRead(A3);
            analogBeatGreen = analogRead(A2);
            analogBeatRed = analogRead(A1);
            blueBeat = constrain(map(analogBeatBlue, 0, 1023, 0, 255), 0, 255);
            greenBeat = constrain(map(analogBeatGreen, 0, 1023, 0, 255), 0, 255);
            redBeat = constrain(map(analogBeatRed, 0, 1023, 0, 255), 0, 255);
            analogColor = true;

        }

        if (buttonPushCounter == 8)
        {
            if (eightReset) {
                unsigned int rgbColour[3] = {255, 0, 0};
                eightReset = false;
            }
            analogColor = true;


            if (redBeat == 255 && greenBeat == 0 && blueBeat == 0)
            {
                decColour = 0;
                incColour = 1;
            }
            if (redBeat == 0 && greenBeat == 255 && blueBeat == 0)
            {
                decColour = 1;
                incColour = 2;
            }
            if (redBeat == 0 && greenBeat == 0 && blueBeat == 255)
            {
                decColour = 2;
                incColour = 0;
            }

            rgbColour[decColour] -= 1;
            rgbColour[incColour] += 1;
            redBeat = rgbColour[0];
            greenBeat = rgbColour[1];
            blueBeat = rgbColour[2];
            digitalWrite(51, HIGH);
        }
        else
        {
            digitalWrite(51, LOW);
            unsigned int rgbColour[3] = {255, 0, 0};
            eightReset = true;
        }

        if (buttonPushCounter == 10) {
            analogColor = true;
            beatUnCount = 0;
            strip.setBrightness(255);
            // map(analogRead(A5), 0, 1023, 0, 48)
            if (strobe >= 10) {
                // analogBeatBlue = analogRead(A3);
                // analogBeatGreen = analogRead(A2);
                // analogBeatRed = analogRead(A1);
                analogBeatBlue = 255;
                analogBeatGreen = 255;
                analogBeatRed = 255;
                blueBeat = constrain(map(analogBeatBlue, 0, 1023, 0, 255), 0, 255);
                greenBeat = constrain(map(analogBeatGreen, 0, 1023, 0, 255), 0, 255);
                redBeat = constrain(map(analogBeatRed, 0, 1023, 0, 255), 0, 255);

                strobe = 0;
            } else {
                redBeat = 0;
                greenBeat = 0;
                blueBeat = 0;
            }
            strobe++;

        } else {
            //strip.setBrightness(constrain(map(analogRead(A5), 0, 1023, 0, 255), 0, 255));
            strip.setBrightness(255);
        }

        if (buttonPushCounter > 10 || buttonPushCounter < 7)
        {
            analogColor = false;
        }

        if (buttonPushCounter == 11) {
            strip.setBrightness(map(height, 0, 59, 10, 255));
        }



        if (beatUnCount > 0)
        {
            redBeat = 0;
            greenBeat = 0;
            blueBeat = 0;
            while (redBeat + greenBeat + blueBeat <= 200 && !analogColor)
            {

                redBeat = random(511) * random(511) * random(511) % 255;
                greenBeat = random(511) * random(511) * random(511) % 255;
                blueBeat = random(511) * random(511) * random(511) % 255;
                if (buttonPushCounter == 0 || buttonPushCounter == 11)
                {
                    if (redBeat >= 100 && greenBeat >= 100 && blueBeat >= 100)
                    {
                        redBeat = 0;
                        greenBeat = 0;
                        blueBeat = 0;
                    }
                    // Serial.print(redBeat, DEC);
                    // Serial.print("\t");
                    // Serial.print(greenBeat, DEC);
                    // Serial.print("\t");
                    // Serial.println(blueBeat, DEC);
                    if (abs(redBeat - greenBeat) < 60 && abs(greenBeat - blueBeat) < 60 && abs(blueBeat - redBeat) < 60)
                    {
                        redBeat = 0;
                        greenBeat = 0;
                        blueBeat = 0;
                    }
                }
                else if (buttonPushCounter == 1)
                {
                    redBeat = random(150, 255);
                    greenBeat = constrain(greenBeat - random(230, 255), 0, 255);
                    blueBeat = constrain(blueBeat - random(230, 255), 0, 255);
                }
                else if (buttonPushCounter == 2)
                {
                    greenBeat = random(120, 255);
                    redBeat = random(120, 255);
                    blueBeat = constrain(blueBeat - random(230, 255), 0, 255);
                }
                else if (buttonPushCounter == 3)
                {
                    greenBeat = random(150, 255);
                    blueBeat = constrain(greenBeat - random(230, 255), 0, 255);
                    redBeat = constrain(redBeat - random(230, 255), 0, 255);
                }
                else if (buttonPushCounter == 4)
                {
                    blueBeat = random(120, 255);
                    greenBeat = random(120, 255);
                    redBeat = constrain(redBeat - random(230, 255), 0, 255);
                }
                else if (buttonPushCounter == 5)
                {
                    blueBeat = random(150, 255);
                    greenBeat = constrain(greenBeat - random(230, 255), 0, 255);
                    redBeat = constrain(redBeat - random(230, 255), 0, 255);
                }
                else if (buttonPushCounter == 6)
                {
                    blueBeat = random(120, 255);
                    greenBeat = constrain(greenBeat - random(230, 255), 0, 255);
                    redBeat = random(120, 255);
                }
                else if (buttonPushCounter > 11)
                {
                    buttonPushCounter = 0;
                }
            }

            beatUnCount = 0;
        }



        if (buttonPushCounter == 9)
        {
            analogColor = true;
            rainbow = true;
            if (rainbowCounter < 257)
            {
                if (rainbowTick > 2)
                {
                    rainbowCounter += map(analogRead(A3), 0, 1023, 1, 8);
                    rainbowTick = 0;
                }
                rainbowTick++;
            }
            else
            {
                rainbowCounter = 0;
            }
            analogWrite(2, random(255));
            analogWrite(3, 0);
            analogWrite(4, 0);
        }
        else
        {
            rainbow = false;
            rainbowCounter = 0;
            rainbowTick = 0;
        }



        // Color pixels based on rainbow gradient
        if (!rainbow)
        {
            for (i = 0; i < N_PIXELS; i++)
            {
                if (i >= height) strip.setPixelColor(i, 0, 0, 0);
                else strip.setPixelColor(i, Wheel(map(i, 0, strip.numPixels() - 1, 30, 150)));
            }
        }
        else
        {
            for (i = 0; i < N_PIXELS; i++)
            {
                if (i >= height) strip.setPixelColor(i, 0, 0, 0);
                else strip.setPixelColor(i, Wheel2(((i * 256 / 24) - rainbowCounter) & 255));
                // else strip.setPixelColor(i, Wheel2(((i * 256 / strip.numPixels()) - rainbowCounter) & 255));
            }
        }





        //    // Draw peak dot
        //    if(peak > 0 && peak <= N_PIXELS-1) strip.setPixelColor(peak, 255, 255, 255);
        //
        //    strip.show(); // Update strip
        //
        //    // Every few frames, make the peak pixel drop by 1:
        //
        //    if(++dotCount >= PEAK_FALL) { //fall rate
        //
        //      if(peak > 0) peak--;
        //      dotCount = 0;
        //    }
        if (peak == 0)
        {
            peakTimeout += 1;
        }
        else
        {
            peakTimeout = 0;
        }

        if (peakTimeout > 4096)
        {
            digitalWrite(53, LOW);
            for (i = 0; i < N_PIXELS - 1; i++) {
                strip.setPixelColor(i, 0, 0, 0);
            }
            strip.show();
        }
        else
        {
            digitalWrite(53, HIGH);
        }






        vol[volCount] = n;                      // Save sample for dynamic leveling
        if (++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

        // Get volume range of prior frames
        minLvl = maxLvl = vol[0];
        for (i = 1; i < SAMPLES; i++)
        {
            if (vol[i] < minLvl)      minLvl = vol[i];
            else if (vol[i] > maxLvl) maxLvl = vol[i];
        }
        // minLvl and maxLvl indicate the volume range over prior frames, used
        // for vertically scaling the output graph (so it looks interesting
        // regardless of volume level).  If they're too close together though
        // (e.g. at very low volume levels) the graph becomes super coarse
        // and 'jumpy'...so keep some minimum distance between them (this
        // also lets the graph go to zero when no sound is playing):
        if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
        minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
        maxLvlAvg = (maxLvlAvg * 63 + maxLvl + 100) >> 6; // (fake rolling average)
    }
    // Draw peak dot
    if (peak > 0 && peak <= N_PIXELS - 1)
    {
        if (buttonPushCounter2 == 0)
        {
            strip.setPixelColor(peak, 127, 127, 127);
            strip.setPixelColor(peak + 1, 0, 0, 0);
        }
        else if (buttonPushCounter2 == 1)
        {
            strip.setPixelColor(peak, redBeat, greenBeat, blueBeat);
            strip.setPixelColor(peak + 1, 0, 0, 0);
        }
        else
        {
            buttonPushCounter2 = 0;
        }
    }

    for (i = 0; i < 36; i++) {
        strip2.setPixelColor(i, strip.getPixelColor(round(map(i, 0, 35, 0, 17))));
    }
    strip2.setBrightness(map(height, 0, 60, 60, 255));
    strip.show(); // Update strip
    strip2.show();

    buttonState = digitalRead(buttonPin);

    if (buttonState != lastButtonState)
    {
        if (buttonState == HIGH)
        {
            buttonPushCounter++;
        }
    }
    lastButtonState = buttonState;

    buttonState2 = digitalRead(buttonPin2);

    if (buttonState2 != lastButtonState2)
    {
        if (buttonState2 == HIGH)
        {
            buttonPushCounter2++;
        }
    }
    lastButtonState2 = buttonState2;

    // Every few frames, make the peak pixel drop by 1:
    if (peakHold == 0)
    {
        if (++dotCount >= PEAK_FALL)  //fall rate
        {
            if (peak > 0) peak--;
            dotCount = 0;
        }
    }
    else
    {
        peakHold--;
    }
    if (buttonPushCounter == 0)
    {
        digitalWrite(42, HIGH);
        digitalWrite(44, HIGH);
        digitalWrite(46, HIGH);
    }
    else if (buttonPushCounter == 1)
    {
        digitalWrite(42, HIGH);
        digitalWrite(44, LOW);
        digitalWrite(46, LOW);
    }
    else if (buttonPushCounter == 2)
    {
        digitalWrite(42, HIGH);
        digitalWrite(44, HIGH);
        digitalWrite(46, LOW);
    }
    else if (buttonPushCounter == 3)
    {
        digitalWrite(42, LOW);
        digitalWrite(44, HIGH);
        digitalWrite(46, LOW);
    }
    else if (buttonPushCounter == 4)
    {
        digitalWrite(42, LOW);
        digitalWrite(44, HIGH);
        digitalWrite(46, HIGH);
    }
    else if (buttonPushCounter == 5)
    {
        digitalWrite(42, LOW);
        digitalWrite(44, LOW);
        digitalWrite(46, HIGH);
    }
    else if (buttonPushCounter == 6)
    {
        digitalWrite(42, HIGH);
        digitalWrite(44, LOW);
        digitalWrite(46, HIGH);
    }
    else if (buttonPushCounter == 7)
    {
        digitalWrite(42, LOW);
        digitalWrite(44, LOW);
        digitalWrite(46, LOW);
    }
    else if (buttonPushCounter > 11)
    {
        buttonPushCounter = 0;
    }



}
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel2(byte WheelPos)
{
    if (WheelPos < 85)
    {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

//uint32_t Wheel(byte WheelPos) {
//  if(WheelPos < 255) {
//    return strip.Color(WheelPos * 1.7, 255 - WheelPos * 1.7, 0);
//  }
//}


uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 255)
    {
        return strip.Color(redBeat, greenBeat, blueBeat);
    }
}









