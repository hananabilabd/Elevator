#define IRpin_PIN PIND
#define IRpin 2
#define IRpin_2 3

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
 
// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20
 
// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2]; // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

int number = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
  Serial.println(number);
}
 
void loop() {
    in();
    out();
    if(number<=0) {number=0;}
//    if(number>=10) {disable elevator();}
        Serial.print(number);
        }
void in(){
  uint16_t highpulse, lowpulse; // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length
  
// while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
     // pin is still HIGH 
     // count off another few microseconds
     highpulse++;
     delayMicroseconds(RESOLUTION);
 
     // If the pulse is too long, we 'timed out' - either nothing
     // was received or the code is finished, so print what
     // we've grabbed so far, and then reset
     if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
//       printpulses();
       currentpulse=0;
       return;
     }
  } 
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
     // pin is still LOW
     lowpulse++; 
     delayMicroseconds(RESOLUTION);
     if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
      
//       printpulses();
       return;
     }
  }
  pulses[currentpulse][1] = lowpulse;
 
  // we read one high-low pulse successfully, continue!
  currentpulse++;
  uint16_t highpulse_2, lowpulse_2; // temporary storage timing
  highpulse_2 = lowpulse_2 = 0; // start out with no pulse length
  
// while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin_2)) {
     // pin is still HIGH 
     // count off another few microseconds
     highpulse_2++;
     delayMicroseconds(RESOLUTION);
 
     // If the pulse is too long, we 'timed out' - either nothing
     // was received or the code is finished, so print what
     // we've grabbed so far, and then reset
     if ((highpulse_2 >= MAXPULSE) && (currentpulse != 0)) {
//       printpulses();
       currentpulse=0;
       return;
     }
  } 
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse_2;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin_2))) {
     // pin is still LOW
     lowpulse_2++; 
     delayMicroseconds(RESOLUTION);
     if ((lowpulse_2 >= MAXPULSE) && (currentpulse != 0)) {
          return;
     }
  }
  pulses[currentpulse][1] = lowpulse;
 
  // we read one high-low pulse successfully, continue!
  currentpulse++;
  number--;
  }


  void out(){
  uint16_t highpulse, lowpulse; // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length
  
// while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin_2)) {
     // pin is still HIGH 
     // count off another few microseconds
     highpulse++;
     delayMicroseconds(RESOLUTION);
 
     // If the pulse is too long, we 'timed out' - either nothing
     // was received or the code is finished, so print what
     // we've grabbed so far, and then reset
     if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
//       printpulses();
       currentpulse=0;
       return;
     }
  } 
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin_2))) {
     // pin is still LOW
     lowpulse++; 
     delayMicroseconds(RESOLUTION);
     if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
      
//       printpulses();
       return;
     }
  }
  pulses[currentpulse][1] = lowpulse;
 
  // we read one high-low pulse successfully, continue!
  currentpulse++;
  uint16_t highpulse_2, lowpulse_2; // temporary storage timing
  highpulse_2 = lowpulse_2 = 0; // start out with no pulse length
  
// while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
     // pin is still HIGH 
     // count off another few microseconds
     highpulse_2++;
     delayMicroseconds(RESOLUTION);
 
     // If the pulse is too long, we 'timed out' - either nothing
     // was received or the code is finished, so print what
     // we've grabbed so far, and then reset
     if ((highpulse_2 >= MAXPULSE) && (currentpulse != 0)) {
       currentpulse=0;
       return;
     }
  } 
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse_2;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
     // pin is still LOW
     lowpulse_2++; 
     delayMicroseconds(RESOLUTION);
     if ((lowpulse_2 >= MAXPULSE) && (currentpulse != 0)) {
             return;
     }
  }
  pulses[currentpulse][1] = lowpulse_2;
 
  // we read one high-low pulse successfully, continue!
  currentpulse++;
  number++;
  }
