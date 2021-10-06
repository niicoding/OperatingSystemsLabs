#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    if (argc != 5) exit(1);
    int hours_worked = atof(argv[3]);
    double hourly_rate = atof(argv[4]), total_payment = hours_worked * hourly_rate;
    printf("%s, %s: %0.2lf", argv[2], argv[1], total_payment);
}