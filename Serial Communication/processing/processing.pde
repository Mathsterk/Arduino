import processing.serial.*;
import java.io.*;

String serialText;
Serial myPort;
String val;
boolean firstContact = false;
int textlength[];
boolean ok = false;
String textLengthStr;

boolean RCV = false;
boolean RDY = false;
int RDYfailCount = 0;
boolean ACK = false;

void setup() {
  myPort = new Serial(this, "COM3", 115200);
  myPort.bufferUntil('\n');
}

void draw() {

  if (val.equals("OK") && !RCV) {
    textLengthStr = String.valueOf(serialText.length());
    myPort.write("RCV " + textLengthStr + "\n");
    print("processing ");
    println("RCV " + textLengthStr);
    RCV = true;
    RDYfailCount = 0;
    ;
  }
  if (val.equals("RDY") && !RDY && !RCV) {
    myPort.write(serialText);
    println(serialText);
  }
  if (val.equals("ACK " + serialText.length()) && !ACK && !RDY && !RCV) {
    //println(serialText);
    ok = false;
    ACK = true;
  }
  if (ACK) {
    //If the text file has run out of numbers, then read the text file again in 5 seconds.
    delay(1000);
    ////// DO SOMETHING USEFUL
  }
  if (!RDY) {
    RDYfailCount++;
    delay(1000);
  }
  if (RDYfailCount >= 10)
    RDY = false;
  RCV = false;
  delay(1000);
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