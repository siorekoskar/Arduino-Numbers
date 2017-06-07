#define MAXDIGITS 3

int dark = 10;
int medium = 100;
int high = 255;

#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15

int numberRight;
byte digits[4] = {3,5,6,9};
byte pins[8] = {2,4,7,8,10,11,12,13};
const byte numChars = 32;
char receivedChars[numChars]; 
boolean newData = false;
boolean programStarted = false;
int DISP = 4500;
int brightness =255;
int BRIGHT[4] = {255,255,255,255};
int TEXT[6] = {0,1,2,3,4,5};

byte string[6];

void writeNumber(int whichNumber);
void setupBars();
void setupDigits();
void stopDigit(int num);
void deleteNumber();
void activateDigitWithNumber();
void readSpeed();

boolean recvWithEndMarker();

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
  numberWriter(TEXT);
}

////////////////////////////////////////FUNCTIONS/////////////////////////////////


/**
 * wyświetlanie napisu
 * @param: TEXT - tablica intów numerów do wyświetlenia
 */
void numberWriter(int TEXT[6]){

  for(int i =10; i>=0; i--){
    
   for(int j =0 ; j<=14; j++){
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
      writeNumber(TEXT[z]);
  delayMicroseconds(DISP);
  z--;
  
}

  recvWithEndMarker();
    }
  }  
}

/**
 * porównanie każdego bitu z tablicą stałych liczb numTable
 * @whichNumber - zmienna określająca który numer wyświetlić
 */ 
void writeNumber(int whichNumber){
  for(byte i = 7; i >= 1;i--){
    if(bitRead(numTable[whichNumber], i)){
      digitalWrite(pins[7 -i], LOW);
    }
  }
}

/**
 * wyłącza wszystkie ledy
 */
void deleteNumber(){
  for(int i =0; i<= 7; i++){
    digitalWrite(pins[i], HIGH);
  }
}

/**
 * ustawia wszystkie segmenty na output
 */
void setupDigits(){
  for(int i=0; i<=3;i++){
    pinMode(digits[i],OUTPUT);
    digitalWrite(digits[i], LOW);
  }
}

/**
 * ustawia wszystkie piny na output
 */
void setupBars(){
  for(int i = 0; i<=8; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }
}

/**
 * włącza dany segment i ustawia jego jasność
 * @sum - numer segmentu
 */
void startDigit(int num){
  if(num <=3 && num >=0){
     analogWrite(digits[num], BRIGHT[num]);
  }
}

/**
 * wyłącza dany segment
 * @num - numer segmentu
 */
void stopDigit(int num){
  deleteNumber();
  if(num <=3 && num >=0 ){
    analogWrite(digits[num], 0);
  }
}

/**
 * funkcja w pętli czyta każdy znak po kolei i zapisuje w naszej tablicy TEXT.
 * Za każdym razem sprawdzane jest,  czy dany znak nie jest znakiem końca linii
 * – jeżeli tak, do tablicy zapisywany jest znak końca stringa i w zależności od pierwszego znaku wywołuje odpowiednią funkcję.
 */
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
    } else {
      Serial.println("Komenda musi zaczynać się od 'T', 'D' lub 'B'");
      if (newData) {
        newData = false;
      }
      return false;
    }
    return true;
   }
 }
 return false;
}

//wczytywanie napisu z konsoli
void setDigits(){
  if(newData){
    newData = false;
  }
  for(int i=0; i<= 5; i++){
    char b = receivedChars[i+1];
    if(!(b>=48 && b<=57 || b>=65 && b <=70)) {
      Serial.println("Znak musi być z przedziału (0, F)");
      return;
    } 
  }
  if (receivedChars[8] != '\0') {
    Serial.println("Za długa komenda");
    return;
  }
  Serial.print("NAPIS: ");
  for(int i=0; i<= 5; i++){
    char b = receivedChars[i+1];
    Serial.print(b);
    if(b>=48 && b<=57){
      TEXT[i] = b-48;
    } else if(b>=65 && b <=70){
      TEXT[i] = b-55;
    } 
  }
  Serial.println();
}

/**
 * wczytuje jasność ekranu z konsoli
 */
void readBrghtns(){
  if(newData){
    newData = false;
  }
  for(int i=0; i<=3; i++){
    char b = receivedChars[i+1];
    if (b != '0' && b != '1' && b != '2') {
      Serial.println("Komenda musi się składać z 0, 1 lub 2");
      if (newData) {
        newData = false;
      }
      return;
    }
  }
  if (receivedChars[6] != '\0') {
    Serial.println("Za długa komenda");
    return;
  }
  Serial.print("JASNOŚĆ: ");
  for(int i =0; i<=3; i++){
    char b = receivedChars[i+1];
    switch(b){
      case '0':
        BRIGHT[i] = dark;
      break;
      case '1':
        BRIGHT[i] = medium;
        break;
      case '2':
        BRIGHT[i] = high;
       break;
    }
    Serial.print(b);
  }
  Serial.println();
}

/**
 * wczytuje prędkość z konsoli
 */
void readSpeed(){
  if(newData){
    newData = false;
  }
  int numberRead = atoi(receivedChars+1);
  if (numberRead < 1 || numberRead > 9999) {
    Serial.println("Czas musi być w granicach (1, 9999)");
    return;
  }
  Serial.print("CZAS: ");
  Serial.println(numberRead);
  DISP = numberRead;
}
