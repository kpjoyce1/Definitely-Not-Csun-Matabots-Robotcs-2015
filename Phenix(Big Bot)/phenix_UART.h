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
char xValue[5];
char yValue[5];

bool NoBall = true;

typedef struct {

	int x;
	int y;
	char sig;
	bool updated;

} Ball;


Ball closestBall, temp;
Ball Balls[10];

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

		int closest = 0;
		for(int i = 0; i < 10; i++)
		{
			if(Balls[closest].x > Balls[i].y && Balls[i].updated)
			{
				closest = i;
			}
		}

		if(Balls[closest].updated && Balls[closest].sig != 'N')
		{
			closestBall.x = Balls[closest].x;
			closestBall.y = Balls[closest].y;
			closestBall.sig = Balls[closest].sig;
			closestBall.updated = Balls[closest].updated;
		}
		else if(NoBall)
		{
			closestBall.sig = 'N';
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
		temp.sig = rcvChars[messageStart+1];
		temp.x = atoi(xValue);
		temp.y = atoi(yValue);
		int untranslatedX = bigBot.X + cosDegrees(bigBot.Theta)*temp.y - sin(bigBot.Theta)*temp.x;
		int untranslatedY = bigBot.Y + sinDegrees(bigBot.Theta)*temp.y - cosDegrees(bigBot.Theta)*temp.x;
		temp.x = untranslatedX;
		temp.y = untranslatedY;
		temp.updated = true;


		for(int i = 0; i < 10; i++)
		{
			if(temp.sig == 'N')
			{
				Balls[i].x = 0;
				Balls[i].y = 0;
				Balls[i].sig = 'N';
				Balls[i].updated = false;
				NoBall = true;
			}
			else if(abs(temp.x - Balls[i].x) < 20 && abs(temp.y - Balls[i].y) < 20 && temp.sig == Balls[i].sig) //Same ball
			{
				Balls[i].x = temp.x;
				Balls[i].y = temp.y;
				Balls[i].sig = temp.sig;
				Balls[i].updated = true;
				NoBall = false;
				break;
			}
			else if(!Balls[i].updated)//Existing ball Check
			{
				Balls[i].x = temp.x;
				Balls[i].y = temp.y;
				Balls[i].sig = temp.sig;
				Balls[i].updated = true;
				NoBall = false;
				break;
			}
		}
	}
}

void ClearBalls()
{
	for(int i = 0; i < 10; i++)
	{
		Balls[i].x = 0;
		Balls[i].y = 0;
		Balls[i].sig = 'N';
		Balls[i].updated = false;
	}

}

void configureSerial()
{
	configureSerialPort(uartTwo, uartUserControl);
	setBaudRate(uartTwo, baudRate19200);
	startTask(UARTReceive);
}
