/* Copyright (c) 2016 Max Asaulov balldir@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

/**
 * Prints usage
 */
static void print_help()
{
  printf("USAGE: . /dev/tty DTR/RTS SET/CLEAR/0/1\n");
}

static int extract_flags(int argc, char **argv, int *flag)
{
  assert(flag);
  assert(argv);
  assert(argv[2]);
  assert(argc >= 2);
  *flag = 0;
  int rc = 0;
  if (strncmp(argv[2],"DTR", sizeof("DTR") ) == 0 ) {
    *flag = TIOCM_DTR;
  }
  else if (strncmp(argv[2],"RTS", sizeof("RTS") ) == 0)
  {
    *flag = TIOCM_RTS;
  }
  else
  {
    rc = EINVAL;
  }
  return rc;
}

static int extract_operation(int argc, char **argv, unsigned long *operation)
{
  assert(operation);
  assert(argv);
  assert(argv[3]);
  assert(argc >= 3);
  *operation = 0;
  int rc = 0;
  if (strncmp(argv[3], "SET", sizeof("SET")) == 0 ||
    strncmp(argv[3], "1", sizeof("1")) == 0)
  {
      *operation = TIOCMBIS;
  }
  else if (strncmp(argv[3], "CLEAR", sizeof("CLEAR")) == 0 ||
      strncmp(argv[3], "0", sizeof("0")) == 0)
  {
      *operation = TIOCMBIC;
  }
  else {
    rc = EINVAL;
  }
  return rc;
}

int main(int argc, char **argv)
{
  int rc = 0;
  if (argc !=  4) {
    rc = EINVAL;
    print_help();
    goto exit;
  }

  int flag = 0;
  rc = extract_flags(argc, argv, &flag);
  if (rc != 0) {
    printf("Wrong line\n");
    print_help();
    goto exit;
  }

  /* Parse arguments to select the operation */
  unsigned long operation = 0;
  rc = extract_operation(argc, argv, &operation);
  if (rc != 0) {
    rc = EINVAL;
    printf("Wrong state\n");
    print_help();
    goto exit;
  }

  /* Try to open the file */
  int fd = open(argv[1],O_RDWR | O_NOCTTY );
  if (fd == -1) {
    printf("Error opening a %s : %s\n", argv[1], strerror(errno));
    rc = errno;
    goto exit_open_failed;
  }

  /* Perform operation */
  if (ioctl(fd, operation, &flag) != 0) {
    printf("Error during %s of %s line : %s\n", argv[3], argv[2] , strerror(errno) );
    rc = errno;
  }

exit_open_failed:
  close(fd);
exit:
  return rc;
}
