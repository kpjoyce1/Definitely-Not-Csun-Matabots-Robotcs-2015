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
const int messageSize = 22;
const int maxMessageLength = 11;
char rcvChars[messageSize]; // Keep buffer of last 23 characters received.
int messageStart = -1;
char xValue[4];
char yValue[4];



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
		++nLastRcvChar;
		nLastRcvChar %= 256;

		nLastRcvChar = rcvChar;
		rcvChars[nRcvIndex] = rcvChar;
		++nRcvIndex;
		if (nRcvIndex >= sizeof(rcvChars))
		{
			nRcvIndex = 0;

			Parse();
		}
	}
}




void Parse()
{
	messageStart = -1;
	bool messageExists = false;

	for(int i = 0; i < messageSize; i++)
	{

		if(rcvChars[i] == 0x5D && i > messageStart && messageStart != -1)
		{
			messageExists = true;
			break;
		}
		else if(rcvChars[i] == 0x5B)
		{
			messageStart = i;
		}

	}

	if(messageStart + maxMessageLength > messageSize)
	{
		messageExists = false;
	}

	if(messageExists)
	{
		bool xRegion = true;
		bool yRegion = false;

		for(int i = 0; i < 3; i++)
		{
			xValue[i] = '\0';
			yValue[i] = '\0';
		}

		int indexX = 0;
		int indexY = 0;

		for(int currentIndex = messageStart + 3; currentIndex < messageSize; currentIndex++)
		{


			if(rcvChars[currentIndex] == '_')
			{
				xRegion = false;
				yRegion = true;
			}
			else if(rcvChars[currentIndex] == ']')
			{
				break;
			}

			if(xRegion && rcvChars[currentIndex] >= 0x20 && rcvChars[currentIndex] <= 0x39)
			{
				char tempX = rcvChars[currentIndex];
				xValue[indexX] = tempX;
				indexX += 1;
			}
			else if(yRegion && rcvChars[currentIndex] >= 0x20 && rcvChars[currentIndex] <= 0x39)
			{
				char tempY = rcvChars[currentIndex];
				yValue[indexY] = tempY;
				indexY += 1;
			}

		}

		closestBall.sig = rcvChars[messageStart+1];
		//if(xValue[2] == '\0')
		//{
			closestBall.x = atoi(xValue);
		//}
		//else
		//{
		//	closestBall.x = atoi(xValue);
		//	closestBall.x /= 1000;
		//}
		closestBall.y = atoi(yValue);
		closestBall.updated = true;
	}

}

void configureSerial()
{
	configureSerialPort(uartTwo, uartUserControl);
	setBaudRate(uartTwo, baudRate19200);
	startTask(UARTReceive);
}
