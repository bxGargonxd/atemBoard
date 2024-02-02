//  DDRn set in out
//  PORTn write
//  PINn read
//      PINREGISTERS              (Board, BCT in)
//
//  A   -   Button/LED in out register
//      22  D0;
//      23  D1;
//      24  D2;
//      25  D3;
//      26  D4;
//      27  D5;
//      28  D6;
//      29  D7;   
//
//  B   -   
//      53  
//      52  
//      51  
//      50  
//      10  
//      11  
//      12  
//      13  
//
//  C   -  Control Pins 
//      37  DIR245
//      36  OE245
//      35  OE4094
//      34  LOCK
//      33  POOTY
//      32  
//      31  
//      30  SIGN
//
//  D   -   
//      21  
//      20  
//      19  
//      18  
//       x
//       x
//       x
//      38
//
//  E
//       0
//       1
//       x
//       5
//       2
//       3
//       x
//       x
//
//  F(A)-   kept free for fading units
//       0  
//       1  
//       2  
//       3  
//       4  
//       5  
//       6  
//       7  
//
//  G   -   
//      41  
//      40  
//      39  
//       x
//       x
//       4  
//       x
//       x
//
//  H
//      17
//      16
//       x
//       6
//       7
//       8
//       9
//       x
//
//  J
//      15  
//      14
//       x
//       x
//       x
//       x
//       x
//       x
//
//  K(A)-   kept free for fading units
//       8
//       9
//      10
//      11
//      12
//      13
//      14
//      15
//
//  L   -   Signal Address Register
//      49  A0; //sel 2000
//      48  A1;
//      47  A2;
//      46  A3;
//      45  A4;
//      44  A5;
//      43  A6;
//      42  A7;

//from platine 2 code
#define strobeAddress 0 //0
#define buttonRefreshAddress 3 //3
#define matrixAddAddress 7 //7

#define matrixRegSize 255 //117

#define defaultLEDValue 0
#define defaultMatrixValue ' '
#define matrixAddressOffset 0 //128
#define regSize 24

int clockAddresses[] = {4, 1, 2};
int clockAddress = clockAddresses[0]; //2P4 4P1 8P2 6P1

int matrixWriteAddress = 6; //26P5 48P6

int regSizes[] = {24, 24, 24};

int buttonRegSize = regSizes[0];
int ledRegSize = 72;

int debugdelay = 0;

long starttime = 0;

bool partymode = false;

uint8_t buttonReg[regSize];
uint8_t ledReg[72];
uint8_t matrixReg[matrixRegSize];
//used for efficient int to HEX with leading zero
char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void setup(){
  Serial.begin(2000000);
  Serial1.end();
  Serial2.end();
  Serial3.end();

  DDRL = DDRC = 255; //set logic IO as Output
  PORTC = B00000100; //set default logic
  PORTL = 0;         //set logic address to 0

  for(int i = 0; i < ledRegSize; ++i){
    ledReg[i] = defaultLEDValue;
  }
  for(int i = 0; i < matrixRegSize; ++i){
    matrixReg[i] = defaultMatrixValue;
  }
  
  for(int i = 0; i < 3;++i){
    clockAddress = clockAddresses[i];
    setLEDs();
  }
  matrixUpdateLoop();
  matrixWriteAddress = 5;
  matrixUpdateLoop();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
}

//Set DataBusRegisters as Input
void readMode() {
  DDRA =  0;           //input
  PORTA = 255;        //enable pullup resistor
}
//Set DataBusRegisters as Output
void writeMode() {
  DDRA = 255;        //output
  PORTA = 0;
}

//enables the pass through of the 245ic
void enable245() {
  PORTC |= B00000010;
  PORTC |= B00000001;
  PORTC &= B11111101;
}

//disable the pass through of the 245ic
void disable245() {
  PORTC |= B00000010;
  PORTC &= B11111110;
  PORTC &= B11111100;
}

void party(){
  delay(100);
  for(int j = 0; j < ledRegSize; ++j){
    ledReg[j] = random(255);
  }
  setLEDs();
  writeStringToMatrix(String(random(99999999999)));
  matrixWriteAddress = 5;
  matrixUpdateLoop();
  matrixWriteAddress = 6;
  matrixUpdateLoop();
}

void executeCommand(String cmd){
  int first = (cmd[0]-48);
  cmd = cmd.substring(1);
  if(first < 5){
    //led command
    for(int i = 0; i < cmd.length()/2; ++i){
      ledReg[i] = htd(cmd.charAt(2*i));
      ledReg[i] = ledReg[i]<<4;
      ledReg[i] += htd(cmd.charAt(2*i+1));
    }
    setLEDs();
  }else if(first < 7){
    for(int i = 0; i < cmd.length(); ++i){
      matrixReg[i] = cmd.charAt(i);
    }
    matrixWriteAddress = first;
    matrixUpdateLoop();
  }else{
    partymode = !partymode;
  }
}

void serialFlush(){
  while (Serial.available()){
    char c = Serial.read();
  }
}

void checkForSerialInput(){
  if(Serial.available()){
    String input = "";
    char cinput = Serial.read();
    while(cinput != '\n'){
       input += cinput;
       cinput = Serial.read();
    }
    Serial.println(input);
    serialFlush();
    executeCommand(input);
  }
}

void loop(){
  while(partymode){
    party();
    checkForSerialInput();
  }
  starttime = millis();
  checkForSerialInput();
  Serial.print("B");
  writeStringToMatrix(String(starttime));
  for(int i = 0; i < 3; ++i){
    clockAddress = clockAddresses[i];
    buttonRegSize = regSizes[i];
    buttonUpdateLoop();
  }
  setLEDs();
  Serial.println();
  Serial.print("P");
  Serial.print(analogRead(A0));
  Serial.print(";");
  Serial.println(analogRead(A1));
  //pootyUpdateLoop(); //TODO
  Serial.print("Update loop in ups:");
  Serial.println(1000/(millis() - starttime));
}

void resetClock(){
  //PORTC &= B01111111;
  //PORTC |= B10000000;
  PORTL = 7;
}


void signPulse(){
  PORTC &= B01111111;
  delay(debugdelay);
  PORTC |= B10000000;
  delay(debugdelay);
}

void buttonUpdateLoop(){
  resetClock();
  readMode();
  disable245();
  //refresh buttonBuffers
  PORTL = buttonRefreshAddress;
  signPulse();
  resetClock();
  PORTL = clockAddress;
  for (int i = 0; i < buttonRegSize; ++i) {
    buttonReg[i] = PINA;
    signPulse();
  }

  sendButtons();
}

void setLEDs(){
  resetClock();
  writeMode();
  enable245();
  int led = 0;
  for(int addr: clockAddresses){
    PORTL = addr;
    for(int i = 0; i < regSize; ++i){
      PORTA = ledReg[led];
      signPulse();
      led++;
    }
  }
  

  resetClock();
  PORTL = strobeAddress;
  signPulse();
  PORTC &= B01111111;
  resetClock();
  
  
}

void ledUpdateLoop(){
  setLEDs();
}

void matrixAddressSave(){
  resetClock();
  PORTL = matrixAddAddress;
  signPulse();
}

void writeStringToMatrix(String s){
  for(int i = 0; i < matrixRegSize;++i){
    matrixReg[i] = s.charAt(i%s.length());
  }
}

//sends a clocksignal to the registers
void matrixWritePulse() {
  resetClock();
  PORTL = matrixWriteAddress;
  signPulse();
}

void matrixUpdateLoop(){
  writeMode();
  enable245();
  for(int x = 0; x < matrixRegSize;x = x + 4){
    for(int y = 0; y < 4; ++y){
    int i = x + y + matrixAddressOffset;
    PORTA = (i);
    matrixAddressSave();
    PORTA = matrixReg[x+3-y];
    matrixWritePulse();
    }
  }
}

void pootyUpdateLoop(){
  
}

void leverUpdateLoop(){
  
}

void sendButtons(){
  Serial.flush();
  for (int i = 0; i < buttonRegSize; ++i) {
    Serial.print(intToHex(buttonReg[i]));
  }
}

String intToHex(uint8_t i) {
  String buf = String(hex[(i & B00001111)]);
  return hex[(i >> 4)] + buf;
}

//convert single HEX Char to int
//uses the raw ascii index to calculate the converted int
uint8_t htd(char c) {
  if (c < 'A') return (uint8_t) (c - '0');
  else return (10 + ((uint8_t)(c - 'A')));
}
