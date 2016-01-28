/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VEX Cortex UART Test
//
// Simple program to test user control over the VEX Cortex UART ports.
//
// 1. The two ports are looped back-to-back.
//
// 2. One port simply outputs characters with hex values 0, 1, 2, ... 255, 0, 1, 2, ...
//
// 3. The "UARTReceive" task grabs the characters from the receive UART and verifies that they are
// in the above sequence.
//
// Various peg counts are used to accumulate total characters transmitted, received and "out of
// sequence" counts. These can be viewed in the ROBOTC debugger.
//
// NOTE: Internally the firmware uses interrupt handlers to buffer the characters transmitted and
// received. The buffers are about 25 characters in length. If user program does not extract
// the characters fast enough the older characters are simply discarded.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task UARTReceive();
