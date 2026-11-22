/*
 * Task 06 - what goes wrong with the wrong format specifier, and what the
 * return value of scanf/sscanf tells us.
 * NOTE: gcc -Wall -Wextra deliberately WARNS about the wrong specifiers below;
 * those warnings are part of the evidence (see output_task06_wrong_specifier.txt).
 */
#include <stdio.h>

int main(void)
{
    long big = 5000000000L;                 /* needs 33 bits */
    printf("-- printing a long (5,000,000,000)\n");
    printf("  %%ld -> %ld   (correct)\n", big);
    printf("  %%d  -> %d   (WRONG: only the low 32 bits are used)\n", big);

    unsigned int u = 4000000000u;
    printf("-- printing an unsigned int (4,000,000,000)\n");
    printf("  %%u  -> %u   (correct)\n", u);
    printf("  %%d  -> %d   (WRONG: read as signed)\n", u);

    double d = 0.0;
    sscanf("3.14", "%f", &d);               /* %f stores a 4-byte float into an 8-byte double */
    printf("-- reading 3.14 into a double\n");
    printf("  sscanf with %%f  -> d = %g   (WRONG: expected 3.14)\n", d);
    d = 0.0;
    sscanf("3.14", "%lf", &d);
    printf("  sscanf with %%lf -> d = %g   (correct)\n", d);

    printf("-- return value of sscanf (number of fields converted)\n");
    int a = -1, b = -1, r;
    r = sscanf("12 34", "%d %d", &a, &b);   printf("  \"12 34\"  -> returned %d\n", r);
    r = sscanf("12 abc", "%d %d", &a, &b);  printf("  \"12 abc\" -> returned %d (second field failed)\n", r);
    r = sscanf("abc 34", "%d %d", &a, &b);  printf("  \"abc 34\" -> returned %d (first field failed)\n", r);
    r = sscanf("",      "%d %d", &a, &b);   printf("  \"\"        -> returned %d (EOF, no input at all)\n", r);

    printf("-- printing rupees and paise from an integer amount\n");
    long paise = 16250;
    printf("  %ld paise -> Rs %ld.%02ld\n", paise, paise / 100, paise % 100);
    paise = 5005;
    printf("  %ld paise -> Rs %ld.%02ld   (%%02ld keeps the leading zero; %%ld.%%ld would print Rs 50.5)\n", paise, paise / 100, paise % 100);
    return 0;
}
