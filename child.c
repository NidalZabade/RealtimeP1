#include "child.h"
#include "local.h"

int main(int argc, char *argv[]) {
    handler_setup(SIGUSR1, &start);
    kill(getppid(), SIGUSR1); // Send SIGUSR1 to parent (Ready signal)
    pause();
    return 0;
}

void start(int sig) {
    printf("Received starting signal [%d]\n", sig);
    int min, max;
    read_range("range.txt", &min, &max);
    float number = generate_random_float_number(min, max);
    write_random_float_number(getpid(), number);
}
void read_range(char *filename, int *min, int *max)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fscanf(fp, "%d,%d", min, max);
    fclose(fp);
}

void create_child_file(int pid)
{
    char filename[20];
    sprintf(filename, "child_%d.txt", pid);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fclose(fp);
}

float generate_random_float_number(int min, int max)
{
    srand(time(NULL) + getpid());
    return (float)rand() / (float)(RAND_MAX / max);
}

void handler_setup(int sig, void (*handler)(int)) {
	struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(sig, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
}

void write_random_float_number(int pid, float number)
{
    char filename[20];
    sprintf(filename, "child_%d.txt", pid);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    fprintf(fp, "%f", number);
    fclose(fp);
}