//    嵌套在共用体中的结构体为匿名结构，可以直接访问其成员，比如 P.C、P.N 等；
//    该匿名结构体使用位域操作，每个成员仅占用一个位，共八个成员，故匿名结构体
//    的大小为一个字节；位域操作先定义的为低位，故 P.C 为 self 的 bit0 位，
//    P.N 为 self 的 bit7 位，P.self = 0b1000 0001 = 129。

#include <stdio.h>
#include <stdint.h>

struct son {
	uint8_t age;
	uint16_t length;
};

typedef union
{
    struct
    {
        uint8_t C : 1; // Carry Bit
        uint8_t Z : 1; // Zero
        uint8_t I : 1; // Disable Interrupts
        uint8_t D : 1; // Decimal Mode (unused in this implementation)
        uint8_t B : 1; // Break
        uint8_t U : 1; // Unused
        uint8_t V : 1; // Overflow
        uint8_t N : 1; // Negative
    };
    uint8_t self;
    uint16_t me;
    struct son s;
} P_t;
P_t P;

int main(void)
{
    printf("P size: %ld Bytes\n", sizeof(P));
    P.C = 1;
    printf("C: %d, P: %d\n", P.C, P.self);
    P.N = 1;
    printf("N: %d, P: %d\n", P.N, P.self);
    printf("me: %d\n", P.me);
    printf("son.age: %d\n", P.s.age);
    printf("son.length: %d\n", P.s.length);
    return 0;
}
