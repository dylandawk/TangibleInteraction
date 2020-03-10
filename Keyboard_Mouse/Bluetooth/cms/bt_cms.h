#ifndef BT_COMMS_H
#define BT_COMMS_H

#include <ArduinoBLE.h>
//ID for the communication communication service
#define FLSID       "19b10000-e8f2-537e-4f6c-d104768a1214"
#define FLSTRBUFLEN 12
#define ONSTR       "TURN ON    \0"
#define ACKSTR      "ACK        \0"

char STRBUF[FLSTRBUFLEN];



#endif
