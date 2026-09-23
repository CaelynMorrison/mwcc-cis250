#include <stdio.h>

void fahrenheit2celsius(float fFahrenheit) {
    printf("%.2f degrees Fahrenheit is %.2f degrees Celsius.\n\n",
        fFahrenheit, (fFahrenheit - 32) * 5 / 9);
    return;
}

void celsius2fahrenheit(float fCelcius) {
    printf("%.2f degrees Celsius is %.2f degrees Fahrenheit.\n\n",
        fCelcius, fCelcius * 9 / 5 + 32);
    return;
}

int main(void) {
    int inputMenu;
    do {

        float inputTemp;
        printf("Please select one of the following options:\n"
        "1. Convert from Fahrenheit to Celsius\n"
        "2. Convert from Celsius to Fahrenheit\n"
        "3. Exit the Temperature Converter\n");
        scanf("%d", &inputMenu);
        if (inputMenu == 1) {
            printf("What is the temperature? ");
            scanf("%f", &inputTemp);
            fahrenheit2celsius(inputTemp);
        } else if (inputMenu == 2) {
            printf("What is the temperature? ");
            scanf("%f", &inputTemp);
            celsius2fahrenheit(inputTemp);
        } else if (inputMenu == 3) {
            printf("Goodbye!");
        } else {

        }
    } while (inputMenu != 3);
    return 0;
}