#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t txValue = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

volatile bool sigOdd=false;
volatile int sigT=-1;
volatile int sigOddT=-1;
volatile int sigOddCount=0;

int sigOddT_=-1;
int sigOddCount_=0;

float v;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};


void setup() {
  Serial.begin(115200);
   Serial.println("sdffffffffff");

  // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                    CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

  pinMode(13, OUTPUT);
  pinMode(2, INPUT);    
  pinMode(4, OUTPUT);   
  pinMode(5, OUTPUT);   
  pinMode(5, HIGH);   
  digitalWrite(4, HIGH); 
  
  //Serial.begin(19200);

  attachInterrupt(digitalPinToInterrupt(2), sig, CHANGE);  
}

void loop() {
 if(sigOddCount==sigOddCount_) return;

  if(sigOddCount_!=0)
  {
    v=(float)(sigOddCount-sigOddCount_)/(sigOddT-sigOddT_)*1000;
    Serial.println(v);
  }

  sigOddCount_=sigOddCount;
  sigOddT_=sigOddT;


  Serial.println('--');


    if (deviceConnected) {
        txValue=(int)(v*1000);//*(uint32_t*)&v;
        pTxCharacteristic->setValue(txValue);
        pTxCharacteristic->notify();
       // txValue++;
   // delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

    delay(500);
}

void sig()
{
  Serial.println('a');
  int t=millis();
  if(t-sigT>1)
  {
    sigOdd=! sigOdd;
    if(sigOdd & 1)
    {
      sigOddCount++;
      sigOddT=t;
    }
  }
  sigT=t;
}
