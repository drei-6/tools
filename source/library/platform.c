#include "platform.h"

#ifdef WINDOWS_OS
    #include <windows.h>
#else
    #error
#endif

typedef uptr* va_args;
PLATFORM_API uptr PlatformPrint(char *Format, ...);
PLATFORM_API void PlatformWriteConsole(void *Buffer, uptr BufferSize);
#define PlatformVaGet(arguments, type) *(type *)PlatformVaGetArgument(arguments)

internal_function va_args PlatformVaBegin(void *Format)
{
    va_args Result = (va_args)Format + 1;
    return Result;
}

internal_function va_args PlatformVaEnd()
{
    va_args Result = 0;
    return Result;
}

internal_function void *PlatformVaGetArgument(va_args *Arguments)
{
    void *Result = (*Arguments)++;
    return Result;
}

internal_function uptr PlatformStringLength(char *String)
{
    uptr Result = 0;

    while(*String++ != '\0')
    {
        Result++;
    }

    return Result;
}

PLATFORM_API uptr PlatformCopyCharacter(char *Buffer, uptr BufferSize, int Character)
{
    uptr Result = 0;

    if(BufferSize < (2 * sizeof(char)))
    {
        Log("BufferSize is %llu\n", BufferSize);
        return Result;
    }

    Buffer[0] = (char)Character;
    Buffer[1] = '\0';

    return 1;
}

PLATFORM_API uptr PlatformCopyString(char *Buffer, uptr BufferSize, char *String, uptr Length)
{
    uptr Result = 0;

    uptr MaxLength = PlatformStringLength(String);

    if(Length > MaxLength)
    {
        Log("String %s has length %llu greater than max length %llu\n", String, Length, MaxLength);
        return Result;
    }

    for(uptr Index = 0; Index < Length; Index++)
    {
        Result += PlatformCopyCharacter(Buffer + Index, BufferSize - Index, String[Index]);
    }

    return Result;
}

PLATFORM_API uptr PlatformCopySigned32(char *Buffer, uptr BufferSize, s32 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = PlatformCopyCharacter(Buffer, BufferSize, '0');
        return Result;
    }

    bool IsNegative = Value < 0;

    if(IsNegative)
    {
        Value = -Value;
    }

    char WorkBuffer[sizeof(Value) * 8] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;

    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    if(IsNegative)
    {
        WorkBuffer[WorkBufferIndex] = '-';
    }
    else
    {
        WorkBufferIndex++;
    }

    Result = PlatformCopyString(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, PlatformStringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

PLATFORM_API uptr PlatformCopyUnsigned32(char *Buffer, uptr BufferSize, u32 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = PlatformCopyCharacter(Buffer, BufferSize, '0');
        return Result;
    }

    char WorkBuffer[sizeof(Value) * 8] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;

    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    WorkBufferIndex++;

    Result = PlatformCopyString(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, PlatformStringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

PLATFORM_API uptr PlatformCopySigned64(char *Buffer, uptr BufferSize, s64 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = PlatformCopyCharacter(Buffer, BufferSize, '0');
        return Result;
    }

    bool IsNegative = Value < 0;

    if(IsNegative)
    {
        Value = -Value;
    }

    char WorkBuffer[sizeof(Value) * 8] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;

    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    if(IsNegative)
    {
        WorkBuffer[WorkBufferIndex] = '-';
    }
    else
    {
        WorkBufferIndex++;
    }

    Result = PlatformCopyString(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, PlatformStringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

PLATFORM_API uptr PlatformCopyUnsigned64(char *Buffer, uptr BufferSize, u64 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = PlatformCopyCharacter(Buffer, BufferSize, '0');
        return Result;
    }

    char WorkBuffer[sizeof(Value) * 8] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;

    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    WorkBufferIndex++;

    Result = PlatformCopyString(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, PlatformStringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

PLATFORM_API uptr PlatformCopyFloat64(char *Buffer, uptr BufferSize, f64 Value, int Precision)
{
    uptr Result = 0;

    char *Decimal = "0123456789";
    s32 IntegerPart = (int)Value;
    Result = PlatformCopySigned32(Buffer, BufferSize, IntegerPart, 10, Decimal);
    Result += PlatformCopyCharacter(Buffer + Result, BufferSize - Result, '.');

    if(Value < 0)
    {
        Value = -Value;
        IntegerPart = -IntegerPart;
    }

    Value -= IntegerPart;

    for(int Index = 0; Index < Precision; Index++)
    {
        Value *= 10;
        IntegerPart = (int)Value;
        Result += PlatformCopySigned32(Buffer + Result, BufferSize - Result, IntegerPart, 10, Decimal);
        Value -= IntegerPart;
    }

    return Result;
}

PLATFORM_API uptr PlatformPrint(char *Format, ...)
{
    uptr Result = 0;

    va_args Arguments = PlatformVaBegin(&Format);
    uptr BufferSize = 1024;
    char Buffer[1024];
    char *Octal = "01234567";
    char *Decimal = "0123456789";
    char *HexadecimalLowercase = "0123456789abcdef";
    char *HexadecimalUppercase = "0123456789ABCDEF";

    for(char *c = Format; *c != '\0';)
    {
        if(*c != '%')
        {
            Result += PlatformCopyCharacter(Buffer + Result, BufferSize - Result, *c);
            c++;
            continue;
        }

        if(*++c == '%')
        {
            Result += PlatformCopyCharacter(Buffer + Result, BufferSize - Result, *c);
            c++;
        }
        else if(*c == 'c')
        {
            int Value = PlatformVaGet(&Arguments, int);
            Result += PlatformCopyCharacter(Buffer + Result, BufferSize - Result, Value);
            c++;
        }
        else if((*c == 'd') || (*c == 'i'))
        {
            s32 Value = PlatformVaGet(&Arguments, s32);
            Result += PlatformCopySigned32(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c++;
        }
        else if(*c == 'f')
        {
            f64 Value = PlatformVaGet(&Arguments, f64);
            Result += PlatformCopyFloat64(Buffer + Result, BufferSize - Result, Value, 6);
            c++;
        }
        else if((c[0] == 'l') && (c[1] == 'l') && ((c[2] == 'd') || (c[2] == 'i')))
        {
            s64 Value = PlatformVaGet(&Arguments, s64);
            Result += PlatformCopySigned64(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c += 3;
        }
        else if((c[0] == 'l') && (c[1] == 'l') && (c[2] == 'u'))
        {
            u64 Value = PlatformVaGet(&Arguments, u64);
            Result += PlatformCopyUnsigned64(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c += 3;
        }
        else if(*c == 'o')
        {
            u32 Value = PlatformVaGet(&Arguments, u32);
            Result += PlatformCopyUnsigned32(Buffer + Result, BufferSize - Result, Value, 8, Octal);
            c++;
        }
        else if(*c == 'p')
        {
            void *Value = PlatformVaGet(&Arguments, void *);
            Result += PlatformCopyUnsigned64(Buffer + Result, BufferSize - Result, (u64)Value, 16, HexadecimalLowercase);
            c++;
        }
        else if(*c == 's')
        {
            char *Value = PlatformVaGet(&Arguments, char *);
            Result += PlatformCopyString(Buffer + Result, BufferSize - Result, Value, PlatformStringLength(Value));
            c++;
        }
        else if(*c == 'u')
        {
            u32 Value = PlatformVaGet(&Arguments, u32);
            Result += PlatformCopyUnsigned32(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c++;
        }
        else if((*c == 'x') || (*c == 'X'))
        {
            u32 Value = PlatformVaGet(&Arguments, u32);

            if(*c == 'x')
            {
                Result += PlatformCopyUnsigned32(Buffer + Result, BufferSize - Result, Value, 16, HexadecimalLowercase);
            }
            else
            {
                Result += PlatformCopyUnsigned32(Buffer + Result, BufferSize - Result, Value, 16, HexadecimalUppercase);
            }

            c++;
        }
        else
        {
            Assert(0);
        }
    }

    Arguments = PlatformVaEnd();

    PlatformWriteConsole(Buffer, PlatformStringLength(Buffer));

    return Result;
}

PLATFORM_API void PlatformWriteConsole(void *Buffer, uptr BufferSize)
{

#ifdef WINDOWS_OS
    HANDLE OutputConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if(!OutputConsoleHandle || (OutputConsoleHandle == INVALID_HANDLE_VALUE))
    {
        Log("GetStdHandle() failed: 0x%X\n", GetLastError());
        return;
    }

    if(!WriteConsoleA(OutputConsoleHandle, Buffer, (u32)BufferSize, 0, 0))
    {
        Log("GetStdHandle() failed: 0x%X\n", GetLastError());
        return;
    }
#else
    #error
#endif
}

PLATFORM_API void *PlatformAllocateMemory(uptr Size)
{
    void *Result = 0;

    if(!Size)
    {
        Log("Size is %llu\n", Size);
        return Result;
    }

#ifdef WINDOWS_OS
    Result = VirtualAlloc(0, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if(!Result)
    {
        Log("VirtualAlloc() failed: 0x%X\n", GetLastError());
        return Result;
    }
#else
    #error
#endif

    return Result;
}

PLATFORM_API void *PlatformFreeMemory(void *Memory)
{
    void *Result = Memory;

#ifdef WINDOWS_OS
    if(!VirtualFree(Memory, 0, MEM_RELEASE))
    {
        Log("VirtualFree() failed: 0x%X\n", GetLastError());
        return Result;
    }
#else
    #error
#endif

    return 0;
}

PLATFORM_API bool PlatformReadFile(char *FileName, platform_file *File)
{
    bool Result = 0;

#ifdef WINDOWS_OS
    HANDLE FileHandle = CreateFileA(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        Log("CreateFileA() failed: 0x%X\n", GetLastError());
        return Result;
    }

    LARGE_INTEGER FileSize = {0};

    if(!GetFileSizeEx(FileHandle, &FileSize))
    {
        Log("GetFileSizeEx() failed: 0x%X\n", GetLastError());
        CloseHandle(FileHandle);
        return Result;
    }
    else if(FileSize.QuadPart > MAX_U32)
    {
        Log("File %s size %llu is greater than %u\n", FileName, FileSize.QuadPart, MAX_U32);
        CloseHandle(FileHandle);
        return Result;
    }

    File->Memory = PlatformAllocateMemory(FileSize.QuadPart);

    if(!File->Memory)
    {
        CloseHandle(FileHandle);
        return Result;
    }

    DWORD BytesRead = 0;

    if(!ReadFile(FileHandle, File->Memory, (DWORD)FileSize.QuadPart, &BytesRead, 0))
    {
        DWORD Error = GetLastError();

        if(Error != ERROR_IO_PENDING)
        {
            Log("ReadFile() failed: 0x%X\n", GetLastError());
            File->Memory = PlatformFreeMemory(File->Memory);
            CloseHandle(FileHandle);
            return Result;
        }
    }

    if(BytesRead != FileSize.QuadPart)
    {
        Log("File %s read mismatch\n", FileName);
        File->Memory = PlatformFreeMemory(File->Memory);
        CloseHandle(FileHandle);
        return Result;
    }

    File->Size = FileSize.QuadPart;
    CloseHandle(FileHandle);
#else
    #error
#endif

    return 1;
}