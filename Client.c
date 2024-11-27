#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <IP Serveur> <Port> <Nom> <Prénom> <Matière>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);
    char *nom = argv[3];
    char *prenom = argv[4];
    int matiere = atoi(argv[5]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Adresse IP invalide");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connexion échouée");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char request[BUFFER_SIZE];
    snprintf(request, BUFFER_SIZE, "%s %s %d", nom, prenom, matiere);

    send(sockfd, request, strlen(request), 0);

    char response[BUFFER_SIZE];
    int received_bytes = recv(sockfd, response, BUFFER_SIZE - 1, 0);
    if (received_bytes < 0) {
        perror("Erreur lors de la réception");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    response[received_bytes] = '\0';
    printf("%s\n", response);

    close(sockfd);
    return 0;
}

