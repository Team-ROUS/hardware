#define DACX 25
#define DACY 26

#define POS_MAX 255

#define CELL_COUNT 17 // factors 1, 3, 5, 15, 17, 51, 85, 255

byte CELL_LENGTH = POS_MAX / CELL_COUNT;

byte X_CURRENT = 0;
byte Y_CURRENT = 0;

char mapData[] = "LULULULULULULULULU";

void setup(){
  Serial.begin(9600);
}

void loop() { // Generate a Sine wave
  //int Value = 128; //255= 3.3V 128=1.65V

//  while(Serial.available()) {
//    mapData= Serial.readString();// read the incoming data as string
//    Serial.println(mapData);
//  }
//  
  for(int i =0; i < strlen(mapData); i++ ) {
    cellStep(mapData[i]);
  }
  for(int i = strlen(mapData)-1; i >= 0; i-- ) {
    cellStep(mapData[i]);
  }
}

void cellStep(char dir){
  if(dir == 'U'){
    Y_CURRENT += CELL_LENGTH;
  }
  if(dir == 'D'){
    Y_CURRENT -= CELL_LENGTH;
  }
  if(dir == 'L'){
    X_CURRENT -= CELL_LENGTH;
  }
  if(dir == 'R'){
    X_CURRENT += CELL_LENGTH;
  }
  dacWrite(DACX, X_CURRENT);
  dacWrite(DACY, Y_CURRENT);
  delayMicroseconds(1000);
}
