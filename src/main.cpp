#include "secrets.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DHT.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <cstdio>
#include <ctime>

// Configuracion del tiempo para CDMX
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC (-25200)
#define DAYLIGHT_OFFSET_SEC (-25200)

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Configuración del sensor de temperatura y humedad
#define DHTPIN 4

const char *wifi_ssid = WIFI_SSID;
const char *wifi_pwd = WIFI_PWD;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long displayUpdateTime = 0;
int count = 0;
bool signupOK = false;

char time_str_buffer[80];

// Path of the device on Firebase
const char *device_path = DEVICE_PATH;

void get_current_date(char *buffer, int size);

void firebase_login();

// =============================================================================
DHT dht(DHTPIN, DHT11);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // NOLINT(*-interfaces-global-init)

void setup()
{
	Serial.begin(115200);
	WiFi.begin(wifi_ssid, wifi_pwd);
	pinMode(LED_BUILTIN, OUTPUT);

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{
		Serial.println("Canno't initialize display");
	}

	display.display();
	delay(100);
	display.clearDisplay();

	Serial.printf("Connecting to %s", wifi_ssid);
	bool intermitent = false;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		digitalWrite(LED_BUILTIN, (intermitent = !intermitent) ? HIGH : LOW);
		delay(100);
	}

	digitalWrite(LED_BUILTIN, HIGH);
	Serial.print("Connected!\nIP Address: ");
	Serial.print(WiFi.localIP());
	Serial.print("\nHostname: ");
	Serial.print(WiFi.getHostname());

	Serial.println("\nSyncing time...");
	configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
	get_current_date(time_str_buffer, 80);

	Serial.println("\nConnecting to Firebase");

	config.api_key = API_KEY;
	config.database_url = DATABASE_URL;
	auth.user.email = AUTH_EMAIL;
	auth.user.password = AUTH_PASSWORD;

	firebase_login();

	Firebase.begin(&config, &auth);
	Firebase.reconnectWiFi(true);
}

void loop()
{
	float temperature = dht.readTemperature();
	float humidity = dht.readHumidity();

	if (millis() - displayUpdateTime > 1000 || displayUpdateTime == 0)
	{
		displayUpdateTime = millis();
		get_current_date(time_str_buffer, 80);
		
		display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0, 0);
		display.println(time_str_buffer);
		display.setCursor(0, 10);
		display.printf("Temprerature: %.2f", temperature);
		display.setCursor(0, 20);
		display.printf("Humidity: %.2f", humidity);
		display.setCursor(0, 30);
		display.print("IP: ");
		display.print(WiFi.localIP());
		display.display();
	}

	if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 30000 || sendDataPrevMillis == 10000))
	{
		sendDataPrevMillis = millis();
		get_current_date(time_str_buffer, 80);

		if (isnan(temperature) || isnan(humidity))
		{
			Serial.println("Error reading temperature");
			return;
		}

		Serial.println(temperature);
		Serial.println(humidity);

		FirebaseJson json;
		json.set("fields/date/timestampValue", time_str_buffer);
		json.set("fields/temperature/doubleValue", temperature);
		json.set("fields/humidity/doubleValue", humidity);

		String measuresPath;
		measuresPath.concat(device_path);
		measuresPath.concat("/measures");

		Serial.printf("Adding to %s\n", measuresPath.c_str());
		Serial.println(json.raw());
		if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", measuresPath.c_str(), json.raw()))
		{
			Serial.println("Data updated");
		}
		else
		{
			Serial.println("Failed to update content");
			Serial.println(fbdo.errorReason());
		}
		count++;
	}
}

void get_current_date(char *buffer, int size)
{
	std::time_t rawtime;
	std::tm *timeinfo;

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer, size, "%Y-%m-%dT%H:%M:%SZ", timeinfo);
}

void firebase_login()
{
	if (Firebase.signUp(&config, &auth, "", ""))
	{
		Serial.println("Ok");
		signupOK = true;
	}
	else
	{
		Serial.printf("Cannot connect to Firebase: %s\n", config.signer.signupError);
	}
}