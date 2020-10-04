#include "osapi.h"
#include "user_interface.h"
#include  "ets_sys.h"
#include "espconn.h"
#include "c_types.h"
#include "limits.h"
#include "mem.h"
//#include <ArduinoJson.h>
bool debug=true;
int count=0;
char*sendData="GET /data/2.5/weather?q=ankara&appid=3d3126f4f655e902fcaf955f71519286\r\nHost: api.openweathermap.org:80\r\nUser-Agent: ESP8266\r\nConnection: close\r\n\r\n";//    
void ICACHE_FLASH_ATTR wificonf()
{
wifi_set_opmode(0x01);
char	ssid[32]	=	"xx";
char	password[64]	=	"12345678";	
struct	station_config	stationConf;	
stationConf.bssid_set	=	0;	
os_memcpy(&stationConf.ssid,ssid,32);
os_memcpy(&stationConf.password,password,64);

wifi_station_set_config(&stationConf);
wifi_station_disconnect();
wifi_station_connect();
}
static void ICACHE_FLASH_ATTR shell_regist_disconcb(void * arg)
{
struct espconn * conn;
}



static void ICACHE_FLASH_ATTR shell_regist_reconcb(void * arg,sint8	err)
{	
}

static void ICACHE_FLASH_ATTR shell_regist_recvcb(void * arg,char*pdata, unsigned	short	len)
{
	struct espconn * conn=(struct espconn *)arg ; 
	count++;
//DynamicJsonBuffer jsonBuffer(strlen(pdata));
//JsonObject& root = jsonBuffer.parseObject(pdata);
/*if(root.success())
{
 const char*temp=root["main"]["temp"];
 int temperature =atoi(temp);
 char buf[50];
 const char* city = root["name"];
 const char*country=root["sys"]["country"];
 os_sprintf(buf,"%i-%i'C %s,%s",count,temperature-273,city,country);
 Serial.println(buf);
}*/
Serial.println(pdata);
}






static void ICACHE_FLASH_ATTR shell_regist_sentcb(void * arg)
{
	struct espconn * conn = (struct espconn *)arg;
	espconn_regist_recvcb(conn,shell_regist_recvcb);

	
	
	
}


static void ICACHE_FLASH_ATTR shell_regist_connectcb(void * arg)
{
struct espconn * conn = (struct espconn *)arg;
espconn_regist_sentcb(conn,shell_regist_sentcb);

espconn_send(conn,(uint8_t *)sendData,strlen(sendData));
}










void ICACHE_FLASH_ATTR dns_callback(const char *name,ip_addr_t *addr,	void*arg)
{
struct espconn * conn = (struct espconn *)arg;
conn->type=ESPCONN_TCP;
conn->state=ESPCONN_NONE;
conn->proto.tcp = (esp_tcp *)os_malloc(sizeof(esp_tcp));

conn->proto.tcp->local_port =espconn_port() ;
conn->proto.tcp->remote_port = 80;
os_memcpy(conn->proto.tcp->remote_ip, addr, 4);
espconn_regist_connectcb(conn,shell_regist_connectcb);
espconn_regist_reconcb(conn,shell_regist_reconcb);
espconn_regist_disconcb(conn,shell_regist_disconcb);
espconn_connect(conn);//espconn_connect
}







void ICACHE_FLASH_ATTR tcpClient()
{

struct espconn *conn=(struct espconn *)os_malloc(sizeof(struct espconn));
ip_addr_t addr;
err_t error;
error=espconn_gethostbyname(conn,"api.openweathermap.org", &addr, dns_callback);
if(error==ESPCONN_INPROGRESS)
{}
else if(error==ESPCONN_OK)
{
dns_callback("api.openweathermap.org",&addr,conn);
}
else if(error==ESPCONN_ARG)
{
}
else
{	
}

}








void setup()
{
Serial.begin(9600);
wificonf();
tcpClient();
}


void loop()
{
delay(500);
}
