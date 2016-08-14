const int colPinFrom = 5;
const int colPinTo = 7;

const int rowPinFrom = 22;
const int rowPinTo = 24;


int test = 0;
unsigned long testdelay = 1000;

char command[1024];
char commandBuffer[128];
int commandBufferSize = 0;

boolean pong = false;


int counter = 0;
int lastIndex = 0;

char inData[80];
byte index = 0;

int ledMatrix[3][3] = {
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0}
};

void setup() {
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	// refreshMatrix();

	for (int pins = colPinFrom; pins <= colPinTo; pins++) {
		pinMode(pins, OUTPUT);
	}
	for (int pins = rowPinFrom; pins <= rowPinTo; pins++) {
		pinMode(pins, OUTPUT);
	}


	Serial.begin(4000000);	//turn on the serial connection
	Serial.println("");
	delay(1000);
	Serial.println("OK");
	digitalWrite(13, LOW);
}

void loop() {

	unsigned int rgbColour[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	// DO SOMETHING USEFUL
	// Serial.println(command);

	int loopCount = 0;

	// char *p = command;
	// char *str;

	// while ((str = strtok_r(p, ",", &p)) != "\n") // delimiter is the comma
	// {
	// 	Serial.println(str);
	// 	loopCount++;
	// 	rgbColour[loopCount] = atoi(str);
	// 	p = NULL;
	// 	delay(1000);
	// }

	// while (Serial.available() > 0)
	// {
	// 	char aChar = Serial.read();
	// 	if (aChar == '\n')
	// 	{
	// 		// End of record detected. Time to parse

	// 		char *token;
	// 		char *rest = inData;

	// 		// Serial.println("I");

	// 		while ((token = strtok_r(rest, ",", &rest)))
	// 		{
	// 			// printf("token:%s\n", token);
	// 			// Serial.println(token);
	// 			rgbColour[loopCount] = atoi(token);
	// 			loopCount++;
	// 		}

	// 		setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2], rgbColour[3], rgbColour[4], rgbColour[5], rgbColour[6], rgbColour[7], rgbColour[8]);


	// 		index = 0;
	// 		inData[index] = NULL;
	// 	}
	// 	else
	// 	{
	// 		inData[index] = aChar;
	// 		index++;
	// 		inData[index] = '\0'; // Keep the string NULL terminated
	// 	}
	// }


	// char *token;
	// char *rest = command;

	// while ((token = strtok_r(rest, ",", &rest)))
	// {
	// 	// printf("token:%s\n", token);
	// 	// Serial.println(token);
	// 	rgbColour[loopCount] = atoi(token);
	// 	loopCount++;
	// }

	// setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2], rgbColour[3], rgbColour[4], rgbColour[5], rgbColour[6], rgbColour[7], rgbColour[8]);



// 101,102,103,104,105,106,107,108,109



	// unsigned int rgbColour[9];

	// // Start off with red.
	// rgbColour[0] = 255;
	// rgbColour[1] = 0;
	// rgbColour[2] = 0;

	// rgbColour[3] = 0;
	// rgbColour[4] = 0;
	// rgbColour[5] = 0;

	// rgbColour[6] = 0;
	// rgbColour[7] = 0;
	// rgbColour[8] = 0;

	// // Choose the colours to increment and decrement.
	// for (int decColour = 0; decColour < 10; decColour += 1) {
	// 	int incColour = decColour == 10 ? 0 : decColour + 1;

	// 	// cross-fade the two colours.
	// 	for (int i = 0; i < 255; i += 1) {
	// 		rgbColour[decColour] -= 1;
	// 		rgbColour[incColour] += 1;

	// 		setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2], rgbColour[3], rgbColour[4], rgbColour[5], rgbColour[6], rgbColour[7], rgbColour[8]);
	// 		delayMicroseconds(100);
	// 	}
	// }
	// delayMicroseconds(testdelay);
	if (testdelay <= 0) {
		testdelay = 500;
		if (++test > 9) {
			test = 1;
		}
	} else {
		testdelay -= 1;
		// int test = random(1, 9);

	}
	switch (test) {
	case 1:
		setColourRgb(1, 0, 0, 1, 0, 0, 1, 0, 0);
		break;
	case 2:
		setColourRgb(0, 1, 0, 0, 1, 0, 0, 1, 0);
		break;
	case 3:
		setColourRgb(0, 0, 1, 0, 0, 1, 0, 0, 1);
		break;
	case 4:
		setColourRgb(1, 0, 0, 0, 1, 0, 0, 0, 1);
		break;
	case 5:
		setColourRgb(0, 1, 0, 0, 0, 1, 1, 0, 0);
		break;
	case 6:
		setColourRgb(0, 0, 1, 1, 0, 0, 0, 1, 0);
		break;
	case 7:
		setColourRgb(0, 0, 0, 0, 0, 0, 1, 0, 0);
		break;
	case 8:
		setColourRgb(0, 0, 0, 1, 0, 0, 0, 0, 0);
		break;
	case 9:
		setColourRgb(1, 0, 0, 0, 0, 0, 0, 0, 0);
		break;
	default:
		setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 0);
		break;
	}

	// for (int i = 3; i < 12; i++) {
	// 	for (int v = 0; v < 256; v++) {
	// 		analogWrite(i, v);
	// 		delay(5);
	// 	}
	// 	analogWrite(i, 0);
	// }
	refreshMatrix();
}

void setColourRgb(unsigned int red1, unsigned int green1, unsigned int blue1, unsigned int red2, unsigned int green2, unsigned int blue2, unsigned int red3, unsigned int green3, unsigned int blue3) {
	// ledMatrix[1][1] = red1;
	// ledMatrix[1][2] = green1;
	// ledMatrix[1][3] = blue1;

	// ledMatrix[2][1] = red2;
	// ledMatrix[2][2] = green2;
	// ledMatrix[2][3] = blue2;

	// ledMatrix[3][1] = red3;
	// ledMatrix[3][2] = green3;
	// ledMatrix[3][3] = blue3;

	ledMatrix[22][5] = red1;
	ledMatrix[22][6] = green1;
	ledMatrix[22][7] = blue1;

	ledMatrix[23][5] = red2;
	ledMatrix[23][6] = green2;
	ledMatrix[23][7] = blue2;

	ledMatrix[24][5] = red3;
	ledMatrix[24][6] = green3;
	ledMatrix[24][7] = blue3;
}



void refreshMatrix() {
	for (int colour = colPinFrom; colour <= colPinTo; colour++) {
		for (int row = rowPinFrom; row <= rowPinTo; row++) {
			for (int rowClear = rowPinFrom; rowClear <= rowPinTo; rowClear++) {
				digitalWrite(rowClear, LOW);
			}
			for (int colClear = colPinFrom; colClear <= colPinTo; colClear++) {
				digitalWrite(colClear, LOW);
			}

			digitalWrite(row, ledMatrix[row][colour]);
			digitalWrite(colour, HIGH);
			// delay(10);
			digitalWrite(13, !digitalRead(13));
		}
	}

	// 	for (int colour = 5; colour <= 7; colour++) {
	// 	for (int row = 22; row <= 24; row++) {

	// 		if (colour > 2) {
	// 			digitalWrite(colour - 1, LOW);
	// 		}
	// 		digitalWrite(colour, HIGH);
	// 		if (colour < 7) {
	// 			digitalWrite(colour + 1, LOW);
	// 		}

	// 		if (row > 22) {
	// 			digitalWrite(row - 1, LOW);
	// 		}
	// 		digitalWrite(row, HIGH);
	// 		if (row < 24) {
	// 			digitalWrite(row + 1, LOW);
	// 		}

	// 		delay(1000);
	// 		digitalWrite(13, !digitalRead(13));
	// 	}
	// }
}