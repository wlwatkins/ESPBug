int stationConnected = 0;
int stationPhished = 0;
int ledState = 0;
// settings.json

int whiteHat;
char ssid[32];
char password[64];
int channel;
int hidden;

char local_IPstr[16];
char local_MAC[20];
char gatewaystr[16];
char subnetstr[16];

char update_username[32];
char update_password[64];

char ftp_username[32];
char ftp_password[64];
int ftpenabled;
int esportalenabled;

String total;
String used;
String freespace;

int numStationsConnected;
int numStationsHaveConnected;
int listofConnectStations[8];

char stationIP[50];
char stationMac[32];
int stationID = 0;

const char *whiteHatDefault= "0";
const char *esportalenabledDefault = "0";

const char *ssidDefault= "espbug";
const char *passwordDefault = "password";
const char *channelDefault= "11";
const char *hiddenDefault = "1";

const char *local_IPDefault = "192.168.1.1";
const char *local_MACDefault = "A4:CF:12:BF:5D:30";
const char *gatewayDefault = "192.168.1.1";
const char *subnetDefault = "255.255.255.0";

const char *update_usernameDefault = "espbug";
const char *update_passwordDefault = "password";

const char *ftp_usernameDefault = "espbug";
const char *ftp_passwordDefault = "password";
const char *ftpenabledDefault = "1";



const char *whiteHatAttack =  "0";
const char *ssidAttack = "espbug";
const char *passwordAttack = "password";
const char *channelAttack = "11";
const char *hiddenAttack = "1";
const char *local_IPAttack =  "192.168.1.1";
const char *local_MACAttack =  "A4:CF:12:BF:5D:30";
const char *gatewayAttack =  "192.168.1.1";
const char *subnetAttack =  "255.255.255.0";
const char *update_usernameAttack =  "espbug";
const char *update_passwordAttack =  "password";
const char *ftp_usernameAttack =  "espbug";
const char *ftp_passwordAttack =  "password";
const char *ftpenabledAttack =  "1";
const char *esportalenabledAttack =  "0";
