#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15
#define G 16
#define DOT 13

byte digits[4] = {3,5,6,9};
byte pins[8] = {2,4,7,8,10,11,12,13};
const byte numChars = 32;
char receivedChars[numChars]; 
boolean newData = false;
boolean programStarted = false;
int speed = 400;

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

  startDigit(3);
  activateDigitWithNumber();
 /* if(programStarted){
    digitChanger(); 
  } else{
     activateDigitWithNumber();
  }*/
  //analogWrite(3, 15);
  //analogWrite(5, 50);
}

////////////////////////////////////////FUNCTIONS/////////////////////////////////
void writeMethod

void digitChanger(){
 /*   for(int i = 3; i>=0; i--){
      startDigit(digits[i]);
      startDigit(digits[i+1]);
 
      activateDigitWithNumber();
      delay(speed);
      if(i == 3){
        stopDigit(digits[0]);
      }
      if(i == 0){
      stopDigit(digits[1]);
      startDigit(digits[0]);
      stopDigit(digits[1]);
    } else {
     stopDigit(digits[i]);
      stopDigit(digits[i+1]);
    }

    }*/
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
      digitalWrite(pins[7 -i], LOW);
    }
  }
}

void deleteNumber(){
  for(int i =0; i<= 7; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void setupDigits(){
  for(int i=0; i<=3;i++){
    pinMode(digits[i],OUTPUT);
    digitalWrite(digits[i], LOW);
  }
}

void setupBars(){
  for(int i = 0; i<=8; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }
}

void startDigit(int num){
  if(num <=3 && num >=0){
    digitalWrite(digits[num], HIGH);
  }
}

void stopDigit(int num){
  if(num <=3 && num >=0 ){
    digitalWrite(digits[num], LOW);
  }
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
