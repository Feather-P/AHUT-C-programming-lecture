#include <stdio.h>

struct Student {
    int id;
    char name[20];
    int score;
};

int main() {
    struct Student students[10] = {
        {2020010, "Alice", 100},
        {2020012, "Beta", 99},
        {2020015, "Charlie", 99},
        {2020017, "Delta", 99},
        {2020019, "Fox", 100},
    };
}

