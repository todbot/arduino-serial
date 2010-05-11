#!/bin/sh


java -Djava.library.path=rxtxlib -classpath .:rxtxlib/RXTXcomm.jar $*
#jamvm -Djava.library.path=rxtxlib -classpath .:rxtxlib/RXTXcomm.jar:/opt/local/share/java/glibj.jar $*

