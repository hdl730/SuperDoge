//const int IN_PIN_1[4]={1,2,3,4};


const int MAX_S=225;
const int EN_PIN[2]={10,11};                                                                                                                                                                                                                                                                                                         
const int BACK_PIN[2]={12,13};
const int MAIN_IR_PIN[3]={4,7,8};
//const int SUB_IR_PIN[2]={A2,A3};
int EN_VALUE[2]={200,200};
bool state[3]={1,1,1};
bool start=false;
bool stop=false;
void setup()
{
  for(int i=0;i<2;i++)
  {
    //pinMode(IN_PIN_1[i],OUTPUT);
    //pinMode(IN_PIN_0[i],OUTPUT);
    pinMode(EN_PIN[i],OUTPUT);
    pinMode(BACK_PIN[i],OUTPUT);
    analogWrite(BACK_PIN[i],0);
    pinMode(MAIN_IR_PIN[i],INPUT);
    //digitalWrite(IN_PIN_1[i],HIGH);
    //digitalWrite(IN_PIN_0[i],LOW);
  }
  pinMode(MAIN_IR_PIN[0],INPUT);
  pinMode(MAIN_IR_PIN[1],INPUT);
  pinMode(MAIN_IR_PIN[2],INPUT);
  
  //BIAS=analogRead(MAIN_IR_PIN[0])-analogRead(MAIN_IR_PIN[1]);
  Serial.begin(115200);
  //Serial.begin(9600);
}

void loop()
{
  mainLoop();
  readSerial();
  if(start){
  //setSpeedWithD();
  mainLoop();
  }
  for(int i=0;i<3;i++)
  {
    state[i]=digitalRead(MAIN_IR_PIN[i]);
  }
  //Serial.println("0:"+(String)state[0]+" 1:"+(String)state[1]+" 2:"+(String)state[2]);
}

void readSerial()
{
  if (Serial.available()>0)
  {
    //delay(100);
    int i=Serial.parseInt();
    Serial.println(i);
    if (i%7 == 0)
    {
      i/=7;
      int mode=i%10;
      int value=i/10;
      Serial.println(mode);
      Serial.println(value);
      if (mode<2)
      {
        if(i/10>=200 || i/10 == 0)
          EN_VALUE[i%10]=i/10;
      }
      if(mode==3)
      {
        Serial.println(analogRead(MAIN_IR_PIN[0]));
        Serial.println(analogRead(MAIN_IR_PIN[1]));
      }
      if(mode==4)
      {
        start=true;      
      }
    }
  }
}

void mainLoop(){
  Serial.println("Mission begin.");
  //goon(-150);
  goon(200);
  //setSpeedWithValue(0,255);
  //setSpeedWithValue(1,255);
  turn(1);
  delay(1000);
  //while(1){}
}

void goon(int s){
  Serial.println("Walking along the line...");
  //setSpeedWithValue(0,255);
  //setSpeedWithValue(1,255);
  while(1){
    if(!setSpeedWithD(s)){
      break;
    }
  }
}
void turn(int dir){/*
  setSpeedWithValue(dir,200);
  setSpeedWithValue(1-dir,200);
  delay(100);
  Serial.println("Turning...");
  setSpeedWithValue(dir,200);
  setSpeedWithValue(1-dir,0);
  analogWrite(BACK_PIN[1-dir],200);
  state[1]=digitalRead(MAIN_IR_PIN[1]);
  int x=(dir==1?0:2);
  while(state[x]){state[x]=digitalRead(MAIN_IR_PIN[x]);}
  while(state[0] && state[1]){}
  while(!state[x]){state[x]=digitalRead(MAIN_IR_PIN[x]);}
  //setSpeedWithValue(dir,0);
  //setSpeedWithValue(1-dir,0);
  analogWrite(BACK_PIN[1-dir],0);
  */
  delay(500);
  setSpeedWithValue(dir,-200);
  setSpeedWithValue(1-dir,-200);
  delay(700);
  setSpeedWithValue(dir,0);
  setSpeedWithValue(1-dir,0);
  delay(500);
  setSpeedWithValue(dir,150);
  setSpeedWithValue(1-dir,0);
  delay(750);
  setSpeedWithValue(dir,150);
  setSpeedWithValue(1-dir,150);
  int x=(dir==1?2:0);
  while(state[x]){state[x]=digitalRead(MAIN_IR_PIN[x]);}
  while(!state[x]){state[x]=digitalRead(MAIN_IR_PIN[x]);}
  delay(500);
  setSpeedWithValue(dir,150);
  setSpeedWithValue(1-dir,0);
  //while(1);
}

bool setSpeedWithD(int s)
{
  for(int i=0;i<3;i++)
  {
    state[i]=digitalRead(MAIN_IR_PIN[i]);
  }
  if(state[0] && state[2])
  {
    Serial.println("STOP");
    setSpeedWithValue(0,0);
    setSpeedWithValue(1,0);
    //delay(1000);
      return false;
  }
  if(state[0])
  {
    //Serial.println("Sensor0 triggered.");
    setSpeedWithValue(1,s);
    setSpeedWithValue(0,0);
    //delay(1000);
    return true;
  }
  if(state[2])
  {
    //Serial.println("Sensor2 triggered.");
    setSpeedWithValue(0,s);
    setSpeedWithValue(1,0);
    //delay(1000);
    return true;
  }
  if(!state[1])
  {
    //Serial.println("Main sensor lose its target.");
    return true;
  }
  setSpeedWithValue(0,s);
  setSpeedWithValue(1,s);
  return true;
}

void setSpeedWithValue(int index,int value)
{
  EN_VALUE[index]=value;
  if (value>255){value=255;Serial.println("Error: invalid speed value");}
  //if (value<200){value=0;}
  if(value<0)
  {
    analogWrite(EN_PIN[index],0);
    analogWrite(BACK_PIN[index],-value);
  }
  else{
    analogWrite(EN_PIN[index],value);
    analogWrite(BACK_PIN[index],0);
  }
}
