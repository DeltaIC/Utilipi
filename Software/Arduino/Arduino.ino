int Transistors_Tab[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4};
int Input_Tab[] = { PIN_INPUT_1,PIN_INPUT_2,PIN_INPUT_3,PIN_INPUT_4,
                    PIN_INPUT_5,PIN_INPUT_6,PIN_INPUT_7,PIN_INPUT_8,
                    PIN_INPUT_9,PIN_INPUT_10,PIN_INPUT_11,PIN_INPUT_12};

void setup(){

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //enable_reset();
  //disable_nfc();
  
  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    pinMode(Transistors_Tab[i], OUTPUT);
    digitalWrite(Transistors_Tab[i], LOW);
  }

  for (int i=0; i<=sizeof(Input_Tab); i++){
    pinMode(Input_Tab[i], INPUT);
  }

  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop(){

  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    digitalWrite(Transistors_Tab[i], HIGH);
  }
  digitalWrite(LED_BUILTIN, digitalRead(Input_Tab[0]));
  delay(1000);

  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    digitalWrite(Transistors_Tab[i], LOW);
  }
  digitalWrite(LED_BUILTIN, digitalRead(Input_Tab[0]));
  delay(1000);
    
}
