//
// Created by xiaotian on 2021/10/31.
//

#ifndef PIO_IN_CLION_CPU_H
#define PIO_IN_CLION_CPU_H

#include <Arduino.h>
#include<stack>
#include <random>
#include <new>
#include "ArrayStack.h"
class CPU {
public:
    byte RAM[4096];
    byte V[16];
    ushort  PC=0;
    ushort  I=0;
    ArrayStack<ushort> *astack = new ArrayStack<ushort>();
    byte DelayTimer;
    byte SoundTimer;
    ushort KeyBoard;
    bool key_array[16];
    uint Display[64*32];
    std::random_device rand;
    bool WaitingForKeyInput = false;
    byte WaitingKeyInputKey = 0;
    void InitializeFont();
    void LoadProgram(byte[], int  );
    unsigned  long stopWatch=0;
    void ExecuteOpcode();
};


#endif //PIO_IN_CLION_CPU_H
