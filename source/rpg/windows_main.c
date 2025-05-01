#include "windows_main.h"

int main(int ArgumentCount, char **Arguments)
{
    platform_file File = {0};
    PlatformReadFile("a.txt", &File);
    File.Memory = PlatformFreeMemory(File.Memory);

    Print("%% Hello, world! %%\n");
    Print("%% Hello, world! %%\n");
    Print("%% Hello, world! %%\n");

    Print("%c %c %c\n", 'A', 'B', 'C');
    Print("%c %c %c\n", 'A', 'B', 'C');
    Print("%c %c %c\n", 'A', 'B', 'C');

    Print("%d %i %d\n", 0, -1, 1234);
    Print("%d %i %d\n", 0, -1, 1234);
    Print("%d %i %d\n", 0, -1, 1234);

    Print("%f %f %f\n", 0.0, -1.0, 1.234);
    Print("%f %f %f\n", 0.0, -1.0, 1.234);
    Print("%f %f %f\n", 0.0, -1.0, 1.234);

    Print("%lld %lli\n", 9223372036854775807, 9223372036854775807);
    Print("%lld %lli\n", 9223372036854775807, 9223372036854775807);
    Print("%lld %lli\n", 9223372036854775807, 9223372036854775807);

    Print("%llu\n", 18446744073709551615);
    Print("%llu\n", 18446744073709551615);
    Print("%llu\n", 18446744073709551615);

    Print("%o %o %o\n", 0, 8, 16);
    Print("%o %o %o\n", 0, 8, 16);
    Print("%o %o %o\n", 0, 8, 16);

    Print("%p %p %p\n", 0, main, Arguments);
    Print("%p %p %p\n", 0, main, Arguments);
    Print("%p %p %p\n", 0, main, Arguments);

    Print("%s %s %s\n", "ABC", "DEF", "GHI");
    Print("%s %s %s\n", "ABC", "DEF", "GHI");
    Print("%s %s %s\n", "ABC", "DEF", "GHI");

    Print("%u %u %u\n", 0, -1, 1234);
    Print("%u %u %u\n", 0, -1, 1234);
    Print("%u %u %u\n", 0, -1, 1234);

    Print("%x %X %x\n", 0, -1, 255);
    Print("%x %X %x\n", 0, -1, 255);
    Print("%x %X %x\n", 0, -1, 255);

    return 0;
}