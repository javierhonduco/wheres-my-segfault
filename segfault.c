#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <string.h>

void safe_puts(char *string) {
  char string_with_newline[strlen(string)+9];
  size_t string_size = strlen(string);

  strcpy(string_with_newline, string);

  string_with_newline[string_size] = '\n';
  string_with_newline[string_size+1] = 0;

  write(STDOUT_FILENO, string_with_newline, strlen(string_with_newline));
}

extern const char *__progname;

void print_stacktrace() {
  unsigned int stack_depth = 16;

  void* callstack[stack_depth];
  int frames = backtrace(callstack, stack_depth);
  char** strs = backtrace_symbols(callstack, frames);
  for(int i=0; i<frames; i++) {
    safe_puts(strs[i]);
  }

  free(strs);
}

void segfault_handler(int signal, siginfo_t* siginfo, void* context) {
  unsigned long long pc = ((ucontext_t*)context)->\
    uc_mcontext.gregs[REG_RIP];

  char *result;
  int pipeing[2];
  char buffer[200] = {0};
  int old_stdout = dup(STDOUT_FILENO);

  safe_puts("========== The program crashed =========");
  safe_puts("==> context");

  printf("0x%llx\n", pc);

  if(pipe(pipeing) == -1) {
    printf("[error] while creating pipe\n");
    exit(-1);
  }

  dup2(pipeing[1], STDOUT_FILENO);

  asprintf(&result, "addr2line -e %s 0x%llx\n", __progname, pc);
  system(result);
  close(pipeing[1]);

  read(pipeing[0], buffer, 200);

  dup2(old_stdout, STDOUT_FILENO);

  safe_puts(buffer);
  asprintf(&result, "sh addr2line.sh %s", buffer);
  system(result);

  safe_puts("==> stacktrace");
  print_stacktrace();
  safe_puts("========================================");

  exit(139);
}

__attribute__((constructor))
void install_sigsegv() {
  struct sigaction sig;
  sig.sa_sigaction = segfault_handler;
  sig.sa_flags = SA_SIGINFO;

  if(sigaction(SIGSEGV, &sig, NULL) < 0) {
    puts("[error] signal registering failed");
    exit(-1);
  }
}
