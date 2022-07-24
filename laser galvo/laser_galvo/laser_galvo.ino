#define DACX 25
#define DACY 26

#define POS_MAX 255

#define CELL_COUNT 17 // factors 1, 3, 5, 15, 17, 51, 85, 255

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CLAMP(x) MAX(MIN(x, 255), 0)

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
                    
int level = 2;

// TODO: delete
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

int curIdx = 10;
void finish_buf_segment(char* buf, int charidx, int argIdx) {
  buf[charidx++] = '\0';
  if (charidx > 1) {
    int argument = atoi(buf);
    if (argIdx == 0) {
      level = argument;
    }
    else if (argIdx == 1) {
      curIdx = argument;
    }
  }
}

void loop() { // Generate a Sine wave
  //int Value = 128; //255= 3.3V 128=1.65V

  int charidx = 0;
  int argIdx = 0;
  char buf[50];
  while(Serial.available()) {
    char k = Serial.read();
    if (k != '\n') {
      if (k == ',') {
        finish_buf_segment(buf, charidx, argIdx);
        charidx = 0;
        argIdx++;
      }
      else {
        buf[charidx++] = k;
      }
    }
  }
  finish_buf_segment(buf, charidx, argIdx);


  y_current = y_start[level] * CELL_LENGTH;
  x_current = x_start[level] * CELL_LENGTH;
  
  for(int i =0; i < strlen(mapData[level]); i++ ) {
    cellStep(mapData[level][i], i, curIdx);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
  
}

byte get_y_flipped(byte y) {
  return map(y_current,0,255,255,0);
}

void cellStep(char dir, int i, int curIdx) {
  if (i == curIdx) {
    drawMouse(x_current, get_y_flipped(y_current));
  }
  
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
  dacWrite(DACY, get_y_flipped(y_current));
  delayMicroseconds(300);
}

void drawArrow(int angle){
  for(int i = 0; i < 5; i++){
    dacWrite(DACX, arrow[i][0]);
    dacWrite(DACY, arrow[i][1]);
    delayMicroseconds(400);
  }
}

void drawMouse(int x, int y){
  dacWrite(DACX, CLAMP(x+5));
  dacWrite(DACY, CLAMP(y+5));
  delayMicroseconds(200);
  dacWrite(DACX, CLAMP(x-5));
  dacWrite(DACY, CLAMP(y-5));
  delayMicroseconds(200);
  dacWrite(DACX, CLAMP(x));
  dacWrite(DACY, CLAMP(y));
  delayMicroseconds(200);
  dacWrite(DACX, CLAMP(x+5));
  dacWrite(DACY, CLAMP(y-5));
  delayMicroseconds(200);
  dacWrite(DACX, CLAMP(x-5));
  dacWrite(DACY, CLAMP(y+5));
  delayMicroseconds(200);
  dacWrite(DACX, CLAMP(x));
  dacWrite(DACY, CLAMP(y));
  delayMicroseconds(200);
}
