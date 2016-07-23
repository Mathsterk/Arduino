const int redPin1 = 3;
const int redPin2 = 4;
const int redPin3 = 5;

const int greenPin1 = 6;
const int greenPin2 = 7;
const int greenPin3 = 8;

const int bluePin1 = 9;
const int bluePin2 = 10;
const int bluePin3 = 11;

const int ledCount = 9;

// int test = 0;

char command[1024];
char commandBuffer[128];
int commandBufferSize = 0;

boolean pong = false;


int counter = 0;
int lastIndex = 0;

void setup() {
	for (int pin = 2; pin >= 13; pin++) {
		pinMode(pin, OUTPUT);
	}

	// Start off with the LED off.
	setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 0);

	Serial.begin(115200);	//turn on the serial connection
	establishContact();
	Serial.println("");
	delay(1000);
	Serial.println("OK");
}

void loop() {

	unsigned int rgbColour[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	while (!pong) {
		readCommand();
		// "command" now contains the full command
		// Serial.println(command);
	}

	if (pong) {
		// DO SOMETHING USEFUL
		Serial.println(command);

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

		char *token;
		char *rest = command;

		while ((token = strtok_r(rest, ",", &rest)))
		{
			// printf("token:%s\n", token);
			// Serial.println(token);
			rgbColour[loopCount] = atoi(token);
			loopCount++;
		}

		setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2], rgbColour[3], rgbColour[4], rgbColour[5], rgbColour[6], rgbColour[7], rgbColour[8]);
		pong = false;
	}



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
	// delay(85);

	// int test = random(1, 9);
	// if (++test > 9) {
	// 	test = 0;
	// }

	// switch (test) {
	// case 1:
	// 	setColourRgb(255, 0, 0, 0, 0, 0, 0, 0, 0);
	// 	break;
	// case 2:
	// 	setColourRgb(0, 255, 0, 0, 0, 0, 0, 0, 0);
	// 	break;
	// case 3:
	// 	setColourRgb(0, 0, 255, 0, 0, 0, 0, 0, 0);
	// 	break;
	// case 4:
	// 	setColourRgb(0, 0, 0, 255, 0, 0, 0, 0, 0);
	// 	break;
	// case 5:
	// 	setColourRgb(0, 0, 0, 0, 255, 0, 0, 0, 0);
	// 	break;
	// case 6:
	// 	setColourRgb(0, 0, 0, 0, 0, 255, 0, 0, 0);
	// 	break;
	// case 7:
	// 	setColourRgb(0, 0, 0, 0, 0, 0, 255, 0, 0);
	// 	break;
	// case 8:
	// 	setColourRgb(0, 0, 0, 0, 0, 0, 0, 255, 0);
	// 	break;
	// case 9:
	// 	setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 255);
	// 	break;
	// default:
	// 	setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 0);
	// 	break;
	// }

	// for (int i = 3; i < 12; i++) {
	// 	for (int v = 0; v < 256; v++) {
	// 		analogWrite(i, v);
	// 		delay(5);
	// 	}
	// 	analogWrite(i, 0);
	// }

}

void setColourRgb(unsigned int red1, unsigned int green1, unsigned int blue1, unsigned int red2, unsigned int green2, unsigned int blue2, unsigned int red3, unsigned int green3, unsigned int blue3) {
	analogWrite(redPin1, red1);
	analogWrite(greenPin1, green1);
	analogWrite(bluePin1, blue1);

	analogWrite(redPin2, red2);
	analogWrite(greenPin2, green2);
	analogWrite(bluePin2, blue2);

	analogWrite(redPin3, red3);
	analogWrite(greenPin3, green3);
	analogWrite(bluePin3, blue3);
}




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

void establishContact() {
	while (Serial.available() <= 0) {
		Serial.println("A");   // send a capital A
		delay(500);
	}
}