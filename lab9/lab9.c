/*
Questions to answer at top of client.c:
(You should not need to change the code in client.c)
1. What is the address of the server it is trying to connect to (IP address and port number).
  IP: 127.0.0.1 PORT: 8000
2. Is it UDP or TCP? How do you know?
  TCP, socket call uses SOCK_STREAM as type parameter.
3. The client is going to send some data to the server. Where does it get this data from? How can
you tell in the code?
  Gets data from STDIN. You can tell because we read from STDIN and write to sfd
4. How does the client program end? How can you tell that in the code?
  Ends if user hits enter, because the number of bytes read from STDIN needs to be > 1
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  while (1) {
    char buf[BUF_SIZE];
    memset(&buf, 0, sizeof(char) * BUF_SIZE);
    if (read(client->cfd, &buf, BUF_SIZE) < 1) {
      pthread_mutex_lock(&client_id_mutex);
      printf("Ending thread for client %d\n", client->client_id);
      close(client->cfd);
      free(client);
      pthread_mutex_unlock(&client_id_mutex);
      return NULL;
    }
    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    printf("Msg #   %d; Client ID %d: %s", total_message_count, client->client_id, buf);
    pthread_mutex_unlock(&count_mutex);
  }
  // TODO: print the message received from client
  // TODO: increase total_message_count per message
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    pthread_t thread;
    struct client_info *cl_inf = malloc(sizeof(struct client_info));
    unsigned int len = sizeof(addr);
    unsigned int *addrlen = &len;
    cl_inf->cfd = accept(sfd, (struct sockaddr *)&addr, addrlen);
    cl_inf->client_id = client_id_counter;
    client_id_counter++;
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info
    pthread_create(&thread, NULL, handle_client, (void *)cl_inf);
    printf("New client created! ID %d on socket FD %d\n", cl_inf->client_id, cl_inf->cfd);
    pthread_detach(thread);
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
