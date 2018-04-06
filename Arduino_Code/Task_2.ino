#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
int bluetoothTx = 7;
int bluetoothRx = 8;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
String state;
String dist;// string to store incoming message from bluetooth
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

int A[10]={0,0,0,0,0,0,0,0,0,0};
int B[10]={0,0,0,0,0,0,0,0,0,0};
int C[10]={0,0,0,0,0,0,0,0,0,0};
int Elevator[10]={0,0,0,0,0,0,0,0,0,0};
int temp[10]={0,0,0,0,0,0,0,0,0,0};

//stepper
int output1=2;
int output2=3;
int output3=4;
int output4=5;

char stops;
int A_requests=0;
int B_requests=0;
int C_requests=0;
int time_A=0;
int time_B=0;
int time_C=0;
char abs_distance=0;
char Time=0;
int n1=0;
int n2=0;

void bluetooth_func();

void add_call(char from, char going); //Which elevator reponds to the call? Add the call to its queue.
char time_calculation(char Final,char Position,char call, char elev); //calculate time for each elevator
char pass (char Final ,char Position, char call, char elev); //Function to tell if the elevator passes by the caller

void stepper();
void clockwise();
void anticlockwise();

void lcd_display();

 
void setup() {
Serial.begin(9600);
pinMode(output1, OUTPUT);
pinMode(output2, OUTPUT);
pinMode(output3, OUTPUT);
pinMode(output4, OUTPUT);
lcd.begin(16, 2);
lcd.setCursor(0,0);
bluetooth.begin(9600);


}

void loop() {


bluetooth_func();

lcd_display();
stepper();



if(!(n1==0&& n2==0)) {add_call(n1, n2);}
 

Serial.print(" A queue : ");
 for(char i=0;i<10;i++){
 
 Serial.print(A[i]);
 }
 Serial.println("  ");
 Serial.print(" B queue : ");
  for(char i=0;i<10;i++){
    
 Serial.print(B[i]);
 }
 Serial.println("  ");

  Serial.print(" C queue : ");
  for(char i=0;i<10;i++){
   
 Serial.print(C[i]);
 }
 Serial.println("  ");


 
}

//--------------------------------------------------------------------------

 void bluetooth_func()
 {


  while (bluetooth.available()>0){  //Check if there is an available byte to read
  //delay(10); //Delay added to make thing stable 
  unsigned int r = bluetooth.read(); //Conduct a serial read
  state += r ;

  
  if (state.length() ==2){
    int d = state.toInt();
     n1 = (d/10)%10;  //from
     n2 = d%10;       //going

    
  Serial.print("Call from  ");
  Serial.print(n1);
  Serial.print(" going to  ");
  Serial.println(n2);

  
  state="";}
 

}
  
  } 



//--------------------------------------------------------------------- 

// How to respond to a call

void add_call(char from, char going)
{  
  int p=0;

   //This part is to check if the call is already in the queue of one of the elevators
//- - -      - - -      - - -      - - -     - - -     - - -   
  char duplicate =0;
 int Check[10]={0,0,0,0,0,0,0,0,0,0};
for (int i=0; i<3; i++)

{
  if (i==0) { for (int k=0; k<=9; k++) {Check[k] = A[k];}}
  if (i==1) { for (int k=0; k<=9; k++) {Check[k] = B[k];}}
  if (i==2) { for (int k=0; k<=9; k++) {Check[k] = C[k];}}
  
for (int m=0; m<10; m++)
{
  if (Check[m] == from && i==0 ) {duplicate='a';}
  if (Check[m] == from && i==1 ) {duplicate='b';}
  if (Check[m] == from && i==2 ) {duplicate='c';}
}
 
} 
//- - -      - - -      - - -      - - -     - - -     - - -

//If any elevator is empty no need to calculate time
    if(A[0]==0)
    {    
       A[A_requests++]=from;
       A[A_requests++]=going;
    }
    
    else if(B[0]==0)
    {
        B[B_requests++]=from;        
        B[B_requests++]=going;
    }
    else if(C[0]==0)
    {
          C[C_requests++]=from;
          C[C_requests++]=going;
    }


//If all elevators have queues, calculate time to know the fastest    
    else
    {     
          time_A= time_calculation(A[A_requests],A[0],from, 'a');
          time_A+= time_calculation(A[A_requests],from, going, 'a');
          time_B= time_calculation(B[B_requests],B[0],from, 'b');
          time_B+= time_calculation(B[B_requests],from, going, 'b');
          time_C= time_calculation(C[C_requests],C[0],from, 'c');
          time_C+= time_calculation(C[C_requests], from, going, 'c');
          Serial.print("Time for A : ");
          Serial.print(time_A); Serial.println(" Secs");
          Serial.print("Time for B : ");
          Serial.print(time_B); Serial.println(" Secs");
          Serial.print("Time for C : ");
          Serial.print(time_C); Serial.println(" Secs");
          //show the min number of steps  
          char min1=min(time_A,time_B);
          char min2=min(min1,time_C); 
          Serial.println(" ");        
          if (min2==time_A)
          {
            if (duplicate=='a') {A[A_requests++]=going;} //If the call is duplicated, just add the destination to the same elevator
            else {A[A_requests++]=from;
                  A[A_requests++]=going;}
            
           }
            else if(min2==time_B)
            {
              if (duplicate=='b') {B[B_requests++]=going;} //If the call is duplicated, just add the destination to the same elevator
              else{B[B_requests++]=from;
                   B[B_requests++]=going;}
              
            }
            else
            {   
              if (duplicate=='c') {C[C_requests++]=going;} //If the call is duplicated, just add the destination to the same elevator
              else {C[C_requests++]=from;
                C[C_requests++]=going;}
                
            }
       }

  //Reset counters     
     if (A_requests>=10) {A_requests=0;}
     if (B_requests>=10) {B_requests=0;} 
     if (C_requests>=10) {C_requests=0;}
     
       } 

//----------------------------------------------------------------- 

//Calculate time for elevator to arrive to a call

char time_calculation(char Final,char Position,char call, char elev)
  {

  if (elev=='a') {for (int i=0; i<=9; i++) {Elevator[i] = A[i];}}
  if (elev=='b') {for (int i=0; i<=9; i++) {Elevator[i] = B[i];}}
  if (elev=='c') {for (int i=0; i<=9; i++) {Elevator[i] = C[i];}}
  
 
   
    stops=pass(Final,Position,call, elev);
   
     abs_distance=abs((Final-Position)+(call-Final));
     Time= abs_distance*2 + stops*5;

     //Correction factor if some distances cancel each other
     if (((Final>Position)&&(Position>call)) || ((Final<Position)&&(Position<call)))
     {
      Time=+2*abs(Final-Position)*2;
     }
      return Time;
  }

//-------------------------------------------------------------------------
  
  // calculate number of stops

  
char pass (char Final ,char Position, char call, char elev)
{ 
  int p=0; //Will it pass on the call?
  int no_stops=0;
 //If it will pass, count no of stops only until it passes on the call
  
  if(((Final>Position)&&(Position<call)&&(call<Final))) //Going up and passing, count until it's bigger than the call
  { p=1;
  
    for(char i=0;i<10 ;i++)
    {
      if((Elevator[i]<call)&&(Elevator[i] != 0))
      {
        no_stops++;
        }
      }
      
    }
    if(((Final<Position)&&(Position>call)&&(call>Final))) //Going down and passing, count until it's smaller than the call
    { p=1;

      for(char i=0;i<10 ;i++)
    {
      if((Elevator[i]>call)&&(Elevator[i] != 0))
      {
        no_stops++;
        }
      }
      }
      
        if (p==1)
  {return no_stops;}

  //If it will not pass, return the number of the requests in the queue
  else 
 {
  if (elev=='a') {return(A_requests);}
  if (elev=='b') {return(B_requests);}
  if (elev=='c') {return(C_requests);}
  }

  }
//-----------------------------------------------------------------------


   void lcd_display()
    {
         lcd.setCursor(0,0);
         lcd.print("A");
          delay(3000);
          
         lcd.setCursor(0,1);
         lcd.print(A[0]);
         delay(300);
        
        lcd.setCursor(5,0);
         lcd.print("B");
          delay(3000);
          
         lcd.setCursor(5,1);
         lcd.print(B[0]);
         delay(300);
         
        lcd.setCursor(10,0);
         lcd.print("C");
          delay(3000);
          
          lcd.setCursor(10,1);
         lcd.print(C[0]);
         delay(300);
      }



void stepper()
{
  //up
 if(A[1]>A[0]){
       if( (A[1]-A[0])==1)
       {
        for(int j=0;j<230;j++){clockwise(); }
      
        }
        else if( (A[1]-A[0])==2)
       {
        for(int j=0;j<460;j++){clockwise(); }
      
        }
        else if((A[1]-A[0])==3)
        {
          for(int j=0;j<690;j++){clockwise(); }
          }
      
           else if((A[1]-A[0])==4)
        {
          for(int j=0;j<920;j++){clockwise(); }
          }
          
        }


        //down
    if(A[1]<A[0]){
       if( (A[0]-A[1])==1)
       {
        for(int j=0;j<230;j++){anticlockwise(); }
      
        }
        else if( (A[0]-A[1])==2)
       {
        for(int j=0;j<460;j++){anticlockwise(); }
      
        }
        else if((A[0]-A[1])==3)
        {
          for(int j=0;j<690;j++){anticlockwise(); }
          }
      
           else if((A[0]-A[1])==4)
        {
          for(int j=0;j<920;j++){anticlockwise(); }
          }
          
        }
}   
void anticlockwise(){

    int n;
          //for(n=0;n<2;n=n+1)
          //{
      
           
             
           
       digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,HIGH);
        delay(10);   

        digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,HIGH);
       digitalWrite(output4,LOW);
     delay(10);
        digitalWrite(output1,LOW);
       digitalWrite(output2,HIGH);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
     
             delay(10);
        digitalWrite(output1,HIGH);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
  
              delay(10);
      
          //}
      }

      void clockwise(){

    int n;
          //for(n=0;n<2;n=n+1)
          //{
       digitalWrite(output1,HIGH);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
  
           
              delay(10);

        digitalWrite(output1,LOW);
       digitalWrite(output2,HIGH);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
     
             delay(10);


               digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,HIGH);
       digitalWrite(output4,LOW);
    
            delay(10);
       digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,HIGH);
              delay(10);
      
          //}
      }

    


