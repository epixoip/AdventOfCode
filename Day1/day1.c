#include <stdio.h>
#include <errno.h>
#include <string.h>

#define error(x) {                   \
    perror(x);                       \
    return(errno ? errno : EINVAL);  \
}

struct elf {
    int num;
    long qty;
};

char *chomp(char *str, int maxlen) {
    for (int i = 0; i < strnlen(str, maxlen); i++) {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = 0;
        }
    }
    return str;
}

int compare(const void *a, const void *b) {
    return (((struct elf *) b)->qty - ((struct elf *) a)->qty);
}

int count_elves(FILE *fp) {
    char buf[BUFSIZ] = {0};
    int num_elves = 0;

    while (fgets(buf, sizeof(buf) - 1, fp)) {
        size_t read = strnlen(chomp(buf, BUFSIZ), BUFSIZ);
        if (!read) num_elves++;
    }

    rewind(fp);
    return num_elves;
}

int main(int argc, char **argv) {
    FILE *fp = 0;

    if (argc < 2 || ! (fp = fopen(argv[1], "r"))) {
        error("fopen");
    }

    int num_elves = 0;

    if (! (num_elves = count_elves(fp))) {
        error("Failed to detect elves");
    }

    struct elf elves[num_elves];
    char buf[BUFSIZ] = {0};
    int idx = 0;

    elves[0].num = 1;
    elves[0].qty = 0;

    while (fgets(buf, sizeof(buf) - 1, fp)) {
        if (! strnlen(chomp(buf, BUFSIZ), BUFSIZ)) {
            idx++;
            elves[idx].num = idx + 1;
            elves[idx].qty = 0;
            continue;
        }

        long num = 0;
        sscanf(buf, "%ld", &num);

        elves[idx].qty += num;
    }

    qsort(elves, num_elves, sizeof(struct elf), compare);

    for (int i = 0; i < 3; i++) {
        printf("%d. Elf #%d has %ld calories\n", i + 1, elves[i].num, elves[i].qty);
    }

    printf("\n%ld total calories\n\n", elves[0].qty + elves[1].qty + elves[2].qty);

    return 0;
}

