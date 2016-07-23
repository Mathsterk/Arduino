//3D Spectrogram with Microphone Input
//Modified by kylejanzen 2011 – http://kylejanzen.wordpress.com
//Based on script wwritten by John Locke 2011 -http://gracefulspoon.com

//Output .DXF file at any time by pressing “r” on the keyboard
import processing.serial.*;
import processing.dxf.*;
import ddf.minim.analysis.*;
import ddf.minim.*;

FFT fftLin;
FFT fftLog;

Minim minim;
AudioInput microphone;


Serial myPort;
float val;

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

float[] rgbpeak = new float[30];
int[] rgbpeak2 = new int[30];

void setup()
{
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 4000000);

  minim = new Minim(this);
  microphone = minim.getLineIn(Minim.STEREO); //repeat the song


  fftLog = new FFT(microphone.bufferSize(), microphone.sampleRate());
  fftLog.logAverages(22, 3); //adjust numbers to adjust spacing;


  size(250, 250);
  background(30);
  stroke(0);
  noFill();

  delay(1000); // so we don't send shit to the bootloader
}


void draw()
{
  for (int i=0; i < 30; i++) {
    peak = fftLog.getAvg(i);
    //peak = (fftLog.getBand(i));

    //maxPeak = peak + maxPeak;
    //if (i / 1 == 1) {
      if (peak > maxPeak) maxPeak = peak;
      rgbpeak[i] = peak;

      //rgbpeak[i] = fftLog.getAvg(i); //* 5 + i * 3;
    //}
  }


  //println(maxPeak);
  println(fftLog.avgSize() + "\t" + maxPeak + "\t" + rgbpeak[0] + "\t" + rgbpeak[1] + "\t" + rgbpeak[2] + "\t" + rgbpeak[3] + "\t" + rgbpeak[4] + "\t" + rgbpeak[5] + "\t" + rgbpeak[6] + "\t" + rgbpeak[7] + "\t" + rgbpeak[8]);
  if ( myPort.available() > 0) {  // If data is available,
    char val = char(myPort.read());         // read it and store it in val 
    print(val); // echo everying but time request
  }
  int tempJ = round(maxPeak);
  if (boop) {
    if (tempJ == 0) {
      if (tempJloop > 300) {
        tempJloop = 0;
        boop = false;
        //for (int i=5; i >= 5; i--) {
        //  //myPort.write(i);
        //}
      } else {
        tempJloop++;
      }
    }
  }


  if (volPeak <= 0) volPeak = 10;

  if (tempJ != tempJ2) {
    tempJ3 = constrain(round(map(tempJ, 0, volPeak, 0, 255)), 0, 255);

    for (int i = 0; i < 30; i++) {
      rgbpeak2[i] = constrain(round(map(rgbpeak[i], 0, volPeak, 0, 255)), 0, 255);
    }
    //println(tempJ + "\t" + tempJ3 + "\t" + volPeak);
    //println("O");
    myPort.write(rgbpeak2[0] + "," + rgbpeak2[3] + "," + rgbpeak2[6] + "," + rgbpeak2[1] + "," + rgbpeak2[4] + "," + rgbpeak2[7] + "," + rgbpeak2[2] + "," + rgbpeak2[5] + "," + rgbpeak2[8] + "\n");
    //println(tempJ3 + "\t" + rgbpeak2[0] + "," + rgbpeak2[1] + "," + rgbpeak2[2] + "," + rgbpeak2[3] + "," + rgbpeak2[4] + "," + rgbpeak2[5] + "," + rgbpeak2[6] + "," + rgbpeak2[7] + "," + rgbpeak2[8]);
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
}


void stop()
{
  // always close Minim audio classes when you finish with them
  microphone.close();
  // always stop Minim before exiting
  myPort.write("0,0,0,0,0,0,0,0,0\n");
  minim.stop();
  super.stop();
}