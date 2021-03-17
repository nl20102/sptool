# Quick start

sptool - a tool designed to send and receive data via a serial port. It can be used for testing when developing applications that work with a serial port.

## Getting help

Use command **sptool --help** to get help.

## Sending data to the serial port

For example, you can send two blocks of data with a delay of 500 ms to a port COM1 using the following command:

**sptool --port COM1 --data "some data to send" --data "more data to send" --delay 500**

This command first sends the first block of data, which is text "some data to send", then, after delay of 500 ms, sends the second, which is text "more data to send".

Note, that the end-of-line character '\n' is added to each data blocks at the end. It indicates the end of the data block. Quotation marks are only needed if the data block consists of several words separated by a space. If the data block is single word, no quotation marks are needed.

If you need to send the same data block 5 times with a 300 ms pause in between, use the following command:

**sptool --port COM1 --data "some data to send" --delay 300 --repeat 5**

To send a single data block which is text "123" to port COM3, use the following command:

**sptool --port COM3 --data 123**

## Receiving data from the serial port

To read any block of data from the port COM5 use the following command:

**sptool --port COM5 --answer ***

The sptool will be waiting for data from the specified port. As soon as the data block is received, it will be displayed on the screen.

Note, a data block is a text that ends with an end of line character '\n'.

To read specific data, 'some response' for example, use:

**sptool --port COM5 --answer "some response"**

The received data will be compared with the specified text. The result of the comparison will be displayed on the screen.

You can also use --delay and --repeat arguments, to specify delays and repetitions.

## Mixed data sending and receiving

If you need to send data first, "123" for example, and then get a response, "ok" for example, you can use the following command:

**sptool --port COM1 --data 123 --answer ok**

You can also use --delay and --repeat arguments, to specify delays and repetitions.