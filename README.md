# Comunicaci-n-LoRa-con-Arduino-MKR-1300-y-Visualizaci-n-OLED
## Descripción General
Este proyecto extiende la funcionalidad del Arduino MKR 1300 para incluir comunicación LoRa junto con visualización de datos en tiempo real usando una pantalla OLED y registro de datos en una tarjeta SD. Es ideal para aplicaciones de IoT que necesitan no solo comunicación a larga distancia sino también una interfaz de usuario local y almacenamiento de datos.

## Componentes y Tecnologías
- **Arduino MKR 1300**: Placa microcontroladora con capacidades LoRa.
- **Módulo LoRa**: Para comunicación de largo alcance.
- **Pantalla OLED**: Utilizada para mostrar datos y mensajes en tiempo real.
- **Tarjeta SD**: Para el registro de datos que pueden incluir mediciones ambientales y registros de eventos.
- **Bibliotecas Arduino**: `MKRWAN_v2`, `SPI`, `SD`, `U8g2lib` para la pantalla OLED.

## Instalación y Uso
- Instala las bibliotecas necesarias a través del Administrador de Bibliotecas del IDE de Arduino.
- Asegúrate de que todos los componentes estén correctamente conectados: MKR 1300, módulo LoRa, pantalla OLED, y tarjeta SD.
- Sube el código al Arduino MKR 1300 usando el IDE de Arduino.

## Ejemplos de Uso
- **Monitorización y visualización de datos**: La pantalla OLED puede mostrar datos en tiempo real como temperatura, humedad, y el estado de la conexión LoRa.
- **Registro de datos**: Los datos se almacenan automáticamente en la tarjeta SD en formato CSV para su posterior análisis.

## Contribuciones y Desarrollo Futuro
- **Contribuciones**: Se anima a los usuarios a contribuir con mejoras al código, nuevas funcionalidades o correcciones de errores.
- **Desarrollo futuro**: Posibles ampliaciones incluyen la integración de sensores adicionales y mejoras en la eficiencia energética.
"""

