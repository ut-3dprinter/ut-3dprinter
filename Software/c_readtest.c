#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int fp;
    unsigned short theta[3];

    if (argc == 1) {
        printf("Too few arguments\n");
        return 1;
    }

    fp = open("output/thetas_DD_carriage_for_atom.dat", O_RDONLY);

    if (fp < 0) {
        printf("Can't open file\n");
        return 1;
    }

    while (read(fp, theta, sizeof(short)*3) != 0) {
        printf("%u %u %u\n", theta[0], theta[1], theta[2]);
    }

    close(fp);
}
