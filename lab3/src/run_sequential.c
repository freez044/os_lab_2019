#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s seed array_size\n", argv[0]);
    return 1;
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    // дочерний процесс: заменяем себя программой sequential_min_max
    execl("./sequential_min_max", "sequential_min_max", argv[1], argv[2],
          (char *)NULL);
    perror("execl");  // сюда попадаем, только если exec не сработал
    exit(1);
  }

  // родитель: ждём завершения ребёнка и смотрим код выхода
  int status;
  waitpid(pid, &status, 0);
  if (WIFEXITED(status)) {
    printf("sequential_min_max exited with code %d\n", WEXITSTATUS(status));
  }
  return 0;
}
