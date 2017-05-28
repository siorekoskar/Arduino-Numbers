#define MAXDIGITS 3

int dark = 10;
int medium = 100;
int high = 255;


int numberRight;
byte digits[4] = {3,5,6,9};
byte pins[8] = {2,4,7,8,10,11,12,13};
const byte numChars = 32;
char receivedChars[numChars]; 
boolean newData = false;
boolean programStarted = false;
int speedt = 4500;
int brightness =255;
int brightns[4] = {255,255,255,255};
int nums[6] = {0,1,2,3,4,5};

byte string[6];

void writeNumber(int whichNumber);
void setupBars();
void setupDigits();
void stopDigit(int num);
void deleteNumber();
void activateDigitWithNumber();
void readSpeed();

void showNewData();
boolean recvWithEndMarker();
int parseData();

const int numTable[16] = {
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
  
    
  
  numberWriter(nums);

}

void writeOnDigit(int number, int digit){
  analogWrite(digits[digit], 250);
    writeNumber(number);
}

////////////////////////////////////////FUNCTIONS/////////////////////////////////

void numberWriter(int nums[6]){

  for(int i =10; i>=0; i--){
    
   for(int j =0 ; j<=10; j++){
    int z = 5;
    
   for(int d = 0; d<= 5; d++){
    startDigit(i-d);

  if(d == 0){
    stopDigit(i-1);
  } else{
    stopDigit(i);
  }

  if(d==0 || d==1){
    stopDigit(i-2);
  } else{
    stopDigit(i-1);
  }

  if(d==2 || d == 1 || d==0){
    stopDigit(i-3);
  } else{
    stopDigit(i-2);
  }

  if(d != 5 && d != 4){
    stopDigit(i-4);
  } else {
    stopDigit(i-3);
  }

  if(d != 5){
    stopDigit(i-5);
  } else {
    stopDigit(i-4);
  }
  writeNumber(nums[z]);
  delayMicroseconds(speedt);
  z--;
}
  recvWithEndMarker();
    }
  }  
}



boolean startProgram(){
  programStarted = true;
  Serial.println("Program started");
}

void writeNumber(int whichNumber){
  for(byte i = 7; i >= 1;i--){
    if(bitRead(numTable[whichNumber], i)){
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
     analogWrite(digits[num], brightns[num]);
  }
}

void stopDigit(int num){
  deleteNumber();
  if(num <=3 && num >=0 ){
    //digitalWrite(digits[num], LOW);
    analogWrite(digits[num], 0);
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
    if(receivedChars[0] == 'D'){
      readSpeed();
    } else if( receivedChars[0] == 'B'){
      readBrghtns();
    } else if (receivedChars[0] == 'T'){
      setDigits();
    }
    return true;
   }
 }
 return false;
}

void setDigits(){
  for(int i =0; i<= 5; i++){
    char b = receivedChars[i+1];
    Serial.println(b);
    if(b>=48 && b<=57){
      nums[i] = b-48;
      Serial.println(nums[i]);
    } else if(b>=65 && b <=70){
      nums[i] = b-55;
      Serial.println(nums[i]);
    }
  }
  if(newData){
    newData = false;
  }
}

void showNewData() {
 if (newData) {
 Serial.print("This just in ... ");
 Serial.println(receivedChars);
 newData = false;
 }
}

int parseData(){
   int numberRead = atoi(receivedChars);
   Serial.print("I read number: ");
   Serial.println(numberRead);
   numberRight = numberRead;
   return numberRead;
}

void readBrghtns(){
  for(int i =0; i<=3; i++){
    char b = receivedChars[i+1];
    switch(b){
      case '0':
        brightns[i] = dark;
      break;
      case '1':
        brightns[i] = medium;
        break;
      case '2':
        brightns[i] = high;
       break;
    }
  }
  if(newData){
    newData = false;
  }
}

void readSpeed(){

  int numberRead = atoi(receivedChars+1);
  Serial.print("SPEED: ");
  Serial.println(numberRead);
  speedt = numberRead;
  if(newData){
    newData = false;
  }
}

