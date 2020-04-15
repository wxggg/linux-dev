#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int read_paddr(const char *f, unsigned long *paddr, int n)
{
    char buf[1024], *p = NULL;
    int i, ret, fd;

    if ((fd = open(f, O_RDONLY)) < 0)
    {
        fprintf(stderr, "can't open %s\n", f);
        return 1;
    }

    ret = read(fd, buf, 1024);
    close(fd);

    p = buf;
    for (i = 0; i < n; i++)
    {
        paddr[i] = atol(p);
        while (*p != ',')
            p++;
        p++;
    }
    return 0;
}

int mmap_laddr(int mmap_fd, unsigned long *paddr, unsigned long **laddr, int n)
{
    int mmap_size = 0x1000;
    for (size_t i = 0; i < n; i++)
    {
        unsigned long base = paddr[i] & ~(mmap_size - 1);
        unsigned long offset = (unsigned long)paddr[0] & (mmap_size - 1);
        char *mmap_addr = (char *)mmap(0, mmap_size, PROT_READ, MAP_SHARED, mmap_fd, base);
        laddr[i] = (unsigned long *)(mmap_addr + offset);
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    const char *task_signal = "/sys/module/kernsensor/parameters/task_signal";
    const char *flag_signal = "/sys/module/kernsensor/parameters/flag_signal";

    unsigned long task_paddr[16], flag_paddr[16];
    unsigned long *task_laddr[16], *flag_laddr[16];

    int mmap_fd;
    if ((mmap_fd = open("/dev/mem", O_RDONLY)) < 0)
    {
        fprintf(stderr, "can't open /dev/mem\n");
        return 1;
    }

    read_paddr(task_signal, task_paddr, 16);
    mmap_laddr(mmap_fd, task_paddr, task_laddr, 2);

    read_paddr(flag_signal, flag_paddr, 16);
    mmap_laddr(mmap_fd, flag_paddr, flag_laddr, 2);

    while (1)
    {
        sleep(1);
        unsigned long task0 = *(task_laddr[0]), task1 = *(task_laddr[1]);
        unsigned long flag0 = *(flag_laddr[0]), flag1 = *(flag_laddr[1]);

        int pid0 = (int)(task0 & 0xffffffff), tid0 = (int)(task0 >> 32);
        int pid1 = (int)(task1 & 0xffffffff), tid1 = (int)(task1 >> 32);

        printf("core 0: pid=%8d tid=%8d flag=%16ld\n", pid0, tid0, flag0);
        printf("core 1: pid=%8d tid=%8d flag=%16ld\n", pid1, tid1, flag1);
        printf("\n");
    }

    return 0;
}
