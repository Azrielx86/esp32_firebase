# Ejemplo ESP32 y Firebase

Los datos se guardan como:

```json
{
  "fields": {
    "date": {
      "timestampValue": "2024-05-12T12:01:38Z"
    },
    "temperature": {
      "doubleValue": 34
    },
    "humidity": {
      "doubleValue": 28
    }
  }
}
```

# Configuración

Este ejemplo funciona en VSCode con la extensión PlatformIO IDE

Requiere crear un archivo `secrets.h` en `src` para cargar los datos de WiFI y Firebase

```c
#define WIFI_SSID "WiFi Here"
#define WIFI_PWD "WiFi Here"
#define API_KEY "Firebase API Key"
#define DATABASE_URL "Firebase RTDB url"
#define AUTH_EMAIL "Firebase email"
#define AUTH_PASSWORD "Firebase password"
#define DEVICE_PATH "devices/<device_name>"
```

En Firebase, se debe crear un documento en `devices` con los siguientes datos

```
device_name: "Device name"
location: "Device location"
type: "Device type"
```

## Conexiones en la ESP32

| Pin | Uso             |
|-----|-----------------|
| 4   | DATA IN - DHT11 |
| 21  | SCL Display     |
| 22  | SDA Display     |

[Pinout diagram](https://www.circuitstate.com/pinouts/doit-esp32-devkit-v1-wifi-development-board-pinout-diagram-and-reference/)