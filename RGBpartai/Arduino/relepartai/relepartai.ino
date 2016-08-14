const int redPin1 = 3;
const int greenPin1 = 4;
const int bluePin1 = 5;

const int redPin2 = 6;
const int greenPin2 = 7;
const int bluePin2 = 8;

const int redPin3 = 9;
const int greenPin3 = 10;
const int bluePin3 = 11;


const int ledCount = 9;

int test = 0;
double testdelay = 200000;

char command[1024];
char commandBuffer[128];
int commandBufferSize = 0;

boolean pong = false;


int counter = 0;
int lastIndex = 0;

char inData[80];
byte index = 0;

void setup() {
	for (int pins = 2; pins >= 13; pins++) {
		pinMode(pins, OUTPUT);
		delay(10);
		digitalWrite(pins, HIGH);
	}
			pinMode(22, OUTPUT);
		pinMode(24, OUTPUT);
		pinMode(26, OUTPUT);

	digitalWrite(22, HIGH);
	digitalWrite(24, HIGH);
	digitalWrite(26, HIGH);

	// Start off with the LED off.
	// setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 0);

	Serial.begin(4000000);	//turn on the serial connection
	Serial.println("");
	for (int boot = 0; boot <= 50; boot++) {
		// setColourRgb(boot, boot, boot, boot, boot, boot, boot, boot, boot);
		delay(10);
	}
	for (int boot = 50; boot >= 0; boot--) {
		// setColourRgb(boot, boot, boot, boot, boot, boot, boot, boot, boot);
		delay(10);
	}
	Serial.println("OK");
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

	while (Serial.available() > 0)
	{
		char aChar = Serial.read();
		if (aChar == '\n')
		{
			// End of record detected. Time to parse

			char *token;
			char *rest = inData;

			// Serial.println("I");

			while ((token = strtok_r(rest, ",", &rest)))
			{
				// printf("token:%s\n", token);
				// Serial.println(token);
				rgbColour[loopCount] = atoi(token);
				loopCount++;
			}

			setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2], rgbColour[3], rgbColour[4], rgbColour[5], rgbColour[6], rgbColour[7], rgbColour[8]);


			index = 0;
			inData[index] = NULL;
		}
		else
		{
			inData[index] = aChar;
			index++;
			inData[index] = '\0'; // Keep the string NULL terminated
		}
	}


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
// 	delayMicroseconds(testdelay);
// if (testdelay <= 0) testdelay = 200000; else testdelay -= 1000;
// 	// int test = random(1, 9);
// 	if (++test > 9) {
// 		test = 1;
// 	}

// 	switch (test) {
// 	case 1:
// 		setColourRgb(255, 0, 0, 0, 0, 0, 0, 0, 0);
// 		break;
// 	case 2:
// 		setColourRgb(0, 255, 0, 0, 0, 0, 0, 0, 0);
// 		break;
// 	case 3:
// 		setColourRgb(0, 0, 255, 0, 0, 0, 0, 0, 0);
// 		break;
// 	case 4:
// 		setColourRgb(0, 0, 0, 255, 0, 0, 0, 0, 0);
// 		break;
// 	case 5:
// 		setColourRgb(0, 0, 0, 0, 255, 0, 0, 0, 0);
// 		break;
// 	case 6:
// 		setColourRgb(0, 0, 0, 0, 0, 255, 0, 0, 0);
// 		break;
// 	case 7:
// 		setColourRgb(0, 0, 0, 0, 0, 0, 255, 0, 0);
// 		break;
// 	case 8:
// 		setColourRgb(0, 0, 0, 0, 0, 0, 0, 255, 0);
// 		break;
// 	case 9:
// 		setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 255);
// 		break;
// 	default:
// 		setColourRgb(0, 0, 0, 0, 0, 0, 0, 0, 0);
// 		break;
// 	}

	// for (int i = 3; i < 12; i++) {
	// 	for (int v = 0; v < 256; v++) {
	// 		analogWrite(i, v);
	// 		delay(5);
	// 	}
	// 	analogWrite(i, 0);
	// }

}

void setColourRgb(unsigned int red1, unsigned int green1, unsigned int blue1, unsigned int red2, unsigned int green2, unsigned int blue2, unsigned int red3, unsigned int green3, unsigned int blue3) {
	if (red1 > 200)
	{
		digitalWrite(22, LOW);
	} else {
		digitalWrite(22, HIGH);
	}

	if (green1 > 127)
	{
		digitalWrite(24, LOW);
	} else
	{
		digitalWrite(24, HIGH);
	}

	if (blue1 > 100 || red2 > 100 || green2 > 100 || blue2 > 100 || red3 > 100 || green3 > 100 || blue3 > 100)
	{
		digitalWrite(26, LOW);
	} else {
		digitalWrite(26, HIGH);
	}


}
