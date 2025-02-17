#ifndef WIFI_EMBARCATECH_H
#define WIFI_EMBARCATECH_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"

#define WIFI_SSID "J"
#define WIFI_PASS "01020304"
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

#define API_KEY "MUB37OMKKLBTLMUD"  // Chave de escrita do ThingSpeak

extern int cont[5];

// Função para inicializar e conectar ao Wi-Fi
int wifi_embarcatech(uint flag_wifi_embarcatech);

#endif // WIFI_EMBARCATECH_H
