import processing.serial.*; //<>//
import java.io.*;
import processing.core.PApplet;
import processing.core.PImage;

int mySwitch = 1;
int counter = 0;
String subtext;
Serial myPort;
boolean pong = false;
String val;
boolean firstContact = false;
int textlength[];
boolean ok = false;
String textLengthStr;

PImage img;
float hue;
static final int hueRange = 360;
float saturation;
float brightness;

int redVal = 0;
int greenVal = 0;
int blueVal = 0;

boolean RCV = false;
boolean RDY = false;
boolean ACK = false;
boolean OK = false;

int RCVtimeout = 0;
int RDYtimeout = 0;
int ACKtimeout = 0;
int OKtimeout = 0;

int textLength[];

boolean newSerialData = false;

int ACKval = 0;


String command = "";

String redString;
String greenString;
String blueString;

String prevSubtext;
boolean newTextData = true;
String serialText = "0,0,0,0,0,0,0,0,0";
int oldCount = 0;
int oldCountTwo = 0;
boolean hilighted = false;
String hilight;
boolean hilightBlank = false;



import processing.serial.*;
import processing.dxf.*;
import ddf.minim.analysis.*;
import ddf.minim.*;

FFT fftLin;
FFT fftLog;

Minim minim;
AudioInput microphone;




float camzoom;
int tempJ2 = 0;
int tempJ3 = 0;
int boom = 255;
float peak;
float maxPeak;
int loop = 0;
int tempJloop = 0;
boolean boop = true;
int volPeak = 127;
int volPeakCount = 0;

float x = 127 - 25;
float y = 127 - 25;
float w = 50;
float h = 50;

void setup() {
  //Create a switch that will control the frequency of text file reads.
  //When mySwitch=1, the program is setup to read the text file.
  //This is turned off when mySwitch = 0
  mySwitch = 1;

  //Open the serial port for communication with the Arduino
  //Make sure the COM port is correct
  myPort = new Serial(this, "COM6", 115200);
  myPort.bufferUntil('\n');



  minim = new Minim(this);
  microphone = minim.getLineIn(Minim.STEREO); //repeat the song


  fftLog = new FFT(microphone.bufferSize(), microphone.sampleRate());
  fftLog.logAverages(1, 2); //adjust numbers to adjust spacing;


  size(250, 250);
  background(30);
  stroke(0);
  noFill();

  delay(1000); // so we don't send shit to the bootloader
}

void draw() {
  if (firstContact) {

    /*The readData function can be found later in the code.
     This is the call to read a CSV file on the computer hard-drive. */



    /*The following switch prevents continuous reading of the text file, until
     we are ready to read the file again. */
    mySwitch = 0;





    for (int i=0; i < 30; i++) {
      peak = (fftLog.getAvg(i));
      maxPeak = peak + maxPeak;
    }

    int tempJ = round(maxPeak);
    if (boop) {
      if (tempJ == 0) {
        if (tempJloop > 300) {
          tempJloop = 0;
          boop = false;
          for (int i=5; i >= 5; i--) {
            //myPort.write(i);
          }
        } else {
          tempJloop++;
        }
      }
    }


    if (volPeak <= 0) volPeak = 10;

    if (tempJ != tempJ2) {
      tempJ3 = constrain(round(map(tempJ, 0, volPeak, 0, 255)), 0, 255);
      //println(tempJ + "\t" + tempJ3 + "\t" + volPeak);
      if (mySwitch > 0) {
        //myPort.write(tempJ3);
        newSerialData = true;
        serialText = str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3) + "," + str(tempJ3);
      }
      tempJ2 = tempJ;
      boop = true;
    }
    maxPeak = 0;

    if (tempJ >= volPeak) {
      volPeak = tempJ;
    } else {  
      if (volPeakCount >= 5 && volPeak > 127) {
        volPeakCount = 0;
        volPeak--;
        if (abs(volPeak - tempJ) >= 1000) volPeak -= 1;
        if (abs(volPeak - tempJ) >= 1500) volPeak -= 2;
        if (abs(volPeak - tempJ) >= 2000) volPeak -= 5;
      } else {
        volPeakCount++;
      }
    }




    fftLog.forward(microphone.mix);

    background(30);
    rect(x, y, w, h);
    fill(255);
    if (mousePressed) {
      if (mouseX>x && mouseX <x+w && mouseY>y && mouseY <y+h) {
        volPeak = 16;
        println("VOLRESET !!!           @@@@");
        fill(0);
        //do stuff
      }
    } 


    oldCount = 0;
    oldCountTwo = 0;




    //if (!newTextData) {

    //  serialText = " ";
    //  delay(1000);
    //  if (++oldCount > 10) {
    //    mySwitch = 1;
    //    oldCount = 0;
    //    println("oldtextdata");
    //    if (++oldCountTwo > 100) {
    //      newTextData = true;
    //      oldCountTwo = 0;
    //    }
    //  }
    //}

    /*Only send new data. This IF statement will allow new data to be sent to
     the arduino. */


    if (val.equals("OK")) {
      OK = true;
      OKtimeout = 0;
    } else if (!RCV && !RDY && !ACK) {
      OKtimeout++;
    }
    if (newTextData) {
      if (OK && !RCV && !RDY && !ACK) {
        textLengthStr = String.valueOf(serialText.length());
        myPort.write("RCV " + textLengthStr + "\n");
        println("RCV " + textLengthStr);
        RCV = true;
        RCVtimeout = 0;
      } else if (OK && !RDY && !ACK) {
        RCVtimeout++;
      }

      if (val.equals("RDY") && !RDY && RCV && !ACK && OK) {
        myPort.write(serialText);
        println(serialText);
        RDY = true;
        RDYtimeout = 0;
      } else if (!RCV && OK && !ACK) {
        RDYtimeout++;
      }
    }
    // if (val.length() >= 3) command = val.substring(0, 3);

    // if (command.equals("ACK") /*&& !ACK && RDY && RCV && OK*/) {
    //     //println(subtext);
    //     if (newSerialData) {
    //         newSerialData = false;
    //         ACKval += int(val.substring(4));
    //         println("pACKval " + ACKval);
    //     }
    //     if (ACKval == serialText.length()) {
    //         ok = false;
    //         ACK = true;
    //         ACKtimeout = 0;
    //         ACKval = 0;
    //         println("P ACK");
    //     }

    // } else if (!RCV && !RDY && OK) {
    //     ACKtimeout++;
    // }


    if (command.equals("ACK") /*&& !ACK && RDY && RCV && OK*/) {
      //println(subtext);

      if (ACKval >= serialText.length()) {
        ok = false;
        ACK = true;
        ACKtimeout = 0;
        ACKval = 0;
        println("P ACK");
      }
    } else if (!RCV && !RDY && OK) {
      ACKtimeout++;
    }

    if (ACK) {
      //If the text file has run out of numbers, then read the text file again in 5 seconds.
      println("PACK ACK");
      delay(500);
      mySwitch = 1;
      RCV = false;
      RDY = false;
      ACK = false;
      newTextData = false;
    }

    // if (RCVtimeout >= 1000 || RCVtimeout >= 1000 || RCVtimeout >= 1000 || RCVtimeout >= 1000) {
    //     RCV = false;
    //     RDY = false;
    //     ACK = false;
    //     OK = true;

    //     RCVtimeout = 0;
    //     RDYtimeout = 0;
    //     ACKtimeout = 0;
    //     OKtimeout = 0;

    //     println("TIMEOUT! TIMEOUT!");
    // }
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
    if (val.length() >= 3) command = val.substring(0, 3);
    if (newSerialData && val.length() > 4) {
      newSerialData = false;
      ACKval += int(val.substring(4));
      println("pACKval " + ACKval);
    }
  }
}

void stop()
{
  // always close Minim audio classes when you finish with them
  microphone.close();
  // always stop Minim before exiting
  minim.stop();
  super.stop();
}