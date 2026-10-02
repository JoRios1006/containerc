#define _GNU_SOURCE
#include <linux/sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAY_FAIL(expr, msg)                                                    \
  do {                                                                         \
    if ((expr) != 0) {                                                         \
      perror("[CONTENEDOR]");                                                  \
      perror(msg);                                                             \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

void container_main() {
  printf("[Contenedor] PID aquí dentro es: %d\n", getpid());
  MAY_FAIL(mount("none", "/", NULL, MS_REC | MS_PRIVATE, NULL),"Fallo al hacer la raíz privada");
  MAY_FAIL(mount("ALPINE", "ALPINE", "bind", MS_BIND | MS_REC, NULL), "Fallo al convertir ALPINE en un punto de montaje");
  MAY_FAIL(chdir("ALPINE"), "Fallo al hacer chdir a ALPINE");
  MAY_FAIL(syscall(SYS_pivot_root, ".", "."), "Fallo en pivot_root");
  MAY_FAIL(umount2(".", MNT_DETACH), "Fallo al desmontar la raíz antigua");
  MAY_FAIL(chdir("/"), "Fallo al hacer chdir a / después del pivot");
  MAY_FAIL(mount("proc", "/proc", "proc", 0, NULL), "Fallo al montar /proc en Alpine");

  printf("[Contenedor] Lanzando terminal interactiva...\n");
  char *cmd[] = {"/bin/sh", NULL};
  MAY_FAIL(execvp(cmd[0], cmd), "Fallo en execvp");
  exit(1);
}

int main() {
  struct clone_args cl_args = {0};
  cl_args.flags = CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS;
  cl_args.exit_signal = SIGCHLD;
  pid_t pid = syscall(SYS_clone3, &cl_args, sizeof(cl_args));
  if (pid < 0) {
    perror("[Host] Error al ejecutar clone3. ¿Eres root?");
    return 1;
  }

  if (pid == 0) {
    container_main();
  }
  printf("[Host] Contenedor creado. Su PID en el host es: %d\n", pid);
  waitpid(pid, NULL, 0);
  printf("[Host] El contenedor se ha cerrado correctamente.\n");
  return 0;
}
