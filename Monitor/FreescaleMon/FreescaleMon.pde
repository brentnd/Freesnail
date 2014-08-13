import processing.serial.*;

Serial myPort;
PImage carImage;
PImage wheelImage;

String typedText = "Tuning Parameters:";

int line;
int wheel;
int speed;
int ramp;

int w;

byte p = 2;
byte p2 = 2;
byte i = 0;
byte i2 = 9;
byte d = 7;
byte d2 = 6;
byte stopgo = 1;

void setup() 
{
  size(600, 600);
  
  // Car image
  carImage = loadImage("car.png");
  
  // Serial setup
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  
  line=0;
  wheel=0;
  speed=0;
  ramp=0;
  
  w = int(0.02*width); 
  
  textAlign(CENTER,BOTTOM);
  
  background(0);
  fill(255);
  
  
  // Labels
  text("Line",width*0.2,height*0.9);
  text("Turn",width*0.4,height*0.9);
  text("Speed",width*0.6,height*0.9);
  text("Ramp",width*0.8,height*0.9);
  textSize(26);
}


void draw()
{
  displayStatus();
  
  fill(255);
  rect(.25*width,0,width*0.5,height*0.5);
  showCar();
  stroke(255);
  showWheel();
  showSpeed();
  showGoStop();  
  
  noStroke();
  fill(0);
  rect(0, 2*height/3-40, width, 60);
  fill(255);
  text(typedText, width/2, 2*height/3-10);
  text("Kp: "+p+"."+p2+"  Ki: "+i+"."+i2+"  Kd: "+d+"."+d2, width/2, 2*height/3+20);
  
  updateStatus();
}

void displayStatus()
{
  fill(0);
  rect(0,height*0.9,width,height*0.1);
  fill(255);
  float h = height*0.95;
  text(line,width*0.2,h);
  text(wheel,width*0.4,h);
  text(speed,width*0.6,h);
  
  String rampText = "-";
  if(ramp == 1)
    rampText = "UP";
  if(ramp == -1)
    rampText = "DOWN";
  text(rampText,width*0.8,h);
}

void showCar()
{
  float xpos = 0.25*width+((-line+100.0)/200.0)*(width*0.5-carImage.width);
  image(carImage,xpos,.1*height);
}

void showWheel()
{
  fill(0,40);
  rect(.25*width,0.5*height,width*0.5,w*3);
  float xpos = 0.25*width+((wheel+100.0)/200.0)*(width*0.5-w);
  fill(0,0,255);
  rect(xpos,.5*height,w,w*3);
}

void showSpeed()
{
  int tspeed = speed;
  fill(0,40);
  rect(.1*width,0,w*3,height*0.5);
  noStroke();
  fill(0);
  rect(0.05*width,0.505*height,100,50);
  stroke(255);
  fill(255);
  if(tspeed < 0)
  {
    text("Brake",0.13*width,0.55*height);
    tspeed = 0;
  }
  float ypos = (1-(tspeed/100.0))*(width*0.5-w);
  // Red
  if(tspeed < 10)
    fill(255,0,0);
  else
    fill(350-tspeed*3,180,0);
  rect(.1*width,ypos,w*3,w);
}

void showGoStop()
{
  String te;
  if(stopgo == 1)
  {
    fill(0,255,0);
    te = "Go!";
  }else
  {
    fill(255,0,0);
    te = "STOP";
  }
  rect(0.82*width,0.1*height,100,30);
  fill(255);
  text(te,0.9*width,0.15*height);
}

void updateParams()
{
  myPort.write(p);
  myPort.write(p2);
  myPort.write(i);
  myPort.write(i2);
  myPort.write(d);
  myPort.write(d2);
  myPort.write(stopgo);
  typedText = "Tuning Parameters:";
}

void updateStatus()
{
  byte[] inBuffer = new byte[4];
  
  if(myPort.available() < 4)
    return;
  
  inBuffer = myPort.readBytes();
  if(inBuffer != null)
  {
    if((inBuffer[0]==3)
      &(inBuffer[1]==2)
      &(inBuffer[2]==1)
      &(inBuffer[3]==0))
    {
      println("Starting control loop...");
    }
    else if((inBuffer[0]==0)
           &(inBuffer[1]==1)
           &(inBuffer[2]==2)
           &(inBuffer[3]==3))
    {
      println("FINISH LINE...");
    }
    else
    {
      line  = int(inBuffer[0])-100;
      wheel = int(inBuffer[1])-100;
      speed = int(inBuffer[2])-100;
      ramp  = int(inBuffer[3])-1;
    }
  }
}

void parseText()
{
  String[] piece = split(typedText, ":");
  String[] tokens = splitTokens(piece[1],"., ");
  p = (byte)int(tokens[0]);
  p2 = (byte)int(tokens[1]);
  i = (byte)int(tokens[2]);
  i2 = (byte)int(tokens[3]);
  d = (byte)int(tokens[4]);
  d2 = (byte)int(tokens[5]);
}

void mousePressed() 
{
  stopgo = byte(1-stopgo);
  updateParams();
}

void keyPressed() {
  switch(key)
  {
    case BACKSPACE:
      typedText = typedText.substring(0,max(0,typedText.length()-1));
      break;
    case ENTER:
    case RETURN:
      parseText();
      updateParams();
      break;
    case ESC:
    case DELETE:
      break;
    default:
      typedText += key;
      break;
  }
}
