const int in1 = 3;
const int in2 = 5;
const int in3 = 13;
const int in4 = 11;
String incoming;
int incoming_int;
int mapped;

unsigned char dta[100];
unsigned char len = 0;
unsigned int t1;
unsigned int t2;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  //Serial.println("Give a value between -1000 and 1000");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
}

void loop() {
  while(Serial1.available()>=9)
    {
        if((0x59 == Serial1.read()) && (0x59 == Serial1.read())) //Byte1 & Byte2
        {
            t1 = Serial1.read(); //Byte3
            t2 = Serial1.read(); //Byte4

            t2 <<= 8;
            t2 += t1;
            Serial.println(t2);
            //Serial.print('\t');

            t1 = Serial1.read(); //Byte5
            t2 = Serial1.read(); //Byte6

            t2 <<= 8;
            t2 += t1;
            //Serial.println(t2);

            for(int i=0; i<3; i++) 
            { 
                Serial1.read(); ////Byte7,8,9
            }
        }
    }
    
  //if (Serial.available() > 0){
    incoming = t2;
    Serial.print("I recived: ");
    Serial.println(incoming);
    incoming_int = incoming.toInt();
    Serial.print("as an int: ");
    Serial.println(incoming_int);
    if (incoming_int > 0){
      mapped = map(incoming_int, 0, 1000, 0, 255);
      analogWrite(in1, mapped);
      digitalWrite(in2, LOW);
      
    }else if (incoming_int < 0 ){
      mapped = map(incoming_int, 0, -1000, 0, 255);
      digitalWrite(in1, LOW);
      analogWrite(in2, mapped);
    }else{
      digitalWrite(in1, HIGH);
      digitalWrite(in2, HIGH);
    }

    
  //}

}
