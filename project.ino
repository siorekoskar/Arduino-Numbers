#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15
#define G 16
#define DOT 13

int d1=2,d2=3,d3=4,d4=5;
const byte numChars = 32;
char receivedChars[numChars]; 
boolean newData = false;
boolean programStarted = false;

void writeNumber(int whichNumber);
void setupBars();
void setupDigits();
void deleteNumber();
void activateDigitWithNumber();

void showNewData();
boolean recvWithEndMarker();
int parseData();

byte arr[16] = {
    B11111100, //0
    B01100000, //1
    B11011010, //2
    B11110010, //3
    B01100110, //4
    B10110110, //5
    B10111110,  //6 
    B11100000, //7
    B11111110, //8
    B11110110, //9
    B11101110, //A
    B11111110, //B
    B10011100, //C
    B11111100, //D
    B10011110,  //E 
    B10001110 //F
};


////////////////////////////////////////SETUP/////////////////////////////////
void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  setupDigits();
  setupBars();
}

////////////////////////////////////////MAIN LOOP/////////////////////////////////
void loop() {
  if(programStarted){
    for(int i = 4; i>=1; i--){
      digitChanger(i);
      activateDigitWithNumber();
      delay(400);
    }
  
   } else{
      activateDigitWithNumber();
   }
}

void digitChanger(int digitToStart){
    startDigit(digitToStart);
    if(digitToStart == 4){
      stopDigit(1);
    } else {
      stopDigit(digitToStart+1);
    }
}

void activateDigitWithNumber(){
  if(recvWithEndMarker()){
    showNewData();
    int parsed = parseData();
    deleteNumber();
    writeNumber(parsed);
    if(!programStarted){
      startProgram();
    }
  }
}

boolean startProgram(){
  programStarted = true;
  Serial.println("Program started");
}

void writeNumber(int whichNumber){
  for(byte i = 7; i >= 1;i--){
    if(bitRead(arr[whichNumber], i)){
      digitalWrite(DOT -i, LOW);
    }
  }
}

void deleteNumber(){
  for(int i =6; i<= 12; i++){
    digitalWrite(i, HIGH);
  }
}

void setupDigits(){
  for(int i=2; i<=5;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i, LOW);
  }
}

void setupBars(){
  for(int i = 6; i<=13; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

void startDigit(int digit){
  digitalWrite(1+digit, HIGH);
}

void stopDigit(int digit){
  digitalWrite(1+digit, LOW);
}

boolean recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 while (Serial.available() > 0 && newData == false) {
  rc = Serial.read();

  if (rc != endMarker) {
    receivedChars[ndx] = rc;
    ndx++;
    if (ndx >= numChars) {
      ndx = numChars - 1;
    }
   }
   else {
    receivedChars[ndx] = '\0'; // terminate the string
    ndx = 0;
    newData = true;
    return true;
   }
 }
 return false;
}

void showNewData() {
 if (newData == true) {
 Serial.print("This just in ... ");
 Serial.println(receivedChars);
 newData = false;
 }
}

int parseData(){
   int numberRead = atoi(receivedChars);
   Serial.print("I read number: ");
   Serial.println(numberRead);
   return numberRead;
}
