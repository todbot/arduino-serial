#!/bin/sh

# pick one
JAVAC=javac
#JAVAC=/System/Library/Frameworks/JavaVM.framework/Versions/1.4.2/Commands/javac
#JAVAC=/System/Library/Frameworks/JavaVM.framework/Versions/1.5/Commands/javac


$JAVAC -classpath .:rxtxlib/RXTXcomm.jar *.java 
#javac -Xlint:unchecked  -classpath .:rxtxlib/RXTXcomm.jar *.java
