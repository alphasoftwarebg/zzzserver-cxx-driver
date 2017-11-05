# zzzserver-cxx-driver
C driver for **ZZZ Base** through ZZZ Server

***ZZZClient.h*** - the header file with driver functions declarations  
***ZZZClient.cpp*** - the file with driver functions implementations  
***main.cpp*** - sample access to **ZZZ Base** through function "zzzclient_zzzprogram"  

**sample usage:**
```c_cpp
ZZZClient client;

cout << client.ZZZProgram("localhost", 3333, "#[cout;Hello World from ZZZServer!]") << endl;
```

Returned result encoded in the utf-8
