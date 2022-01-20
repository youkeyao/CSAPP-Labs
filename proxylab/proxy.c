#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

struct cache_object {
    char *request;
    char *response;
    struct cache_object *prev;
    struct cache_object *next;
};

struct cache_object *head;
struct cache_object *tail;
int cache_size = 0;
pthread_rwlock_t rwlock;

void doit(int fd);
void parse_uri(char *uri, char *host, char *port, char *path);
void handle_connection(void *argv);
char* read_cache(char *request);
void write_cache(char *request, char *response);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_rwlock_init(&rwlock, NULL);
    head = tail = (struct cache_object*)malloc(sizeof(struct cache_object));
    head->prev = NULL;
    head->next = NULL;

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE,
            port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        Pthread_create(&tid, NULL, handle_connection, (void*)&connfd);
    }
}

void handle_connection(void *argv)
{
    int fd = *(int*)argv;
    Pthread_detach(pthread_self());
    doit(fd);
    Close(fd);
}

void doit(int fd)
{
    size_t n, all = 0;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char host[MAXLINE], port[MAXLINE], path[MAXLINE];
    char client_request[MAXLINE], server_request[MAXLINE];
    char key[MAXLINE], value[MAXLINE];
    char server_response[MAX_OBJECT_SIZE];
    rio_t client_rio, server_rio;

    // read client request
    Rio_readinitb(&client_rio, fd);
    Rio_readlineb(&client_rio, buf, MAXLINE);
    strcpy(client_request, buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    parse_uri(uri, host, port, path);
    sprintf(buf, "%s %s HTTP/1.0\r\n", method, path);
    strcpy(server_request, buf);
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(&client_rio, buf, MAXLINE);
        strcat(client_request, buf);
        if (strcmp(buf, "\r\n")) {
            sscanf(buf, "%s %s\r\n", key, value);
            if (!strcmp(key, "Host:")) {
                parse_uri(value, host, port, path);
            }
            else if (!strcmp(key, "User-Agent:")) {
                sprintf(buf, "%s", user_agent_hdr);
            }
            else if (!strcmp(key, "Connection:") || !strcmp(key, "Proxy-Connection:")) {
                sprintf(buf, "%s close\r\n", key);
            }
        }
        strcat(server_request, buf);
    }
    strcat(server_request, buf);

    char *cache_response = read_cache(client_request);
    if (cache_response != NULL) {
        Rio_writen(fd, cache_response, strlen(cache_response));
        return;
    }

    // send server request
    int server_fd = Open_clientfd(host, port);
    Rio_readinitb(&server_rio, server_fd);
    Rio_writen(server_fd, server_request, strlen(server_request));
    while ((n = Rio_readlineb(&server_rio, buf, MAXLINE)) != 0) {
        Rio_writen(fd, buf, n);
        all += n;
        if (all <= MAX_OBJECT_SIZE) {
            strcat(server_response, buf);
        }
    }
    if (all <= MAX_OBJECT_SIZE) {
        write_cache(client_request, server_response);
    }

    Close(server_fd);
}

void parse_uri(char *uri, char *host, char *port, char *path)
{
    int uri_index = 0, host_index = 0, port_index = 0, path_index = 0;
    if (strlen(uri) > 7 && uri[0] == 'h' && uri[1] == 't' && uri[2] == 't' && uri[3] == 'p') {
        uri_index += 7;
    }
    while (uri[uri_index] != '/' && uri[uri_index] != ':' && uri[uri_index] != '\0') {
        host[host_index++] = uri[uri_index++];
    }
    if (host_index != 0) {
        host[host_index] = '\0';
    }
    if (uri[uri_index] == ':') {
        uri_index ++;
        while (uri[uri_index] != '/' && uri[uri_index] != '\0') {
            port[port_index++] = uri[uri_index++];
        }
    }
    else {
        port[port_index++] = '8';
        port[port_index++] = '0';
    }
    port[port_index] = '\0';
    while (uri[uri_index] != '\0') {
        path[path_index++] = uri[uri_index++];
    }
    if (path_index != 0) {
        path[path_index] = '\0';
    }
}

char* read_cache(char *request)
{
    pthread_rwlock_rdlock(&rwlock);
    struct cache_object *tmp = head->next;
    while (tmp != NULL) {
        if (!strcmp(request, tmp->request)) {
            pthread_rwlock_unlock(&rwlock);
            
            if (tmp != tail) {
                pthread_rwlock_wrlock(&rwlock);
                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev;
                cache_size -= strlen(tmp->response);
                pthread_rwlock_unlock(&rwlock);
                write_cache(tmp->request, tmp->response);
                free(tmp->request);
                free(tmp->response);
                free(tmp);
            }
            
            printf("hit\n");
            return tail->response;
        }
        tmp = tmp->next;
    }
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

void write_cache(char *request, char *response)
{
    if (strlen(response) > MAX_OBJECT_SIZE) return;

    pthread_rwlock_wrlock(&rwlock);
    struct cache_object *new_cache = (struct cache_object*)malloc(sizeof(struct cache_object));
    while (cache_size + strlen(response) > MAX_CACHE_SIZE) {
        struct cache_object *tmp = head->next;
        cache_size -= strlen(tmp->response);
        head->next = tmp->next;
        tmp->next->prev = head;
        free(tmp->request);
        free(tmp->response);
        free(tmp);
    }
    new_cache->request = (char*)malloc(sizeof(char) * strlen(request));
    strcpy(new_cache->request, request);
    new_cache->response = (char*)malloc(sizeof(char) * strlen(response));
    strcpy(new_cache->response, response);
    cache_size += strlen(response);

    tail->next = new_cache;
    new_cache->prev = tail;
    new_cache->next = NULL;
    tail = new_cache;
    pthread_rwlock_unlock(&rwlock);
}