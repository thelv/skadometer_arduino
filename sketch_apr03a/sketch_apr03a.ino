volatile bool sigOdd=false;
volatile int sigT=-1;
volatile int sigOddT=-1;
volatile int sigOddCount=0;

int sigOddT_=-1;
int sigOddCount_=0;

float v;

void setup() 
{
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);    
  pinMode(4, OUTPUT);   
  pinMode(5, OUTPUT);   
  pinMode(5, HIGH);   
  digitalWrite(4, HIGH); 
  
  Serial.begin(19200);

  attachInterrupt(digitalPinToInterrupt(2), sig, CHANGE);  
}

void loop() 
{
  Serial.println("asd");
 // if(sigOddCount==sigOddCount_) return;

  if(sigOddCount_!=0)
  {
    v=(float)(sigOddCount-sigOddCount_)/(sigOddT-sigOddT_)*1000;
    Serial.println(v);
  }

  sigOddCount_=sigOddCount;
  sigOddT_=sigOddT;


  Serial.println('--');
  delay(500);
}

void sig()
{
  Serial.println('a');
  int t=millis();
  if(t-sigT>1)
  {
    sigOdd=! sigOdd;
    if(sigOdd & 1)
    {
      sigOddCount++;
      sigOddT=t;
    }
  }
  sigT=t;
}
