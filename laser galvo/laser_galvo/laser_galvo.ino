#define DACX 25
#define DACY 26

#define POS_MAX 255

#define CELL_COUNT 17 // factors 1, 3, 5, 15, 17, 51, 85, 255

byte CELL_LENGTH = POS_MAX / CELL_COUNT;

byte x_current = 0;
byte y_current = 0;


byte x_start[] = {0,2,0, 0, 0};
byte y_start[] = {6,0,12,14,0};
char mapData[][100] = {
  "rrrrrrrrrrruuurrrr",
  "rrrrruuuuurrrrruuurrr",
  "rrrrrrdddlllddddrrrrrruuuuurrrddddddddd",
  "rddrruurrrrrrdldlulldddrrrrrurrdddlldllululllddrrddllluuuldddddluuuuuldddddddrrrrruurrddrrrrruuulll",
  "ruurdrruuuuluuurrdruullllluuurrdruuurddddrrdddrddddrurddruuruuuuruuullldldrrdllluuuuurrrrdrr"
};
                    
byte level = 2;

byte arrow[5][2] = {{128,0},
                  {128,255},
                  {78,170},
                  {178,170},
                  {128,255}};
                  
unsigned long previousMillis = 0;  
const long interval = 1000;

byte mouseX = map(12,0,14,0,255);
byte mouseY = map(3,0,14,0,255);

void setup(){
  Serial.begin(9600);
}

int curidx = 10;
void loop() { // Generate a Sine wave
  //int Value = 128; //255= 3.3V 128=1.65V

  int charidx = 0;
  char charBuf[50];
  while(Serial.available()) {
    char k = Serial.read();
    if (k != '\n') {
      charBuf[charidx++] = k;
    }
  }
  charBuf[charidx++] = '\0';
  if (charidx > 1) {
    curidx = atoi(charBuf);
  }

  y_current = y_start[level] * CELL_LENGTH;
  x_current = x_start[level] * CELL_LENGTH;
  
  for(int i =0; i < strlen(mapData[level]); i++ ) {
    cellStep(mapData[level][i], i, curidx);
  }
  //drawMouse(mouseX, mouseY);
  drawMouse(mouseX, mouseY);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

//    level++;
//    if(level == 5){
//      level = 0;
//    }
    //Serial.println(level);
  }
  
}

void cellStep(char dir, int i, int curidx){
  if(dir == 'u'){
    y_current += CELL_LENGTH;
  }
  if(dir == 'd'){
    y_current -= CELL_LENGTH;
  }
  if(dir == 'l'){
    x_current -= CELL_LENGTH;
  }
  if(dir == 'r'){
    x_current += CELL_LENGTH;
  }
  dacWrite(DACX, x_current);
  byte yFlipped = map(y_current,0,255,255,0);
  dacWrite(DACY, yFlipped);
  delayMicroseconds(300);

  if (i == curidx) {
    drawMouse(x_current, yFlipped);
  }
  
  //Serial.printf("%d %d %d %d\n", mouseX,mouseY,x_current,yFlipped);
//  if(mouseX == x_current && mouseY == yFlipped){
//    
//  }
}

void drawArrow(int angle){
  for(int i = 0; i < 5; i++){
    dacWrite(DACX, arrow[i][0]);
    dacWrite(DACY, arrow[i][1]);
    delayMicroseconds(400);
  }
}

void drawMouse(byte x, byte y){
  dacWrite(DACX, x+5);
  dacWrite(DACY, y+5);
  delayMicroseconds(200);
  dacWrite(DACX, x-5);
  dacWrite(DACY, y-5);
  delayMicroseconds(200);
  dacWrite(DACX, x);
  dacWrite(DACY, y);
  delayMicroseconds(200);
  dacWrite(DACX, x+5);
  dacWrite(DACY, y-5);
  delayMicroseconds(200);
  dacWrite(DACX, x-5);
  dacWrite(DACY, y+5);
  delayMicroseconds(200);
}
