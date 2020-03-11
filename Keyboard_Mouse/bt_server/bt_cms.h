#ifndef BT_COMMS_H
#define BT_COMMS_H

#include <ArduinoBLE.h>
//ID for the communication communication service
#define FLSID       "19b10000-e8f2-537e-4f6c-d104768a1214"

//Flashlight Trasmissin buffer length
#define FLTXBUFLEN  12

//Value array length
#define VALARRLEN   6
int16_t vals[VALARRLEN];
typedef enum{
	     AX,
	     AY,
	     AZ,
	     GX,
	     GY,
	     GZ,
	     VALEND
}sensVals_e;



#endif
