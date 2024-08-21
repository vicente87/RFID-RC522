#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN	9    //Pin 9 para el reset del RC522
#define SS_PIN	10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522


byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x24 ,0x31, 0x39, 0xA3} ; //código del llavero
byte Usuario2[4]= {0xEF, 0x8C, 0xC4, 0x1E} ; //código de la tarjeta
byte Usuario3[4]= {0x14, 0xDF, 0x77, 0x2C} ; //código de la pulsera
void setup() {
	Serial.begin(9600); //Iniciamos la comunicación  serial
	SPI.begin();        //Iniciamos el Bus SPI
	mfrc522.PCD_Init(); // Iniciamos  el MFRC522
	Serial.println("Control de Acceso");
}

void loop() {
	// Revisamos si hay nuevas tarjetas  presentes
	if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
  		//Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                  Serial.print("Card UID:");
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX); 
                          ActualUID[i]=mfrc522.uid.uidByte[i];  
                  } 
                  Serial.println();
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1))
                    Serial.println("Acceso concedido a llavero...");
                  else if(compareArray(ActualUID,Usuario2))
                    Serial.println("Acceso concedido a tarjeta...");
                  else if (compareArray(ActualUID,Usuario3))
                    Serial.println("Acceso concedido a pulsera...");
                  else
                   Serial.println("Acceso denegado ...");
                  // Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();         
            }      
	}	
}

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}