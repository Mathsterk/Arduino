/**
 * An FFT object is used to convert an audio signal into its frequency domain representation. This representation
 * lets you see how much of each frequency is contained in an audio signal. Sometimes you might not want to 
 * work with the entire spectrum, so it's possible to have the FFT object calculate average frequency bands by 
 * simply averaging the values of adjacent frequency bands in the full spectrum. There are two different ways 
 * these can be calculated: <b>Linearly</b>, by grouping equal numbers of adjacent frequency bands, or 
 * <b>Logarithmically</b>, by grouping frequency bands by <i>octave</i>, which is more akin to how humans hear sound.
 * <br/>
 * This sketch illustrates the difference between viewing the full spectrum, 
 * linearly spaced averaged bands, and logarithmically spaced averaged bands.
 * <p>
 * From top to bottom:
 * <ul>
 *  <li>The full spectrum.</li>
 *  <li>The spectrum grouped into 30 linearly spaced averages.</li>
 *  <li>The spectrum grouped logarithmically into 10 octaves, each split into 3 bands.</li>
 * </ul>
 *
 * Moving the mouse across the sketch will highlight a band in each spectrum and display what the center 
 * frequency of that band is. The averaged bands are drawn so that they line up with full spectrum bands they 
 * are averages of. In this way, you can clearly see how logarithmic averages differ from linear averages.
 * <p>
 * For more information about Minim and additional features, visit http://code.compartmental.net/minim/
 */

// PROCESSING, YOU FUCKING ASSHAT, START SAVING WHEN YOU'RE SUPPOSED TO....

import processing.serial.*;

import ddf.minim.analysis.*;
import ddf.minim.*;

Minim minim;  
AudioInput jingle;
FFT fftLin;
FFT fftLog;

float height3;
float height23;
float spectrumScale = 4;

PFont font;




float fullVal;

Serial Serial;
Serial myPort;
float val;

float[] peak = new float[30];
float ledScale = 2;

int serialIt = 0;
int mouseIt = 0;
float maxPeak = 0;
int peakZero;


void setup()
{
  size(1024, 960);
  height3 = height/3;
  height23 = 2*height/3;

  minim = new Minim(this);
  jingle = minim.getLineIn(Minim.STEREO);

  // loop the file
  //jingle.loop();

  // create an FFT object that has a time-domain buffer the same size as jingle's sample buffer
  // note that this needs to be a power of two 
  // and that it means the size of the spectrum will be 1024. 
  // see the online tutorial for more info.
  fftLin = new FFT( jingle.bufferSize(), jingle.sampleRate() );

  // calculate the averages by grouping frequency bands linearly. use 30 averages.
  fftLin.linAverages( 20 );

  // create an FFT object for calculating logarithmically spaced averages
  fftLog = new FFT( jingle.bufferSize(), jingle.sampleRate() );

  // calculate averages based on a miminum octave width of 22 Hz
  // split each octave into three bands
  // this should result in 30 averages
  fftLog.logAverages( 22, 2 );

  rectMode(CORNERS);
  //font = loadFont("ArialMT-12.vlw");


  String portName = Serial.list()[0];
  Serial = new Serial(this, portName, 4000000);
  delay(1000);
}

void draw()
{
  background(0);

  //textFont(font);
  textSize( 18 );

  float centerFrequency = 0;

  // perform a forward FFT on the samples in jingle's mix buffer
  // note that if jingle were a MONO file, this would be the same as using jingle.left or jingle.right
  fftLin.forward( jingle.mix );
  fftLog.forward( jingle.mix );

  // draw the full spectrum
  {
    noFill();
    for (int i = 0; i < fftLin.specSize(); i++)
    {
      // if the mouse is over the spectrum value we're about to draw
      // set the stroke color to red
      if ( i * 2== mouseX )
      {
        centerFrequency = fftLin.indexToFreq(i);
        stroke(255, 0, 0);
        fullVal = fftLin.getBand(i)*spectrumScale;
        mouseIt = i;
      } else
      {
        stroke(255);
      }
      line(i * 2, height3, i * 2, height3 - fftLin.getBand(i)*spectrumScale);
    }

    fill(255, 128);
    text("Spectrum Center Frequency: " + centerFrequency, 5, height3 - 75);
    text("Spectrum Value: " + fullVal, 5, height3 - 100);
    text("Iteration: " + mouseIt, 5, height3 - 125);
  }

  // no more outline, we'll be doing filled rectangles from now
  noStroke();

  // draw the linear averages
  {
    // since linear averages group equal numbers of adjacent frequency bands
    // we can simply precalculate how many pixel wide each average's 
    // rectangle should be.
    int w = int( width/fftLin.avgSize() );
    for (int i = 0; i < fftLin.avgSize(); i++)
    {
      // if the mouse is inside the bounds of this average,
      // print the center frequency and fill in the rectangle with red
      if ( mouseX >= i*w && mouseX < i*w + w )
      {
        centerFrequency = fftLin.getAverageCenterFrequency(i);

        fill(255, 128);
        text("Linear Average Center Frequency: " + centerFrequency, 5, height23 - 75);
        text("Spectrum Value: " + fftLin.getBand(i)*spectrumScale, 5, height23 - 100);
        text("Iteration: " + i, 5, height23 - 125);
        fill(255, 0, 0);
      } else
      {
        fill(255);
      }
      // draw a rectangle for each average, multiply the value by spectrumScale so we can see it better
      rect(i*w, height23, i*w + w, height23 - fftLin.getAvg(i)*spectrumScale);
    }
  }

  // draw the logarithmic averages
  {
    // since logarithmically spaced averages are not equally spaced
    // we can't precompute the width for all averages
    for (int i = 0; i < fftLog.avgSize(); i++)
    {
      centerFrequency    = fftLog.getAverageCenterFrequency(i);
      // how wide is this average in Hz?
      float averageWidth = fftLog.getAverageBandWidth(i);   

      // we calculate the lowest and highest frequencies
      // contained in this average using the center frequency
      // and bandwidth of this average.
      float lowFreq  = centerFrequency - averageWidth/2;
      float highFreq = centerFrequency + averageWidth/2;

      // freqToIndex converts a frequency in Hz to a spectrum band index
      // that can be passed to getBand. in this case, we simply use the 
      // index as coordinates for the rectangle we draw to represent
      // the average.
      int xl = (int)fftLog.freqToIndex(lowFreq);
      int xr = (int)fftLog.freqToIndex(highFreq);

      // if the mouse is inside of this average's rectangle
      // print the center frequency and set the fill color to red
      if ( mouseX >= xl * 2 && mouseX < xr * 2 )
      {
        fill(255, 128);
        text("Logarithmic Average Center Frequency: " + centerFrequency, 5, height - 75);
        text("Spectrum Value: " + fftLog.getBand(i)*spectrumScale, 5, height - 100);
        text("Iteration: " + i, 5, height - 125);
        fill(255, 0, 0);
      } else
      {
        fill(255);
      }
      // draw a rectangle for each average, multiply the value by spectrumScale so we can see it better
      rect( xl * 2, height, xr * 2, height - fftLog.getAvg(i)*spectrumScale );

      peak[i] = constrain(fftLog.getBand(i)*ledScale, 0, 255);
    }
  }
  
  maxPeak = 0;

  for (int peakIt = 0; peakIt < peak.length - 1; peakIt++) {
    if (peak[peakIt] > maxPeak) maxPeak = peak[peakIt];
    if (peak[peakIt] < peak[peakIt + 1]) {
      peak[peakIt] = peak[peakIt + 1];
    }
  }

  for(int counter = 0; counter < maxPeakArray.length; counter) {
  
  }

  peak[0] = peak[0] * 1.00;
  peak[1] = peak[1] * 1.00;
  peak[2] = peak[2] * 1.00;

  peak[3] = peak[3] * 0.50;
  peak[4] = peak[4] * 0.50;
  peak[5] = peak[5] * 0.50;

  peak[6] = peak[6] * 1.00;
  peak[7] = peak[7] * 1.00;
  peak[8] = peak[8] * 1.00;

  if (maxPeak < 1) {
    peakZero++;
  } else { 
    peakZero = 0;
  }
  

  if (serialIt++ > 0 && peakZero < 200) {
    serialIt = 0;
    Serial.write(int(peak[0]) + "," + int(peak[3]) + "," + int(peak[6]) + "," + int(peak[1]) + "," + int(peak[4]) + "," + int(peak[7]) + "," + int(peak[2]) + "," + int(peak[5]) + "," + int(peak[8]) + "\n");
    println(peak[0] + "\t\t" + peak[1] + "\t\t" + peak[2] + "\t\t" + peak[3] + "\t\t" + peak[4] + "\t\t" + peak[5] + "\t\t" + peak[6] + "\t\t" + peak[7] + "\t\t" + peak[8]);
  }

  if ( Serial.available() > 0) {  // If data is available,
    char val = char(Serial.read());         // read it and store it in val 
    print(val); // echo everying but time request
  }
}

void stop()
{
  // always close Minim audio classes when you finish with them
  jingle.close();

  // always stop Minim before exiting
  //Serial.write("0,0,0,0,0,0,0,0,0\n");
  delay(10);
  minim.stop();
  super.stop();
}