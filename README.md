# ESP32 Firebase

> Este proyecto está realizado para la práctica de Sistemas de Comunicaciones
> 
> Grupo 01, Semestre 2024-1.
> 
> El repositorio de la app web se puede encontrar [aquí](https://github.com/Azrielx86/Siscom_Proyecto_2024-1).

## Envío de datos

Los datos se envían a Firestore de la siguiente manera:

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

Estos se almacenan en una colección por cada dispositivo que se agrega.

`/devices/<device_id>/measures/<información de sensores>`

## Recepción de datos

Los datos se recuperan de Firebase Realtime Database en un nodo perteneciente al device_id en la ruta:

`/devices/<device_id>/triggets/<actuador>`

Estos se actualizan mediante una _Firebase function_ explicada más a detalle en el repositorio de la app web.

# Configuración

Este ejemplo funciona en VSCode o CLion con la extensión PlatformIO IDE

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

En Firebase, se debe crear un documento en la colección `devices` con los siguientes datos

```
device_name: "Device name"
location: "Device location"
type: "Device type"
```

## Conexiones en la ESP32

| Pin |             Uso              | Tipo   |
|-----|:----------------------------:|--------|
| 4   |       DATA IN - DHT11        | INPUT  |
| 21  |         SCL Display          | OUTPUT |
| 22  |         SDA Display          | OUTPUT |
| 13  |   Botón prueba actuadores    | INPUT  |
| 14  |    Botón prueba sensores     | INPUT  |
| 25  | Receptor IR (Solo de prueba) | INPUT  |
| 26  |          Emisor IR           | OUTPUT |

[Pinout diagram](https://www.circuitstate.com/pinouts/doit-esp32-devkit-v1-wifi-development-board-pinout-diagram-and-reference/)