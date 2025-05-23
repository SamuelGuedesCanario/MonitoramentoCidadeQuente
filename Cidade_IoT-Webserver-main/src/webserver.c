#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/netif.h"

#include "webserver.h"
#include "setup.h"

int webserver_init(void) {
    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return -1;
    }

    if (tcp_bind(pcb, IP_ANY_TYPE, 80) != ERR_OK) {
        printf("Erro ao bindar porta 80\n");
        return -1;
    }

    pcb = tcp_listen(pcb);
    if (!pcb) {
        printf("Erro ao colocar em modo listen\n");
        return -1;
    }

    tcp_accept(pcb, tcp_server_accept);
    printf("Webserver rodando em http://%s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));
    return 0;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

void user_request(char **request) {
    if (strstr(*request, "GET /blue_on") != NULL) {
        buzzer_ligado = true;  // Liga buzzer
    }
    else if (strstr(*request, "GET /blue_off") != NULL) {
        buzzer_ligado = false; // Desliga buzzer
    }
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    // Aloca e copia o request
    char *request = (char *)malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = '\0';

    printf("Request: %s\n", request);

    // Processa o request
    user_request(&request);
    
    // Cria a resposta HTML
    char html[1024];
    snprintf(html, sizeof(html),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "<title>Monitoramento</title>\n"
        "<style>\n"
        "body { font-family: Arial; text-align: center; margin-top: 20px; }\n"
        "button { padding: 10px 20px; font-size: 16px; }\n"
        "</style>\n"
        "<script>\n"
        "setInterval(() => location.reload(), 1000);\n"
        "</script>\n"
        "</head>\n"
        "<body>\n"
        "<h1>Controle por Joystick</h1>\n"
        "<p>Temperatura: <strong>%d°C</strong></p>\n"
        "<p>Umidade: <strong>%d%%</strong></p>\n"
        "<form action=\"/blue_on\"><button>Ligar Umidificador</button></form>\n"
        "<form action=\"/blue_off\"><button>Desligar Umidificador</button></form>\n"
        "</body>\n"
        "</html>\n",
        temperatura, umidade);

    // Envia a resposta
    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    // Libera recursos
    free(request);
    pbuf_free(p);

    return ERR_OK;
}
