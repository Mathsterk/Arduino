import processing.serial.*;
import java.io.*;
import processing.core.PApplet;
import processing.core.PImage;

int mySwitch = 0;
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

void setup() {
    //Create a switch that will control the frequency of text file reads.
    //When mySwitch=1, the program is setup to read the text file.
    //This is turned off when mySwitch = 0
    mySwitch = 1;

    //Open the serial port for communication with the Arduino
    //Make sure the COM port is correct
    myPort = new Serial(this, "COM3", 115200);
    myPort.bufferUntil('\n');

    size(640, 720);
    background(0);
    colorMode(HSB, (hueRange - 1));
}

void draw() {
    if (firstContact) {
        if (mySwitch > 0) {
            /*The readData function can be found later in the code.
             This is the call to read a CSV file on the computer hard-drive. */
            readData("C:\\Users\\Mathsterk\\Programmer\\Snip\\snip.txt");
            img = loadImage("C:\\Users\\Mathsterk\\Programmer\\Snip\\Snip_Artwork.jpg" /* Your image here */  );

            /*The following switch prevents continuous reading of the text file, until
             we are ready to read the file again. */
            mySwitch = 0;
            getColors();
            boolean colorFull = false;
            while (!colorFull) {
                int highest = redVal;

                if (greenVal > highest) {
                    highest = greenVal;
                }
                if (blueVal > highest) {
                    highest = blueVal;
                }
                highest = abs(255 - highest);
                redVal += highest;
                greenVal += highest;
                blueVal += highest;

                if (redVal == 255 || greenVal == 255 || blueVal == 255) {
                    colorFull = true;
                }
            }


            if (redVal < 10) redString = "00" + str(redVal);
            if (redVal < 100) redString = "0" + str(redVal);
            if (redVal >= 100) redString = str(redVal);

            if (greenVal < 10) greenString = "00" + str(greenVal);
            if (greenVal < 100) greenString = "0" + str(greenVal);
            if (greenVal >= 100) greenString = str(greenVal);

            if (blueVal < 10) blueString = "00" + str(blueVal);
            if (blueVal < 100) blueString = "0" + str(blueVal);
            if (blueVal >= 100) blueString = str(blueVal);

            subtext = redString + "," + greenString + "," + blueString + " " + subtext;
            // colorString += subtext;
            // subtext = "";
            // subtext = colorString;
            // println(subtext);
        }
        /*Only send new data. This IF statement will allow new data to be sent to
         the arduino. */


        if (val.equals("OK")) {
            OK = true;
            OKtimeout = 0;
        } else if (!RCV && !RDY && !ACK) {
            OKtimeout++;
        }

        if (OK && !RCV && !RDY && !ACK) {
            textLengthStr = String.valueOf(subtext.length());
            myPort.write("RCV " + textLengthStr + "\n");
            println("RCV " + textLengthStr);
            RCV = true;
            RCVtimeout = 0;
        } else if (OK && !RDY && !ACK) {
            RCVtimeout++;
        }

        if (val.equals("RDY") && !RDY && RCV && !ACK && OK) {
            myPort.write(subtext);
            println(subtext);
            RDY = true;
            RDYtimeout = 0;
        } else if (!RCV && OK && !ACK) {
            RDYtimeout++;
        }
        if (val.length() >= 3) command = val.substring(0, 3);

        if (command.equals("ACK") && !ACK && RDY && RCV && OK) {
            //println(subtext);
            if (newSerialData) {
                newSerialData = false;
                ACKval += int(val.substring(4));
                println("pACKval " + ACKval);
            }
            if (ACKval == subtext.length()) {
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
            delay(5000);
            mySwitch = 1;
            RCV = false;
            RDY = false;
            ACK = false;
        }
        //if (RCVtimeout >= 100 || RCVtimeout >= 100 || RCVtimeout>= 100 || RCVtimeout >= 100) {
        //  RCV = false;
        //  RDY = false;
        //  ACK = false;
        //  OK = true;

        //  RCVtimeout = 0;
        //  RDYtimeout = 0;
        //  ACKtimeout = 0;
        //  OKtimeout = 0;

        //  println("TIMEOUT! TIMEOUT!");
        //}
    }
}



/* The following function will read from a CSV or TXT file */
void readData(String myFileName) {

    File file = new File(myFileName);
    BufferedReader br = null;

    try {
        br = new BufferedReader(new FileReader(file));
        String text = null;

        /* keep reading each line until you get to the end of the file */
        while ((text = br.readLine()) != null) {
            /* Spilt each line up into bits and pieces using a comma as a separator */
            subtext = text;
        }
    } catch (FileNotFoundException e) {
        e.printStackTrace();
    } catch (IOException e) {
        e.printStackTrace();
    } finally {
        try {
            if (br != null) {
                br.close();
            }
        } catch (IOException e) {
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

private void extractColorFromImage() {
    img.loadPixels();
    int numberOfPixels = img.pixels.length;
    int[] hues = new int[hueRange];
    float[] saturations = new float[hueRange];
    float[] brightnesses = new float[hueRange];

    for (int i = 0; i < numberOfPixels; i++) {
        int pixel = img.pixels[i];
        int hue = Math.round(hue(pixel));
        float saturation = saturation(pixel);
        float brightness = brightness(pixel);
        hues[hue]++;
        saturations[hue] += saturation;
        brightnesses[hue] += brightness;
    }

    // Find the most common hue.
    int hueCount = hues[0];
    int hue = 0;
    for (int i = 1; i < hues.length; i++) {
        if (hues[i] > hueCount) {
            hueCount = hues[i];
            hue = i;
        }
    }

    // Set the vars for displaying the color.
    this.hue = hue;
    saturation = saturations[hue] / hueCount;
    brightness = brightnesses[hue] / hueCount;
}

public void getColors() {
    extractColorFromImage();
    image(img, 0, 0, 640, 640);
    fill(hue, saturation, brightness);
    rect(-1, 640, 641, 120);
    // Color c = color.HSBtoRGB(map(hue, 0, 360, 0, 1), map(saturation, 0, 100, 0, 1), map(brightness, 0, 100, 0, 1));
    color c = color(hue, saturation, brightness);  // Define color 'c'
    redVal = int(map(red(c), 0, 359, 0, 255));
    greenVal = int(map(green(c), 0, 359, 0, 255));
    blueVal = int(map(blue(c), 0, 359, 0, 255));

    println("red: " + redVal + "\t green: " + greenVal + "\t\tblue: " + blueVal);
    println("hue: " + hue + "\tsaturation: " + int(map(saturation, 0, 359, 0, 100)) + "\t brigthness: " + int(map(brightness, 0, 359, 0, 100)));
    println();
}