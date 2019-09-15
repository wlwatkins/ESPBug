# ESPBug

ESPBug is a millicious captive portal program which runs on the ESP8266 dev board, such as the NodeMCU (clones included). It is a social engennering tool which generates a WiFi network of a given name alluring people to connect  to it and enter some credential.

# DISCLAIMER
The source code given in this public repo is for educational use only. Stealing credential might be illegal depending on where you live. I am not liable for any actions on your behalf. This code is given with no warranties and liabilities. **Just be responsible OK?**

# Demonstration & How tos

Here are a few screenshots from my phone showing off the web interface, and what each settings do.

![Imgur1](https://i.imgur.com/qYyqwnQl.jpg)


When you connect to the ESPBug, go to any domain name and to the **espportal** page (e.g. `http://espbug.com/espportal`)
You should be shown a pop up window asking you for the password. Defaults are `Username: **espbug**` and `Password: **password**`.
From there you are redirected to the home page.



![Imgur2](https://i.imgur.com/NgNZkYol.jpg)


# Installation
To get this code running on a nodeMCU like board - such as the one illustrated below - you need to install the [Arduino IDE] (! I have only tried on Linux, but this should work on any OS - virtual machine included).

![esp8266 image][esp8266]

```
Download and install
```

From there you need a couple installations clicking the upload button. First got `File -> Preferences`. At the bottom of the window, you should see `Additional Boards Manager URLS`. Click the little icon at the end at the end of that line. A new window should show up asking you to `Enter additional URLS, one for each row`. Add:
```
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```
and click `OK`.

Next, in library manager (`Sketch -> Include Library -> Manage Libraries`) search for
```
ArduinoJson
```
From Benoit Blanchon. **YOU MUST INSTALL VERSION `5.13.5`** not version 6.

nearly there

Now you only need to install the board. `Tools -> Board: "<SOME BOARD NAME>" -> Boards Manager`. In the search bar, type

```
esp8266
```
by **ESP8266 Community**. I installed version `2.5.5`.

And YOU ARE DONE! Now, you need to open the `espbug_public.ino` and setup the correct parameters for the board.

Here are **MY** setup parameters

```
Board: "NodeMCU 1.0 (ESP-12E Module)"
Upload Speed: "115200"
CPU Frequency: "160 MHz"
Flash Size: "4M (3M SPIFFS)"
Debug port: "Disabled"
Debug Level: "None"
IwIP Variant: "v2 Lower Memory"
VTables: "Flash"
Exceptions: "Disabled"
Erase Flash: "All Flash Contents"
SSL Support: "All SSL ciphers (most compatible)"
Port : "<USB PORT>"
```




# CREDITS where credit is due

Most of the code is strongly inspired by different repo from github, I just wanetd to mention them here.
* [ESPortalV2] - https://github.com/exploitagency/ESPortalV2
* [github-ESPortal] - https://github.com/exploitagency/github-ESPortal
* [esp8266_deauther] - https://github.com/spacehuhn/esp8266_deauther
* and maybe more which I'll add if I remember



### Todos

 - Finish README.md
 - Have FTP Working
 - Make more captive portal HTML pages

### Known bugs
- In white Hat mode, the explination page is closed automatically when using `8.8.8.8`. Don't know yet how this can be fixed

License
----

  GNU GENERAL PUBLIC LICENSE


**Free Software, Hell Yeah!**


   [ESPortalV2]: <https://github.com/exploitagency/ESPortalV2>
   [github-ESPortal]: <https://github.com/exploitagency/github-ESPortal>
   [esp8266_deauther]: <https://github.com/spacehuhn/esp8266_deauther>
   [esp8266]: https://rcl.lt/files/c59c2f4d86f239f67a86-128x128
   [Arduino IDE]: https://www.arduino.cc/en/main/software
