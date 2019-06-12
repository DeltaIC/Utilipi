int Transistors_Tab[] = {PIN_T1,PIN_T2,PIN_T3,PIN_T4,PIN_T5,PIN_T6,PIN_T7,PIN_T8};

void setup(){
    
  for (int i=0; i<=sizeof(Transistors_Tab); i++){
    pinMode(Transistors_Tab[i], OUTPUT);
    digitalWrite(Transistors_Tab[i], LOW);
  }

}

void loop(){
  delay(100000);
}
