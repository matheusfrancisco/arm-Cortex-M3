void setup() {
  int vet[40],v;
  Serial.begin(115200);
  pinMode(8, OUTPUT);
  digitalWrite(8,LOW);
  delay(20);
  pinMode(8, INPUT);
  delayMicroseconds(120);
  while (digitalRead(8)==HIGH);

  while (digitalRead(8)==LOW);

    for (int x=0;x<40;x++)
    {
      delayMicroseconds(35);
      v = digitalRead(8);
      if (v==LOW) vet[x] = 0;
      else vet[x] = 1;
      
      if (v==HIGH) while (digitalRead(8)==HIGH);
      while (digitalRead(8)==LOW);
    }

    for (int x=0;x <8;x++) 
    Serial.print(vet[x]);

    Serial.println("");

    for (int x=0;x <8;x++) 
    Serial.print(vet[x+8]);

Serial.println("");
 for (int x=0;x <8;x++) 
    Serial.print(vet[x+16]);
Serial.println("");

 for (int x=0;x <8;x++) 
    Serial.print(vet[x+24]);
 
 Serial.println("");
 analogReference(INTERNAL);
 int d = analogRead(0);
 Serial.print("Temperatura lm35  ");
 float f = d * (110.0)/1023;
 Serial.println(f); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
