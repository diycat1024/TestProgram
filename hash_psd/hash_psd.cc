#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <functional>
#include <string>

static unsigned long hashpjw(char *arKey, unsigned int nKeyLength)
{
unsigned long h = 0, g;
char *arEnd=arKey+nKeyLength; 

while (arKey < arEnd) {
    h = (h << 4) + *arKey++;
    if ((g = (h & 0xF0000000))) {
    h = h ^ (g >> 24);
    h = h ^ g;
    }
}
printf("arEnd:%s\n", arEnd);
return h;
}
unsigned int hash1(char *str)
{
register unsigned int h;
register unsigned char *p; 

for(h=0, p = (unsigned char *)str; *p ; p++)
    h = 31 * h + *p; 

return h;
}
uint32_t fnv_hash(char const *str, int len)
{
    unsigned long hash = 2166136261; //offset_basis
//FNV prime for 32 bit is 16777619
//#define FNV_OP()  hash = (hash*16777619)^*str++
#define FNV_OP()  hash += (hash<<1) + (hash<<4) + (hash<<7) + (hash<<8) + (hash<<24);\
        hash ^=*str++;
    for (; len >= 8; len -= 8) {
        FNV_OP(); //1
        FNV_OP(); //2
        FNV_OP(); //3
        FNV_OP(); //4
        FNV_OP(); //5
        FNV_OP(); //6
        FNV_OP(); //7
        FNV_OP(); //8
    }
    switch (len) {
        case 7: FNV_OP(); /* fallthrough... */
        case 6: FNV_OP(); /* fallthrough... */
        case 5: FNV_OP(); /* fallthrough... */
        case 4: FNV_OP(); /* fallthrough... */
        case 3: FNV_OP(); /* fallthrough... */
        case 2: FNV_OP(); /* fallthrough... */
        case 1: FNV_OP(); break;
        case 0: break;
    }
    return hash;
}

const std::string HEX = "0123456789abcdef";
std::string toHex(int num) {
    if (num == 0) return "0";
    std::string result;
    int count = 0;
    while (num && count++ < 8) {
        result = HEX[(num & 0xf)] + result;
        num >>= 4;
    }
    return result;
}
int main(int argc, char const *argv[])
{
    // // char* testa = "1231231";
    // std::string testa("cca");
    // std::hash<std::string> ha1;
    // long long l1 = ha1("adfasdasadfa");
    // printf("hash1: %u; \n", l1);
    // std::hash<std::string> ha2;
    // long long l2 = ha2("dfadfadfas");
    // printf("hash2: %u; \n", l2);
    // std::hash<std::string> ha3;
    // long long l3 = ha3("vbs");
    // printf("hash3: %u; \n", l3);

    char * testa = "afasd";
    uint32_t f = fnv_hash(testa, strlen(testa));
    std::string f1 = toHex(int(f));
    printf("hash1:%u\n", f);
    printf("hash11:%s\n", f1);

    testa = "vsdfsd";
    f = fnv_hash(testa, strlen(testa));
    f1 = toHex(int(f));
    printf("hash2:%u\n", f);
    printf("hash21:%s\n", f1);


    return 0;
}
