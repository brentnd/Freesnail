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
  background(0);
  fill(255);
}


void draw()
{
  // While track
  fill(255,50);
  rect(.25*width,0,width*0.5,height);
  
  updateStatus();
  moveCar();
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
