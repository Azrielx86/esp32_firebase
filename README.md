# Ejemplo ESP32 y Firebase

Los datos se guardan como:

```json
{
  "esp32_edgar": {
    "register": {
      "-Nx_anwvHXt2qImi7ewX": {
        "date": "2024-05-10-13-03-43",
        "temp": {
          "type": "Celsius",
          "value": 30
        }
      },
      "-Nx_arWFVCL87hb5y0Di": {
        "date": "2024-05-10-13-03-58",
        "temp": {
          "type": "Celsius",
          "value": 30
        }
      }
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
```