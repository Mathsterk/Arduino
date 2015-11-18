char command[1024];
char commandBuffer[128];
int commandBufferSize = 0;
boolean pong = false;

void setup()
{
	Serial.begin(115200);	//turn on the serial connection
	establishContact();
	Serial.println("");
	delay(1000);
	Serial.println("OK");
}

void loop ()
{
	while (!pong) {
		readCommand();
		// "command" now contains the full command
		// Serial.println(command);
	}

	if (pong) {
		// DO SOMETHING USEFUL
		Serial.println(command);
	}


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

void establishContact() {
	while (Serial.available() <= 0) {
		Serial.println("A");   // send a capital A
		delay(500);
	}
}