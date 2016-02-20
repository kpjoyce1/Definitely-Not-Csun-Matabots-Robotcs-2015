#pragma config(UART_Usage, UART1, uartUserControl, baudRate115200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartUserControl, baudRate115200, IOPins, None, None)
#pragma config(Sensor, dgtl1,  butt,           sensorTouch)
#pragma config(Sensor, dgtl2,  butt2,          sensorTouch)
#pragma config(Sensor, I2C_1,  anything,       sensorSpare2OnI2CPort,         , 66)
#pragma config(Motor,  port10,          testMotor,     tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task UARTReceive();
int nRcvIndex = 0;
long nTotalRcvChars = 0;
long nTotalRcvOutOfSequence = 0;
signed char inChar;
int rcvChar;
signed char rcvChars[7];
signed char endOfString = '$';
int i;
int j;

// Keep buffer of last 23 characters received.
task main()
{
	// Setup the two UART ports
	configureSerialPort(UART1, uartUserControl);
	configureSerialPort(UART2, uartUserControl);
	setBaudRate(UART1, baudRate115200);
	setBaudRate(UART2, baudRate115200);
	while (getChar(UART2) != -1) // Purge existing chars from buffer
	{}

	startTask(UARTReceive);

	while (true)
	{

		//if (rcvChars[0] == 'H')
		//	motor[testMotor] = 127;
		//else if (rcvChars[0] == 'W')
		//	motor[testMotor] = 0;
		//else
		//	motor[testMotor] = 0;
		if(SensorValue(butt) == 0)
			i = 0;
		if(SensorValue(butt2) == 0)
			j = 0;

		if (SensorValue(butt) == 1 && i == 0)
		{
			inChar = '%';
			sendChar(UART2, inChar);
			i = 1;
		}

		if (SensorValue(butt2) == 1 && j == 0)
		{
			inChar = '&';
			sendChar(UART2, inChar);
			j = 1;
		}

		while(!bXmitComplete(UART2))
			{
				wait1Msec(50);
			}
	}
}

task UARTReceive()
	{

	while (true)
		{
			// Loop forever getting characters from the "receive" UART. Validate that they arrive in the expected
			// sequence.
			static int nLastRcvChar = 0;

			rcvChar = getChar(UART2);
			if (rcvChar == -1)
			{
				// No character available
				wait1Msec(3);	// Don't want to consume too much CPU time. Waiting eliminates CPU consumption for this task.
				continue;
			}

			if(rcvChar == endOfString)
			{
				rcvChars[nRcvIndex] = endOfString;
				nRcvIndex = 0;
			}
			else
			{
				++nLastRcvChar;
				nLastRcvChar %= 256;
				if (nLastRcvChar != rcvChar)
					++nTotalRcvOutOfSequence;

				nLastRcvChar = rcvChar;
				rcvChars[nRcvIndex] = rcvChar;



				++nTotalRcvChars;
				++nRcvIndex;
				if (nRcvIndex >= sizeof(rcvChars))
				nRcvIndex = 0;
		  }//end of else

		}
}