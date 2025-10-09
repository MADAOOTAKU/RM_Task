#include <stdio.h>

int main()
{
    int n;
    while (1)
    {
        scanf("%d", &n);
        if (n == -1)
        {
            break;
        }
        else if (n == 0)
        {
            printf("helloworld\n");
        }
        else if (n == 1)
        {
            printf("HELLOWORLD\n");
        }
    }
    return 0;
}
