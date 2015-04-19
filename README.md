# Digital-broadcasting


Real-time Transfer Protocol
Problem 1:  Download the source codes attached. Build sender and receiver. To build use the following commands in the terminal:
gcc receiver.c rtp.c -o receiver
gcc sender.c rtp.c -o sender

Select an image file of your choice, and execute the receiver as follows
 ./receiver 12345 > image_rcv.jpg
 then in a new terminal window execute 
 ./sender 127.0.0.1 12345 < image.jpg 
 where 12345 is a port of a receiver and image.jpg is your image file.
 The symbols < and > are used to redirect standard output and input.  By typing sender < image.jpg we redirect input from a file, instead of a keyboard and by typing receiver > image_rcv.jpg  we redirect output to  a file instead of a screen.

Problem 2:  Study source codes very carefully and add detailed comments for as many statements as you think is necessary, keeping in mind that the more the better. The goal of this problem is to understand the codes in depth.  

