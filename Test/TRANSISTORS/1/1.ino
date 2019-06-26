#define TRANSISTOR_ON   0
#define TRANSISTOR_OFF  1

int Transistors_Tab[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4,PIN_T5,PIN_T6,PIN_T7,PIN_T8};

void setup(){

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //enable_reset();
  //disable_nfc();
  
  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    pinMode(Transistors_Tab[i], OUTPUT);
    digitalWrite(Transistors_Tab[i], HIGH);
  }

  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop(){

  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    digitalWrite(Transistors_Tab[i], TRANSISTOR_ON);
  }
  delay(2000);

  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    digitalWrite(Transistors_Tab[i], TRANSISTOR_OFF);
  }
  delay(2000);
    
}
