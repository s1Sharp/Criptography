#include <iostream>
#include <fstream>
using namespace std;

#define xtime(x) ((x<<1)^(((x>>7)&1)*0x1b))
#define Multiply(x,y)(((y&1)*x)^((y>>1&1)*xtime(x))^((y>>2&1)*xtime(xtime(x)))^((y>>3&1)*xtime(xtime(xtime(x))))^((y>>4&1)*xtime(xtime(xtime(xtime(x))))))

// task1
void FillShiftRowCount(int* shiftRowCount, int m);
void FillSbox(int** sbox, int n);
void FillinvSbox(int** invsbox, int n);
void FillState(int**S, int I, int J, ifstream&f);
void ShowState(int**S, int I, int J);
int opp(int m);
void Mult(int*r);
void Mult2(int*r);

//возвращает предыдущее слово
void OP(int*temp, int J, int**W);
void Rotl(int W[4], int o);
//сдвиг последних трех строк
void ShiftRow(int**m, int I, int J, int c[3]);
void InvShiftRow(int**m, int I, int J, int c[3]);
void SubByte(int**m, int I, int J, int**S);
void SubByte2(int m[4],  int**Sbox);
void InvMixColumn(int**m, int I, int J);
void MixColumn(int**m, int I, int J);
void AddRoundKey(int**m, int I, int J, int r, int**W);

// task2
void Cipher(int**M,int**h, int N, int r, int**SBox);
void Init(int**inpM, int**M, int I, int n);
void Ex(int**M, int**H, int n);
