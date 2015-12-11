// scrolls data read from serial to matrix
// the following libraries are required for the sketch to work
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define PIN 52
// String readString;	// creates the string scrolling text will be held in


// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                            NEO_RGB            + NEO_KHZ800);

//sets up colors for scrolling text - in this sketch only the first one is used
const uint16_t colors[] =
{
	matrix.Color(255, 0, 0), matrix.Color(255, 255, 0), matrix.Color(0, 255, 0), matrix.Color(0, 255, 255), matrix.Color(0, 0, 255), matrix.Color(255, 0, 255)
};

int counter = 0;
int color = 0;

char command[1024];
char commandBuffer[128];
int commandBufferSize = 0;
boolean pong = false;

int redVal = 0;
int greenVal = 0;
int blueVal = 0;

String redString;
String greenString;
String blueString;

void setup()
{
	Serial.begin(115200);	//turn on the serial connection
	matrix.begin();  	//initalize the matrix
	matrix.fillScreen(0);  //clear the screen
	matrix.setTextWrap(false);
	matrix.setBrightness(255);
	matrix.setTextColor(colors[0]);
	// matrix.fillScreen(0);  //clear the screen
	// matrix.setCursor(16, 0);  //set the begining cursor position
	// matrix.setTextColor(matrix.Color(255, 255, 255));
	// chartoscreen("Hello");
	establishContact();
	Serial.println("");


	delay(1000);
	Serial.println("OK");
}

int x    = 16;  //holds the width of the matrix for counting colums
int pass = 0;  //used to calculate the number of characters read from the serial input

void loop ()
{
	matrix.fillScreen(0);  //clear the screen
	matrix.setCursor(x, 0);  //set the begining cursor position
	while (!pong) {
		readCommand();
		// "command" now contains the full command
		// Serial.println(command);
	}
	// serialtochar();
	// matrix.setBrightness(map(analogRead(A0), 0, 1023, 1, 255));

	if (pong) {
		String commandStr = String(command);

		redString = commandStr.substring(0, 3);
		greenString = commandStr.substring(4, 7);
		blueString = commandStr.substring(8, 11);

		redVal = redString.toInt();
		greenVal = greenString.toInt();
		blueVal = blueString.toInt();

		// Serial.print(redVal);
		// Serial.print(" ");
		// Serial.print(greenVal);
		// Serial.print(" ");
		// Serial.println(blueVal);
		matrix.setTextColor(matrix.Color(redVal, greenVal, blueVal));
		if (commandStr.substring(12).equals(" ") == false) {
			chartoscreen(commandStr.substring(12));
		}
	}
	// if (counter++ > 5000) {
	// 	// Serial.println("OK");
	// 	counter = 0;

	// 	matrix.setTextColor(matrix.Color(random(255), random(255), random(255)));

	// }


}  //end loop


void readCommandBuffer(int bytesToRead) {
	int i = 0;
	char c = 0;
	while (i < 128 && (i < bytesToRead || bytesToRead <= 0)) {
		while (!Serial.available())
			;
		c = Serial.read();
		if (c == '\r' || c == '\n') {
			break;
		}
		commandBuffer[i] = c;
		i++;
	}
	commandBufferSize = i;
}

void readCommand() {
	command[0] = '\0';
	readCommandBuffer(0);
	if (strncmp(commandBuffer, "RCV", 3) == 0) {
		commandBuffer[commandBufferSize] = '\0';
		int expectedSize = atoi(commandBuffer + 4);
		if (expectedSize <= 0 || expectedSize > 1024) {
			return;
		}
		Serial.println("RDY");
		int bytesRead = 0;
		while (bytesRead < expectedSize) {
			readCommandBuffer(expectedSize - bytesRead);
			memcpy(command + bytesRead, commandBuffer, commandBufferSize);
			bytesRead += commandBufferSize;
			Serial.print("ACK ");
			Serial.println(commandBufferSize);
		}
		command[bytesRead] = '\0';
		pong = true;
		// Serial.print("com ");
		// Serial.println(command);

	} else {
		memcpy(command, commandBuffer, commandBufferSize);
		command[commandBufferSize] = '\0';
	}
}

// void serialtochar()
// {
// 	while (Serial.available())  //if serial data is available, run the loop
// 	{
// 		delay(1);
// 		if (Serial.available() > 0)  //if there is serial data continue
// 		{
// 			char c = Serial.read();  //read the next byte of data
// 			readString += c;  //append the data to the char variable readString
// 		}
// 	}

// }

void chartoscreen(String readString)
{
	if (readString.length() > 0)  //if the number of bytes in readString are greater than 0
	{
		matrix.println(readString);  //print the data in readString
		pass = -abs(readString.length());  //set pass to the negative value of the number of characters
		// if (--x < pass * 6) //reserve 6 spaces for each character - continue looping until x = pass * 6
		x -= 1;
		if (x < pass * 6)
		{
			x = matrix.width();  //set x to the number of colums in the matrix
			readString = "";  //set the data to blank
		}

		// Serial.print("mat ");
		// Serial.println(readString);
		matrix.show();  //show the data stored

		delay(constrain(map(readString.length(), 30, 100, 100, 40), 40, 200));  //wait a bit - sets scrolling speed

	}
	if (readString.length() == 0) {
		Serial.println("OK");
		pong = false;
		if (++color >= 6) color = 0;
		matrix.setTextColor(colors[color]);
	}
}

// void chartoscreen(String readString)
// {
// 	if (readString.length() > 0)  //if the number of bytes in readString are greater than 0
// 	{
// 		matrix.println(readString);  //print the data in readString
// 		pass = -abs(readString.length());  //set pass to the negative value of the number of characters

// 		if (--x < pass * 6) //reserve 6 spaces for each character - continue looping until x = pass * 6
// 		{
// 			x = matrix.width();  //set x to the number of colums in the matrix
// 			readString = "";  //set the data to blank
// 		}


// 		matrix.show();  //show the data stored
// 		delay(70);  //wait a bit - sets scrolling speed

// 	}
// }

void establishContact() {
	while (Serial.available() <= 0) {
		Serial.println("A");   // send a capital A
		delay(500);
	}
}