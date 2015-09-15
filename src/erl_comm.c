typedef unsigned char byte;

read_cmd(byte *buf)
{
  int len;

  // if we can't read 2 bytes we freak out
  if (read_exact(buf, 2) != 2)
    return(-1);
  // those 2 bytes tell us how long this message is.  We then read the message
  // This is the bit where I said the elixir side has it easy but the c code has
  // to be explicit.
  len = (buf[0] << 8) | buf[1];
  return read_exact(buf, len);
}

// when we write a message out, we write the 2 byte length indicator
// as well, so that the erlang program's knowledge that we're using
// 2 byte length indicators is satisfied.
write_cmd(byte *buf, int len)
{
  byte li;

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);

  li = len & 0xff;
  write_exact(&li, 1);

  return write_exact(buf, len);
}

// This is just a function to read a certain number of bytes into a buffer from
// standard input (0)
read_exact(byte *buf, int len)
{
  int i, got=0;

  do {
    if ((i = read(0, buf+got, len-got)) <= 0)
      return(i);
    got += i;
  } while (got<len);

  return(len);
}

// and this is a function to write a certain number of bytes from a buffer to
// standard output (1)
write_exact(byte *buf, int len)
{
  int i, wrote = 0;

  do {
    if ((i = write(1, buf+wrote, len-wrote)) <= 0)
      return (i);
    wrote += i;
  } while (wrote<len);

  return (len);
}
