import processing.serial.*;
import java.io.*;

int mySwitch=0;
int counter=0;
String subtext;
Serial myPort;
boolean pong = false;
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
  //Create a switch that will control the frequency of text file reads.
  //When mySwitch=1, the program is setup to read the text file.
  //This is turned off when mySwitch = 0
  mySwitch=1;

  //Open the serial port for communication with the Arduino
  //Make sure the COM port is correct
  myPort = new Serial(this, "COM3", 115200);
  myPort.bufferUntil('\n');
}

void draw() {
  if (firstContact) {
    if (mySwitch>0) {
      /*The readData function can be found later in the code.
       This is the call to read a CSV file on the computer hard-drive. */
      readData("C:\\Users\\Mathsterk\\Programmer\\Snip\\snip.txt");

      /*The following switch prevents continuous reading of the text file, until
       we are ready to read the file again. */
      mySwitch=0;
    }
    /*Only send new data. This IF statement will allow new data to be sent to
     the arduino. */


    if (val.equals("OK") && !RCV) {
      textLengthStr = String.valueOf(subtext.length());
      myPort.write("RCV " + textLengthStr + "\n");
      print("processing ");
      println("RCV " + textLengthStr);
      RCV = true;
      RDYfailCount = 0;;
    }
    if (val.equals("RDY") && !RDY && !RCV) {
      myPort.write(subtext);
      println(subtext);
    }
    if (val.equals("ACK " + subtext.length()) && !ACK && !RDY && !RCV) {
      //println(subtext);
      ok = false;
      ACK = true;
    }
    if(ACK) {
      //If the text file has run out of numbers, then read the text file again in 5 seconds.
      delay(5000);
      mySwitch=1;
    }
    if(!RDY) {
      RDYfailCount++;
      delay(1000);
  }
  if(RDYfailCount >= 10)
  RDY = false;
  RCV = false;
  delay(1000);
}
}



/* The following function will read from a CSV or TXT file */
void readData(String myFileName) {

  File file=new File(myFileName);
  BufferedReader br=null;

  try {
    br=new BufferedReader(new FileReader(file));
    String text=null;

    /* keep reading each line until you get to the end of the file */
    while ((text=br.readLine())!=null) {
      /* Spilt each line up into bits and pieces using a comma as a separator */
      subtext = text;
    }
  }
  catch(FileNotFoundException e) {
    e.printStackTrace();
  }
  catch(IOException e) {
    e.printStackTrace();
  }
  finally {
    try {
      if (br != null) {
        br.close();
      }
    } 
    catch (IOException e) {
      e.printStackTrace();
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