import processing.serial.*;
import java.io.*;

Serial myPort;

boolean pong = false;
boolean firstContact = false;
boolean ok = false;
boolean RCV = false;
boolean RDY = false;
boolean ACK = false;
boolean OK = false;
boolean newSerialData = false;

int textLength[];
int ACKval = 0;

String command = "";
String textLengthStr;
String text;
String val;




void setup() {
    myPort = new Serial(this, "COM3", 115200);
    myPort.bufferUntil('\n');
}

void draw() {
    if (firstContact) {
        if (val.equals("OK")) {
            OK = true;
            OKtimeout = 0;
        } else if (!RCV && !RDY && !ACK) {
            OKtimeout++;
        }

        if (OK && !RCV && !RDY && !ACK) {
            textLengthStr = String.valueOf(text.length());
            myPort.write("RCV " + textLengthStr + "\n");
            println("RCV " + textLengthStr);
            RCV = true;
            RCVtimeout = 0;
        }

        if (val.equals("RDY") && !RDY && RCV && !ACK && OK) {
            myPort.write(text);
            println(text);
            RDY = true;
            RDYtimeout = 0;
        }

        if (val.length() >= 3) command = val.substring(0, 3);

        if (command.equals("ACK") && !ACK && RDY && RCV && OK) {
            //println(text);
            if (newSerialData) {
                newSerialData = false;
                ACKval += int(val.substring(4));
                println("pACKval " + ACKval);
            }
            if (ACKval == text.length()) {
                ok = false;
                ACK = true;
                ACKtimeout = 0;
                ACKval = 0;
                println("P ACK");
            }
        }

        if (ACK) {
            //If the text file has run out of numbers, then read the text file again in 5 seconds.
            delay(5000);
            mySwitch = 1;
            RCV = false;
            RDY = false;
            ACK = false;
        }
    }
}






void serialEvent( Serial myPort) {
    //put the incoming data into a String -
    //the '\n' is our end delimiter indicating the end of a complete packet
    val = myPort.readStringUntil('\n');
    //make sure our data isn't empty before continuing
    if (val != null) {
        //trim whitespace and formatting characters (like carriage return)
        val = trim(val);
        println(val);
        newSerialData = true;
        //look for our 'A' string to start the handshake
        //if it's there, clear the buffer, and send a request for data
        if (firstContact == false) {
            if (val.equals("A")) {
                myPort.clear();
                firstContact = true;
                myPort.write("A\n");
                println("contact");
                delay(100);
            }
        }
    }
}