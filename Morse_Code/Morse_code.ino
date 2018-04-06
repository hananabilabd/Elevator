
int button1=4;
int button2=5;
int button3=6;
int button4=7;


int led = 11;           // the pin that the LED is attached to

void setup(){
  Serial.begin(9600);
//  lcd.begin(16, 2);
// lcd.setCursor(0,0);
  pinMode(led, OUTPUT);
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  }

void loop(){
  analogWrite(led, 0); //500 is the degree of the brightness
  
//This code prints SOS
  
   if( digitalRead(button1)==HIGH) //S
   {
     dot();
     dot();
     dot();
    }
    else if(digitalRead(button2)==HIGH) //O
    {
       dash();
       dash();
       dash();
      }

       
  }

 void dash(){
    analogWrite(led, 500);
    delay(3000);
    analogWrite(led, 0);
    delay(1000);
    }

  void dot(){
    analogWrite(led, 500);
    delay(1000);
    analogWrite(led, 0);
    delay(1000);
    }
