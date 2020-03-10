# Bluetooth Protocol

## Overview

* Wireless communication protocol
* 2.4-2.485 GHz [1]
* Constructs PANs
  * Personal area networks
* Uses frequency-hopping spread spectrum 
  * Transmitting radio signals by rapidly changing the carrier freauqncy
    among many distinc frequencies occupying a large spectal band [3]
	* Avoids interference
	* Enables code-division multiple access (CDMA)

## Bluetooth Protocol Stack

  * ![bluetooth_stack][bt_stack]
  * Link Manager
	* System that manages establishing the conneciton between devices
	  * Establishment, authentication, and configuration
  * HCI Interface
	* Provides a command interface for the controller and the link manager
	* Allows access to the hardware status and control registers
	  * Exchanges commands and data with the HCI firmware
  * Logical Link Control (LLC) and Adaption Protocol (L2CAP)
	* Used to multiplex multiple logical connections between two devices
	* Provides segmentation and reassembly of on-air packets
	* Basic Mode
	  * L2CAP provides packets with a payload of up to 64KB
		* 672 bytes is default MTU
		* 48 bytes is manadtory supported MTU
	* Retransmission and Flow Control Modes
	  * L2CAP can be configured for isochronous data or reliable data
		* Implmeneted through retransmissiona and CRC checks
	* Enhanced Retransmission Mode (ERTM)
	  * Improved version of the original retransmission mode
	  * Provides a reliable L2CAP channel
    * Streaming Mode (SM)
	  * No retransmission or flow control
	  * Provides an unreliable L2CAP channel
  * Service Discovery Protocol (SDP)
	* Allows a device to discover services offered by other devices
	  * And parameters of services
  * Radio Frequency Communication (RFCOMM)
	* Cable replacement protocol used for generating a virtual serial data stream
  * Bluetooth Network Encapsulation Protocol (BNEP)
	* Used for transferring another protocol stack's data via an L2CAP
      channel
    * Main purpose is the tranmission of IP packets in the PAN profile
  * Audio/Video Control Transport Protocol (AVCTP)
	* Used by the remote control profile to transfer AV/C commands over an
      L2CAP channel
	  * Music control buttons on headsets (etc//) use this protocol to
        control the music player
  * Audio/Video Distribution Transport Protocol (AVDTP)
	* Used by the advanced audio distribution (A2DP) profile to stream music
      to headsets over an L2CAP channel 
  * Telephony Control Protocol - Binary (TCS BIN)
	* Bit-oriented protocol that defines the call control signaling for the
      establishment of voice and data calls between bluetooth devies
	  

  
## IEEE 802.15.1
	
* Defines the IEEE standard for WPAN and Bluetooth
  * WPAN = Wireless Personal Area Networks
* Defines a physical layer (PHY) and a media access control (MAC)
  specification
  * For wireless connectivity with fixed, portable, and moving devices
    within or entering personal operating space [2]
* Standards issued in 2002 and 2005


## Used Protocols

* ### Frequency-hopping spread spectrum

* ### Code-division multiple access (CDMA)

	* Spread the bandwidth of the data uniformly for the same transmitted
	  power
	* A locally generated code runs at a much higher rate than the data
	  to be transmitted
	* Data for transmisison is combined by XOR with the faster code
	  * ![signal generation][bt_signal]
	  * Tc = Period of the chip
		* 1/Tc = Spread spectrum bandwidth
	  * Tb = Period of the data
		* 1/Tb = Bandwidth of the data signal
	  * Tb / Tc = spreading factor (upper limit of total number of
		supported users
	* The separation of signals is made by correlating the received signal
      with the locally generated code of the desired user
	  * If the signal matches, correlation function will be high, otherwise
        should be close to zero and eliminate the signal
	* Analogy: "An analogy ... is a room (channel) in which people wish to
      talk to each other simulteaneously. To avoid confusion they could
      take turns speaking (time division), speak at different pitches
      (frequency division), or speak in different languages (code
      division)."
    * Code unique to each user is used to extract data
	  * [CDMA Example][cdma_example]
  

* ### Ambiguity Function 

## References

[1] https://en.wikipedia.org/wiki/Bluetooth

[2]
https://en.wikipedia.org/wiki/IEEE_802.15#IEEE_802.15.1:_WPAN_/_Bluetooth

[3] https://en.wikipedia.org/wiki/Frequency-hopping_spread_spectrum


[bt_stack]: https://upload.wikimedia.org/wikipedia/commons/thumb/9/9f/Bluetooth_protokoly.svg/1024px-Bluetooth_protokoly.svg.png "Bluetooth Stack"
[bt_signal]: https://upload.wikimedia.org/wikipedia/commons/thumb/7/7e/Generation_of_CDMA.svg/500px-Generation_of_CDMA.svg.png "CDMA Signal Generation Diagram"
[cdma_example]:https://en.wikipedia.org/wiki/Code-division_multiple_access#Example "CDMA Example"
