//
// Created by xiaotian on 2021/10/31.
//

#include "CPU.h"

void CPU::InitializeFont() {
    byte characters[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0,
                         0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80,
                         0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
                         0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80,
                         0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80,
                         0x80};
    int len = sizeof(characters) / sizeof(characters[0]);
    memcpy(RAM, characters, len * sizeof(byte));
    //  Array.Copy(characters, RAM, characters.Length);
}

void CPU::LoadProgram(byte program[], int arrayLen) {
    for (int i = 0; i < 4096; ++i) {
        RAM[i] = 0x00;
    }
    InitializeFont();  //work
    for (int i = 0; i < arrayLen; i++) {
        RAM[512 + i] = program[i];
    }
    PC = 512;
}

void CPU::ExecuteOpcode() {
    if (WaitingForKeyInput) {
        return;
    }
    if (stopWatch == 0) stopWatch = millis();
    if (millis() - stopWatch > 16) {
        if (DelayTimer > 0) DelayTimer--;
        if (SoundTimer > 0) SoundTimer--;
        stopWatch = millis();
    }
    ushort opcode = (ushort) ((RAM[PC] << 8) | RAM[PC + 1]);
    ushort nibble = (ushort) (opcode & 0xF000);
    PC += 2;
    switch (nibble) {
        case 0x0000:
            if (opcode == 0x00E0) {
                for (int i = 0; i < 64 * 32; i++) {
                    Display[i] = 0;
                }
            } else if (opcode == 0x00EE) {
                PC = astack->pop();
//                PC = Stack.top();
//                Stack.pop();
            } else {
                Serial.print("unsupported opcode at line 54 ");
                Serial.printf("%08X \n", opcode);
                break;
            }
            break;
        case 0x1000:
            //Jumps to address NNN.
            PC = (ushort) (opcode & 0x0FFF);
            break;
        case 0x2000:
            //Calls subroutine at NNN.
            astack->push(PC);
            //    Stack.push(PC) ;
            PC = (ushort) (opcode & 0x0FFF);
            break;
        case 0x3000:
            //Skips the next instruction if VX equals NN.
            //(Usually the next instruction is a jump to skip a code block);
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) PC += 2;
            break;
        case 0x4000:
            //Skips the next instruction if VX equals NN.
            //(Usually the next instruction is a jump to skip a code block);
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) PC += 2;
            break;
        case 0x5000:
            //5XY0	Cond	if (Vx == Vy)
            //	Skips the next instruction if VX equals VY.
            //	(Usually the next instruction is a jump to skip a code block);
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) PC += 2;
            break;
        case 0x6000:
            //6XNN	Const	Vx = N	Sets VX to NN.
            V[(opcode & 0x0F00) >> 8] = (byte) (opcode & 0x00FF);
            break;
            //7XNN	Const	Vx += N	Adds NN to VX. (Carry flag is not changed);
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += (byte) (opcode & 0x00FF);
            break;

        case 0x8000: {
            byte vx = (opcode & 0x0F00) >> 8;
            byte vy = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F) {
                //8XY0	Assig	Vx = Vy	Sets VX to the value of VY.
                case 0:
                    V[vx] = V[vy];
                    break;
                    //8XY1	BitOp	Vx |= Vy	Sets VX to VX or VY. (Bitwise OR operation);
                case 1:
                    V[vx] |= V[vy];
                    break;
                    //8XY2	BitOp	Vx &= Vy	Sets VX to VX and VY. (Bitwise AND operation);
                case 2:
                    V[vx] &= V[vy];
                    break;
                    //8XY3	BitOp	Vx ^= Vy	Sets VX to VX xor VY.
                case 3:
                    V[vx] ^= V[vy];
                    break;
                    //8XY4	Math	Vx += Vy	Adds VY to VX.
                    //VF is set to 1 when there's a carry, and to 0 when there is not.
                case 4:
                    V[15] = (byte) ((V[vx] + V[vy]) > 255 ? 1 : 0);
                    V[vx] = (byte) ((V[vx] + V[vy]) & 0x00FF);
                    break;
                case 5:
                    V[15] = (byte) ((V[vx] - V[vy]) < 0 ? 0 : 1);
                    V[vx] = (byte) ((V[vx] - V[vy]) & 0x00FF);
                    break;
                case 6:
                    V[15] = (byte) (V[vx] & 0x0001);
                    V[vx] >>= 1;
                    break;
                case 7:
                    V[15] = (byte) (V[vy] > V[vx] ? 1 : 0);
                    V[vx] = (byte) ((V[vy] - V[vx]) & 0x00FF);
                    break;
                case 0xe:
                    V[15] = (byte) ((V[vx] & 0x80) == 0x80 ? 1 : 0);
                    V[vx] <<= 1;
                    break;
                default:
                    Serial.print("unsupported opcode at line 137");
                    Serial.printf("%08X \n", opcode);
                    break;
            }
        }
            break;
        case 0x9000:
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) PC += 2;
            break;
        case 0xA000:
            I = (ushort) (opcode & 0x0FFF);
            break;
        case 0xB000:
            PC = (ushort) ((opcode & 0x0FFF) + V[0]);
            break;
        case 0xC000:
            V[(opcode & 0x0F00) >> 8] = (byte) ((rand() % 0xFF) & (opcode & 0x00FF));
            break;
        case 0xD000: {
            //关于本opcode，请参考wiki
            byte xloc = V[(opcode & 0x0F00) >> 8];
            byte yloc = V[(opcode & 0x00F0) >> 4];
            byte n = opcode & 0x000F;
            V[15] = 0;
            for (int i = 0; i < n; i++) {
                byte mem = RAM[I + i];
                for (int j = 0; j < 8; j++) {
                    //参见
                    byte pixel = ((byte) ((mem >> (7 - j)) & 0x01));
                    int index = xloc + j + (yloc + i) * 64;
                    if (index > 2047) continue;
                    if (pixel == 1 && Display[index] != 0) V[15] = 1;
                    Display[index] =
                            ((Display[index] != 0 && pixel == 0)
                             || (Display[index] == 0 && pixel == 1)) ? 0x01 : 0;
                }
            }
            break;
        }
        case 0xE000:
            //Skip next instruction if key with the value of Vx is pressed.
            //Checks the keyboard, and if the key corresponding to the value of Vx is currentlyin the down position
            //, PC is increased by 2.
            if ((opcode & 0x00FF) == 0x009E) {
                // Console.WriteLine($"need to press : {(opcode & 0x0F00) >> 8}");
                //这里debug了半个小时。。。原因是写成了 if (key_m[(opcode & 0x0F00) >> 8] == true)
                if (key_array[V[(opcode & 0x0F00) >> 8]]) PC += 2;
                //存疑
                //  if ((KeyBoard >> (V[(opcode & 0x0F00) >> 8] & 0x01)) == 0x01) PC += 2;
                break;
            } else if ((opcode & 0x00FF) == 0x00A1) {
                //这里debug了半个小时。。。原因是写成了 if (key_m[(opcode & 0x0F00) >> 8] == false)
                if (!key_array[V[(opcode & 0x0F00) >> 8]]) PC += 2;
                // if ((KeyBoard >> (V[(opcode & 0x0F00) >> 8] & 0x01)) != 0x01) PC += 2;
                break;
            } else {
                Serial.print("unsupported opcode ");
                Serial.printf("%08X \n", opcode);
                break;
            }
        case 0xF000:
            int fx = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x0007:
                    V[fx] = DelayTimer;
                    break;
                case 0x000A:
                    WaitingForKeyInput = true;
                    WaitingKeyInputKey = (byte) fx;
                    //PC -= 2;
                    break;
                case 0x0015:
                    DelayTimer = V[fx];
                    break;
                case 0x0018:
                    SoundTimer = V[fx];
                    break;
                case 0x001E:
                    I += V[fx];
                    break;
                case 0x0029:
                    //这个没看懂
                    //
                    //
                    //
                    I = (ushort) (V[fx] * 5);
                    break;
                    //
                    //
                    //
                    //
                case 0x0033:
                    RAM[I] = (byte) (V[fx] / 100);
                    RAM[I + 1] = (byte) ((V[fx] % 100) / 10);
                    RAM[I + 2] = (byte) (V[fx] % 10);
                    break;
                case 0x0055:
                    for (int i = 0; i <= fx; i++) {
                        RAM[I + i] = V[i];
                    }
                    break;
                case 0x0065:
                    for (int i = 0; i <= fx; i++) {
                        V[i] = RAM[I + i];
                    }
                    break;
                default: {
                    Serial.print("unsupported opcode at line 244");
                    Serial.printf("%08X \n", opcode);
                    break;
                }
            }
            break;

    }
}
