#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

bool only_digits(string str);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    if (argc != 2 || !only_digits(argv[1]))
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    int key = atoi(argv[1]);
    string text = get_string("plaintext: ");
    for (int i = 0; text[i]; i++)
    {
        text[i] = rotate(text[i], key);
    }
    printf("ciphertext: %s\n", text);
    return 0;
}

bool only_digits(string str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

char rotate(char c, int n)
{
    if (isalpha(c))
    {
        char base;
        if (isupper(c))
        {
            base = 'A';
        }
        else
        {
            base = 'a';
        }
        return base + (c - base + n) % 26;
    }
    return c;
}
