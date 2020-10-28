Step 1: build the open62541 from source following the tutorial (https://open62541.org/doc/current/tutorial_server_firststeps.html)
Step 2: run "gcc -std=c99 open62541.c myServer.c -lws2_32 -o myServer.exe" in cmd to compile the myServer.exe
Step 3: run "./myServer.exe <ip> <port>" to launch the opc ua server
Step 4: add the opc ua server from the UaExpert software with the opc ua addres, e.g., opc.tcp://xxxx.xxxx.xxxx.xxxx:12345/
Step 5: browse the namespace from the uaexpert and you will find some attributes created in the myServer.cpp file
