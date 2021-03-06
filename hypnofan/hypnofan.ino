// setter noen globale variabler
// unsigned sier at vi ikke trenger et negativt tall
// long er en datatype (32 bit integer(heltall), så 0 til 4,294,967,295)

unsigned long ticktime = 0;
unsigned long currentMicros = 0;
unsigned long shift = 0;

// int er integer, en integer er et heltall, (på en arduino er en integer -32,768 til 32,767  med andre ord, 16 bit totalt, 1 bit brukes til å si om tallet er positivt eller negativt, og 15 bit brukes til å beskrive tallet)
int fanColor = 0;
int fanCount = 0;

// denne kjøres bare 1 gang
void setup() {

// setter opp hva pinsene brukes til, om de skal drive noe eller bare leses ifra
// pin 2 skal være INPUT
  pinMode(2, INPUT);
// pin 13 skal være et output, den kan være enten lav eller høy
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // setter opp en interrupt (når noe skjer, uansett hva arduinoen holder på med, kjør funksjonen fantrip)
  // digitalPinToInterrupt() oversetter pin 2 til hvilken interrupt
  // fantrip er funksjonen
  // FALLING er hva den skal trigge på, nå er det når pin 2 trekkes til jord
  attachInterrupt(digitalPinToInterrupt(2), fantrip, FALLING);
// ut av løkka
}

// denne loopes til arduinoen resettes, tryner eller slåes av
void loop() {

// if(!digitalRead(2)) {
//   // if (fanColor == 0) {
//    fanCount++;
//   }

  // if (fanCount >= 2 && fanColor == 0) {

    // if-løkke, om fanCount er større eller lik 2, kjører den
  if (fanCount >= 2) {
    // if (fanColor == 0) {
      // setter variabelen ticktime til hva funksjonen micros returnerer
    ticktime = micros();
    // setter variabelen fanCount til 0
    fanCount = 0;
    // ut av if-løkka
  }

  // setter currentMicros til hva funksjonen micros returnerer
  currentMicros = micros();

 // om currentMicros minus ticktime er større eller lik 2100 så kjøres den
  if (currentMicros - ticktime >= 2100) {
    fanColor = 0;
    // ticktime = micros();
  }

 // om currentMicros minus ticktime er mindre eller lik 6000 ELLER currentMicros minus ticktime er større eller lik 2000
  if (currentMicros - ticktime <= 6000 && currentMicros - ticktime >= 2000) {
    fanColor = 1;
  }
    if (currentMicros - ticktime <= 2000 && currentMicros - ticktime >= 1500) {
    fanColor = 4;
  }
    if (currentMicros - ticktime <= 1500 && currentMicros - ticktime >= 500) {
    fanColor = 3;
  }
    if (currentMicros - ticktime <= 500 && currentMicros - ticktime >= 0) {
    fanColor = 4;
  }

// denne er litt interessant, det er som mange if-løkker, om fanColor er 1, 2 ,3 etc
  switch (fanColor) {
    // om fanColor er 0, så kjøres funksjonen showled
  case 0: showled(0,  0,  0);
  // ut av case 0
    break;
  case 1: showled(1, 0, 0);
    break;
  case 2: showled(0, 1, 0);
    break;
  case 3: showled(0, 0, 1);
    break;
  case 4: showled(1, 1, 1);
    break;
  }

}

// dette er en funksjon, den returnerer ingenting (void), den heter showled, og tar imot 3 parametere, red, green og blue, de må være en integer (helt tall)
void showled(int red, int green, int blue) {
  // skriv til pin 9, 1 (true) minus variabelen rød
  digitalWrite(9, 1 - red);

  digitalWrite(10, 1 - green);
  digitalWrite(11, 1 - blue);
  // ut av funksjonen
}

// enda en funksjon, returnerer ingenting, og heter fantrip
void fantrip() {
  // øk verdien til fanCount med 1
  fanCount++;
}