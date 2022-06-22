# Anomaly-detecion-server

A server that finding a irregularity in flight pattern,using design pattern and object-oriented programming. 

# Instructions:

1.Download all files.

2.In the main - there is a 2 client as an example. each one of the, have a file that inside the cmake-bulid dirctory(input.txt)- that contion an example 

of data flight pattern.

3.The clien sent to the server all the data that he need to learn about a normal pattern flight - after that he asks the server to find irregularity in another data

that given,acoording the normal pattern.

The serverusing 2 different algorithms to find irregularity in the pattern:

A. SimpleAnomalyDetactor that finding the deviation of a single point from a regression line that contion all the normal point.

B. MinCIrcle that contion all the normal point inside it and find if there is a deviation from the circle.

In both cases - the server will declare alarm about the deviation.

4.In addtion - the server sent a menu that offer the client a faw different options about like th decide when the server should declare of a alarm.
