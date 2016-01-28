#define STRTOK_MAX_TOKEN_SIZE 20
#define STRTOK_MAX_BUFFER_SIZE 50

/**
 * Tokenise an array of chars, using a seperator
 * @param buffer pointer to buffer we're parsing
 * @param token pointer to buffer to hold the tokens as we find them
 * @param seperator the seperator used between tokens
 * @return true if there are still tokens left, false if we're done
 */
bool strtok(char *buffer, char *token, char *seperator)
{
  int pos = StringFind(buffer, seperator);
  char t_buff[STRTOK_MAX_BUFFER_SIZE];

  // Make sure we zero out the buffer and token
  memset(token, 0, STRTOK_MAX_TOKEN_SIZE);
  memset(&t_buff[0], 0, STRTOK_MAX_BUFFER_SIZE);

  // Looks like we found a seperator
  if (pos >= 0)
  {
    // Copy the first token into the token buffer, only if the token is
    // not an empty one
    if (pos > 0)
      memcpy(token, buffer, pos);
    // Now copy characters -after- the seperator into the temp buffer
    memcpy(&t_buff[0], buffer+(pos+1), strlen(buffer) - pos);
    // Zero out the real buffer
    memset(buffer, 0, strlen(buffer) + 1);
    // Copy the temp buffer, which now only contains everything after the previous
    // token into the buffer for the next round.
    memcpy(buffer, &t_buff[0], strlen(&t_buff[0]));
    return true;
  }
  // We found no seperator but the buffer still contains a string
  // This can happen when there is no trailing seperator
  else if(strlen(buffer) > 0)
  {
    // Copy the token into the token buffer
    memcpy(token, buffer, strlen(buffer));
    // Zero out the remainder of the buffer
    memset(buffer, 0, strlen(buffer) + 1);
    return true;
  }
  return false;
}

task main()
{
  // For output purposes only
  int counter = 0;

  // Buffer we'd like to parse
  char *buff = "$GPGLL,4027.027912,N,08704.857070,W,18432.00,A,D*7A";

  // Array and associated pointer to hold tokens
  char tokArr[STRTOK_MAX_TOKEN_SIZE];
  char *tokPtr = &tokArr;

  // The character used to seperate the tokens
  char *sep = ",";


  writeDebugStreamLine("buff: %s", buff);
  writeDebugStreamLine("sep: \"%s\"", sep);

  // Go through buff until we're done and print out all the tokens
  // as we find them
  while (true)
  {
    if (strtok(buff, tokPtr, sep))
      writeDebugStreamLine("tok[%d]: %s", counter++, tokPtr);
    else
      return;
    // This is to keep writeDebugStreamLine from tripping up when
    // there's too much output at once
    EndTimeSlice();
  }
}
