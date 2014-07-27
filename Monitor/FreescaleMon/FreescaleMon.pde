import processing.serial.*;

Serial myPort;
PImage carImage;

int line;
int wheel;
int speed;
int ramp;

void setup() 
{
  size(600, 600);
  
  // Car image
  carImage = loadImage("car.png");
  
  // Serial setup
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  
  int line=0;
  int wheel=0;
  int speed=0;
  int ramp=0;
  
  textAlign(CENTER,BOTTOM);
  
  background(0);
  fill(255);
  
  // Labels
  text("Line",width*0.2,height*0.9);
  text("Turn",width*0.4,height*0.9);
  text("Speed",width*0.6,height*0.9);
  text("Ramp",width*0.8,height*0.9);
}


void draw()
{
  rect(.25*width,0,width*0.5,height*0.5);
  
  updateStatus();
  displayStatus();
  moveCar();
}

void displayStatus()
{
  text(line,width*0.2,height*0.9);
  text(wheel,width*0.4,height*0.9);
  text(speed,width*0.6,height*0.9);
  
  String rampText = "-";
  if(ramp == -1)
    rampText = "UP";
  if(ramp == 1)
    rampText = "DOWN";
  text(rampText,width*0.8,height*0.9);
}

void moveCar()
{
  println(0.25*width+((line+100.0)/200.0)*(width*0.5));
  image(carImage,0.25*width+((line+100.0)/200.0)*(width*0.5-carImage.width),.1*height);
  println(line);
}

void updateStatus()
{
  byte[] inBuffer = new byte[4];
  
  while(myPort.available() < 4);
  
  inBuffer = myPort.readBytes();
  if(inBuffer != null)
  {
    line  = int(inBuffer[0])-100;
    wheel = int(inBuffer[1])-100;
    speed = int(inBuffer[2]);
    ramp  = int(inBuffer[3])-1;
  }
}


void keyPressed() {
  switch(key)
  {
    default:
      myPort.write(key);
      break;
  }
}
