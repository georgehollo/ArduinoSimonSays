/*
 * Author: George Hollo
 * Date: 19/05/2021
 * Purpose: A simon says  game using arduino hardware. 4 LEDs
 * are set up to flash in a specicific randomized pattern. The user
 * then has to enter the sequence correctly (using the joystick) to
 * pass onto the next level. If the user gets the sequence wrong all
 * LEDs start flashing and the game is over. As you progress through
 * the levels the pattern gets longer and is showed for a shorter time.
 * 
 */

//Defining key constants
#define inJoyX A0 //Analog input for x-axis
#define inJoyY A1 //Analog input for x-axis
#define inButton 2 //Analog input for button
#define LOW 1
#define HIGH 0

#define MAX_LEVEL 100

//Define variables
int xValue,yValue,start; //All  used for joystick

int sequence[MAX_LEVEL]; //Array to store simon says sequence
int your_sequence[MAX_LEVEL]; //Array to store user inputted sequence
int level = 1; //Define start level as level 1

int velocity = 1000; //Time that each LED is turned on for when showing pattern (decreases as levels increase)

//Function Prototypes
void generateSeq();
void showSeq();
void getSeq();
void wrongSeq();
void rightSeq();
 
void setup() {
  //Setting up pins for Joystick
  pinMode(inJoyX, INPUT); //Setup x input
  pinMode(inJoyY, INPUT); //Setup y input
  pinMode(inButton, INPUT_PULLUP); //Setup button input

  //Setting up pins for LEDs
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);

  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  
  Serial.begin(9600);
}
 
void loop() {
  // put your main code here, to run repeatedly:

  //Read  state of button repetively
  start = digitalRead(inButton);

  if (start == 0 || level == 1) {
    generateSeq(); //When game starts generate a sequence
  }

  //If joystick is pressed (start  button) or level is not 1 (you  are  winning), show sequence and prompt user to copy
  if (start == 0 || level != 1) {
    showSeq();  //Show sequence on LEDs
    getSeq();  //Prompt user for repetition of sequence
  }
}

//Function to generate a random sequence of MAX_LEVEL length
void generateSeq() {
  randomSeed(analogRead(0)); //Set a completely random seed for the random() function by reading from a not connected pin
  
  //for loop to generate sequence of maximum length
  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(3,7); //Generate random number (number represents pin LED is attached to)
  }
}

//Have all LEDs flash 5 times when wrong sequence is entered and variables are reset
void wrongSeq() {
  int count = 0; //Variable to count repetitions of while loop
  //While loop to make lights flash 5 times
  while (count <  5) {
    //Turn all LEDs ON
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    //Wait 30ms
    delay(400);
    //Turn all LEDs OFF
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);

    count++;
  }

  //Reset variables
  level = 1;
  velocity = 1000;
}

//All LEDs turn ON and  then OFF, level is increased and speed sequence is shown at is decreased
void rightSeq() {
  //Turn all LEDs OFF
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  //Turn all LEDs ON
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  //Wait 75ms
  delay(750);
  //Turn all LEDs OFF
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);

  //Increase level if possible and speed shown
  if (level < MAX_LEVEL) {
    level++;
    velocity -= 50;
  }
  else {  //If final level is finished turn each light on seperately, repeat 5 times
    int count = 0;
    //Loop to make pattern with LEDs
    while (count <  5) {
      //Turn first LED ON
      digitalWrite(3,HIGH);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      //Wait 30ms
      delay(300);
      //Turn second LED ON
      digitalWrite(3,LOW);
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      //Wait 30ms
      delay(300);
      //Turn third LED ON
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      digitalWrite(6,LOW);
      //Wait 30ms
      delay(300);
      //Turn fourth LED ON
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      //Wait 30ms
      delay(300);
  
      count++;
    }
  }
}

//Display Simon says sequence using LEDs
void showSeq() {
  //Turn all LEDs OFF
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);

  //For loop to show sequence corresponding to level
  for (int i = 0; i < level; i++) {
    digitalWrite(sequence[i],HIGH); //Turn on LED given by sequence
    delay(velocity); //Wait for time corresponding to difficulty of level
    digitalWrite(sequence[i],LOW); //Turn off LED given by sequence
    delay(300); //Wait 30ms before showing next LED
  }
}

//Prompt user to input repetition of sequence using joystick
void getSeq() {
  int correct = 0; //Variable to flag if user input is correct or not

  //For loop to prompt user for each part of sequence matching with level they are on
  for (int i = 0; i < level; i++) {
    correct = 0;

    while (correct == 0) {
      
      //If  statements to determine which LED was selected with the joystick
      if (analogRead(inJoyX) == 0) { //If joystick is in LEFT position
        //Turn on LED corresponding to left position (GREEN)
        digitalWrite(6,HIGH);
        delay(700);
        correct = 1;
  
        //Save user input sequence
        your_sequence[i] = 6;
  
        //Check if sequence matches with Simon says sequence
        if (your_sequence[i] != sequence[i]) {
          //Run wrongSeq function and quit getSeq function
          wrongSeq();
          return;
        }
  
        //Turn OFF LED
        digitalWrite(6,LOW);
      }
      else if (analogRead(inJoyY) == 0) { //If joystick is in UP position
        //Turn on LED corresponding to up position (BLUE)
        digitalWrite(5,HIGH);
        delay(700);
        correct = 1;
  
        //Save user input sequence
        your_sequence[i] = 5;
  
        //Check if sequence matches with Simon says sequence
        if (your_sequence[i] != sequence[i]) {
          //Run wrongSeq function and quit getSeq function
          wrongSeq();
          return;
        }
  
        //Turn OFF LED
        digitalWrite(5,LOW);
      }
      else if (analogRead(inJoyY) == 1023) { //If joystick is in DOWN position
        //Turn on LED corresponding to down position (YELLOW)
        digitalWrite(4,HIGH);
        delay(700);
        correct = 1;
  
        //Save user input sequence
        your_sequence[i] = 4;
  
        //Check if sequence matches with Simon says sequence
        if (your_sequence[i] != sequence[i]) {
          //Run wrongSeq function and quit getSeq function
          wrongSeq();
          return;
        }
  
        //Turn OFF LED
        digitalWrite(4,LOW);
      }
      else if (analogRead(inJoyX) == 1023) { //If joystick is in RIGHT position
        //Turn on LED corresponding to down position (ORANGE)
        digitalWrite(3,HIGH);
        delay(700);
        correct = 1;
  
        //Save user input sequence
        your_sequence[i] = 3;
  
        //Check if sequence matches with Simon says sequence
        if (your_sequence[i] != sequence[i]) {
          //Run wrongSeq function and quit getSeq function
          wrongSeq();
          return;
        }
  
        //Turn OFF LED
        digitalWrite(3,LOW);
      }
    }
  }

  //Display correct sequence LED pattern
  rightSeq();
}
