#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_ETUDIANTS 50
#define NB_MATIERE 5

typedef struct {
    char nom[50];
    char prenom[50];
    float moyennes[NB_MATIERE];
} Etudiant;

Etudiant etudiants[MAX_ETUDIANTS] = {
    {"Doe", "John", {12.5, 14.0, 16.5, 13.0, 15.0}},
    {"Smith", "Anna", {15.5, 13.5, 14.0, 12.5, 16.0}},
};

float get_moyenne(char *nom, char *prenom, int matiere) {
    for (int i = 0; i < MAX_ETUDIANTS; i++) {
        if (strcmp(etudiants[i].nom, nom) == 0 && strcmp(etudiants[i].prenom, prenom) == 0) {
            return etudiants[i].moyennes[matiere];
        }
    }
    return -1.0;
}

void* handle_client(void* client_socket) {
    int client_fd = *(int*)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE];
    int received_bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (received_bytes < 0) {
        perror("Erreur lors de la réception");
        close(client_fd);
        pthread_exit(NULL);
    }
    buffer[received_bytes] = '\0';

    char nom[50], prenom[50];
    int matiere;
    sscanf(buffer, "%s %s %d", nom, prenom, &matiere);

    char response[BUFFER_SIZE];
    if (matiere < 0 || matiere >= NB_MATIERE) {
        snprintf(response, BUFFER_SIZE, "Erreur Serveur : l'index de matière [%d] n'est pas une matière valide", matiere);
    } else {
        float moyenne = get_moyenne(nom, prenom, matiere);
        if (moyenne >= 0) {
            snprintf(response, BUFFER_SIZE, "Moyenne de %s %s pour la matière %d: %.2f",
                    nom, prenom, matiere, moyenne);
        } else {
            snprintf(response, BUFFER_SIZE, "Étudiant %s %s non trouvé", nom, prenom);
        }
    }

    send(client_fd, response, strlen(response), 0);
    close(client_fd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Échec du bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Erreur lors de l'écoute");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Serveur en écoute sur le port %d...\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("Erreur lors de l'acceptation de la connexion");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        int* client_socket = malloc(sizeof(int));
        *client_socket = client_fd;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, client_socket) != 0) {
            perror("Erreur lors de la création du thread");
            free(client_socket);
            close(client_fd);
            continue;
        }
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}