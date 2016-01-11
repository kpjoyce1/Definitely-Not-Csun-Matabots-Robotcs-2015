/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VEX Cortex UART
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////



task UARTReceive();
void Parse();
int nRcvIndex = 0;
int rcvChar;
const int messageSize = 23;
unsigned char rcvChars[messageSize]; // Keep buffer of last 23 characters received.


typedef struct {

	int x;
	int y;
	char sig;
	bool updated;

} Ball;


Ball closestBall, targetBall;

// // Setup the UART ports
// configureSerialPort(uartTwo, uartUserControl);
// setBaudRate(uartTwo, baudRate115200);
// startTask(UARTReceive);

task UARTReceive()
{
 while (true)
 {
 	// Loop forever getting characters from the "receive" UART. Validate that they arrive in the expected
 	// sequence.

 	static int nLastRcvChar = 0;

 	rcvChar = getChar(uartTwo);
 	if (rcvChar == -1)
 	{
 		// No character available

 		wait1Msec(3); // Don't want to consume too much CPU time. Waiting eliminates CPU consumption for this task.
 		continue;
 	}
 	else if(rcvChar == 0x0)
 	{
   	for(int i = 0; i < messageSize; i++)
   	{
  		memset(&rcvChars[i], 0, sizeof(rcvChars));
 	 	}
 	}

 	nLastRcvChar = rcvChar;
 	rcvChars[nRcvIndex] = rcvChar;
 	++nRcvIndex;
 	if (nRcvIndex >= sizeof(rcvChars))
 	nRcvIndex = 0;

	Parse();
 	}
}



void Parse()
{
		int messageStart = -1;
		bool messageExists = false;

		for(int i = 0; i < messageSize; i++)
		{
			if(rcvChars[i] == 0x5B)
			{
				messageStart = i;
			}

			if(rcvChars[i] == 0x5D && i > messageStart && i != -1)
			{
				messageExists = true;
		  }
	  }


	  if(messageExists)
	  {
	  	char xValue[], yValue[];

	  	bool xRegion, yRegion;
	  	for(int i = messageStart; i < messageSize; i++)
	  	{
	  			if(rcvChars[i] == 0x5F && !xRegion)
	  			{
	  					xRegion = true;
	  			}
	  			else if(rcvChars[i] == 0x5F)
	  			{
	  					yRegion = true;
	  			}


	  			if(yRegion)
	  			{
	  					yValue[0] = rcvChar[i];
	  			}
	  			else if(xRegion)
	  			{
	  					xValue[0] = rcvChar[i];
	  			}

	  	}
	  	closestBall.sig = rcvChars[messageStart+1];
	  	closestBall.x = atoi(xValue);
	  	closestBall.y = atoi(yValue);
	  	closestBall.updated = true;
		}

}

void configureSerial()
{
	configureSerialPort(uartTwo, uartUserControl);
  setBaudRate(uartTwo, baudRate115200);
	startTask(UARTReceive);
}
