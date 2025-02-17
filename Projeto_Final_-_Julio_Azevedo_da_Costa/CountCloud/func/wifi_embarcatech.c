#include "wifi_embarcatech.h"

static struct tcp_pcb *tcp_client_pcb;
static ip_addr_t server_ip;

//Chamada da variável global externa a esse arquivo Cont
extern int cont[5];

// Função de Callback para receber resposta do ThingSpeak
static err_t http_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    printf("Resposta do ThingSpeak: %.*s\n", p->len, (char *)p->payload);
    pbuf_free(p);
    return ERR_OK;
}

// Função de Callback para conexão TCP e criação da requisição 
static err_t http_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("Erro na conexão TCP\n");
        return err;
    }
    int cont_total=0;
    for(int i=0;i<5;i++){
        cont_total+=cont[i];
    }

    printf("Conectado ao ThingSpeak!\n");

    char request[256];
    snprintf(request, sizeof(request),
        "GET /update?api_key=%s&field1=%d&field2=%d&field3=%d&field4=%d&field5=%d&field6=%d HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        API_KEY, cont[0], cont[1], cont[2], cont[3], cont[4], cont_total, THINGSPEAK_HOST);

    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    tcp_recv(tpcb, http_recv_callback);

    return ERR_OK;
}

// Resolver DNS e conectar ao servidor
static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {
        printf("Endereço IP do ThingSpeak: %s\n", ipaddr_ntoa(ipaddr));
        tcp_client_pcb = tcp_new();
        tcp_connect(tcp_client_pcb, ipaddr, THINGSPEAK_PORT, http_connected_callback);
    } else {
        printf("Falha na resolução de DNS\n");
    }
}


int wifi_embarcatech(uint flag_wifi_embarcatech) {
    //Flag para garantir que os componentes usados em wifi_embarcatech iniciarem apenas na primeira execução.
    if(flag_wifi_embarcatech==0){
        if (cyw43_arch_init()) {
            printf("Falha ao iniciar Wi-Fi\n");
            return 1;
        }
    }
    //Iniciando a placa em modo Station para se conectar à rede
    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");
    //Função que se conecta à rede, passando as credenciais de acesso
    if (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return 1;
    }

    printf("Wi-Fi conectado!\n");

    //Chamada da função que iniciará o dns
    dns_gethostbyname(THINGSPEAK_HOST, &server_ip, dns_callback, NULL);

    return 0;
}

