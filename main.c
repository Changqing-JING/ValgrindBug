#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
uint8_t *memory = NULL;

__attribute__((naked)) void eval(uint8_t *mem) {
  asm(".intel_syntax noprefix");
  asm("mov rax,0x11");
  asm("mov QWORD PTR [rdi+rax*1],0x1122");
  asm("mov rax,0x11");
  asm("mov QWORD PTR [rdi+rax*1],0x1122");
  asm("ret");
  asm(".att_syntax prefix");
}

size_t getOSMemoryPageSize() { return (size_t)sysconf(_SC_PAGE_SIZE); }

void signalHandler(int32_t const signalId, siginfo_t *const si, void *const ptr) {
  long const offsetInMemory = (uint8_t *)si->si_addr - memory;
  char msg[1000];
  int len = sprintf(msg, "==> offset in memory: %lu\n", offsetInMemory);
  write(2, msg, (size_t)len);
  if (offsetInMemory < 0 || offsetInMemory >= getOSMemoryPageSize()) { exit(1); }
  // mprotect is signal safe on Linux
  mprotect(memory, getOSMemoryPageSize(), PROT_READ | PROT_WRITE);
}

int main(void) {
  memory = (uint8_t *)mmap(NULL, getOSMemoryPageSize(), PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  struct sigaction sa = {};
  sa.sa_sigaction = signalHandler;
  sa.sa_flags = SA_SIGINFO | SA_NODEFER;
  sigfillset(&sa.sa_mask);
  sigaction(SIGSEGV, &sa, NULL);
  printf("memory is %p\n", memory);
  eval(memory);
}