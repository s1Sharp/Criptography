#include "utils.h"

void task1()
{
	setlocale(LC_ALL, "Russian");
	ifstream f1("text.txt"); // encrypted text
	ifstream f2("key.txt"); // key
	ifstream f3("Text1.txt"); // sbox
	ifstream f4("Text2.txt"); //invsbox
	
	int m=16;
	int NB=4;
	int q; 
	int NK;
	int r;
	int N = 4;
	int O = 1;

	cout << "Enter size of cipher key q = ";
	cin >> q;
	NK = q / 4;

	int ShiftRowC[3];
	FillShiftRowCount(ShiftRowC, m);

	//fill sbox and invsbox
	int** SBox = new int*[N*N];;
	FillSbox(SBox, N);
	int** InvSBox = new int*[N*N];;
	FillinvSbox(InvSBox, N);


/*
	int **SBox = new int*[N*N];
	for (int i = 0;i < N*N;i++)
		SBox[i] = new int[N*N];
	for (int i = 0;i < 16;i++)
	{
		for (int j = 0;j < N*N;j++)
		{
			f3 >> hex >> SBox[i][j];
		}
	}
	f3.close();
*/
/*
	int** InvSBox = new int*[N*N];
	for (int i = 0;i < N*N;i++)
		InvSBox[i] = new int[N*N];
	for (int i = 0;i < 16;i++)
	{
		for (int j = 0;j < N*N;j++)
		{
			f4 >> hex >> InvSBox[i][j];
		}
	}
	f4.close();
*/

	//inp text
	int**inpB = new int*[N];
	for (int i = 0;i < N;i++)
		inpB[i] = new int[NB];
	FillState(inpB, N, NB, f1);
	cout << "input" << endl;
	ShowState(inpB, N, NB);

	//key
	int**inpK = new int*[N];
	for (int i = 0;i < N;i++)
		inpK[i] = new int[NK];
	FillState(inpK, N, NK, f2);
	cout << "key=" << endl;
	ShowState(inpK, N, NK);
	
	//num of rounds
	if (m == 16 && q == 16) r = 10;
	if (m == 16 && q == 24 || m == 24 && q == 16 || m == 24 && q == 24) r = 12;
	if (m == 32 || q == 32) r = 14;

	//RoundConst
	int Rcon[4][11] = { {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36}, 
						{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
						{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
						{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
						//массив который состоит из битов 32 разрядного слова и является постоянным для данного раундадля AES 128

	//key expansion
	int E = NB * (r + 1);////количество 4х байтовых слов в расширенном ключе
	int**W = new int*[N];
	for (int i = 0;i < N;i++)
	    W[i] = new int [E];
	for (int j = 0;j < NK;j++)
	{
		for (int i = 0;i < N;i++)
		{
			W[i][j] = inpK[i][j];
		}
	}
	for (int j = NK;j < NB*(r + 1);j+=NB) //с 4 слова до 44 слова через каждые 4(4, 8, 12, 16)
	{
		int chek[4];
		chek[0] = W[0][j-1];
		chek[1] = W[1][j-1];
		chek[2] = W[2][j-1];
		chek[3] = W[3][j-1];
		Rotl(chek, O);
		SubByte2(chek, SBox);
		for (int i = 0;i < N;i++)
		{
			W[i][j] = W[i][j - NK] ^ chek[i] ^ Rcon[i / NK][j];//o-константа на которую цикличский сдвиг происходит
			
		}
		for (int l = 1;l < NK&&l + j < NB*(r + 1);l++)
		{
			for (int h = 0;h < N;h++)
			{		
				W[h][l + j] = W[h][l + j - NK] ^ W[h][l + j - 1];
			}
		}
	}
	cout << "ExpansionKey:" << endl;
	ShowState(W, N, E);
	
	//cipher inpB
	AddRoundKey(inpB, N, NB, 0, W);
	for (int i = 1;i < r;i++)
	{
		SubByte(inpB,N, NB, SBox);
		ShiftRow(inpB,N,NB,ShiftRowC);
		MixColumn(inpB,N,NB);
		AddRoundKey(inpB,N,NB,i, W);
	}
	SubByte(inpB, N, NB, SBox);
	ShiftRow(inpB, N, NB, ShiftRowC);
	AddRoundKey(inpB, N, NB, r, W);
	cout << "input after cipher:" << endl;
	ShowState(inpB, N, NB);


	//decipher
	AddRoundKey(inpB, N, NB, r, W);
	InvShiftRow(inpB, N, NB, ShiftRowC);
	SubByte(inpB, N, NB, InvSBox);

	for (int i = r-1;i >=1 ;i--)
	{
		AddRoundKey(inpB, N, NB, i, W);
		InvMixColumn(inpB, N, NB);
		InvShiftRow(inpB, N, NB, ShiftRowC);
		SubByte(inpB, N, NB, InvSBox);
	}
	AddRoundKey(inpB, N, NB, 0, W);
	cout << "input after decipher:" << endl;
	ShowState(inpB, N, NB);
	system("pause");
    return;
}

void task2()
{
	setlocale(LC_ALL, "Russian");
	ifstream f1("text.txt");//шифруемый текст
	ifstream f2("key.txt");//фиксированное начальное значение
	ifstream f3("Text1.txt");// sbox

	int  N = 4, O, r, w, NM, jo;//N-для матриц, О- конст сдвига, r-кол-во раундов в шифровании, w-колво байтов в сообщении М
	cout << "Enter num of bytes in M = ";
	//cin >> w;
	w=32;
	cout << "Enter num of bytes in cipher key = ";
	//cin >> r;
	r=10;
	NM = w / 4;//колво столбцов в матрице послного сообщения М
	jo = w / 16;//колво блоков в 16 бит в сообщении М

	int **SBox = new int*[N*N];
	FillSbox(SBox, N);
	

	//H=h0(key from txt)
	int**H = new int*[N];
	for (int i = 0;i < N;i++)
		H[i] = new int[N];
	FillState(H, N, N, f2);
	cout << "H0=" << endl;
	ShowState(H, N, N);

	
	///M-изначальное целое сообщение
	int **M = new int*[N];
	for (int i = 0;i < N;i++)
		M[i] = new int[NM];
	FillState(M, N, NM, f1);
	cout << "M:" << endl;
	ShowState(M, N, NM);
	

	for (int i = 0;i < jo;i++)//для каждого блока сообщения М
	{
		int**inpM = new int*[N];
		for (int j = 0;j < N;j++)
			inpM[j] = new int[N];
		Init(inpM, M, i, N);//i-тый блок данных М
		Ex(inpM, H, N);//в inpM хранится сложение по модулю  блока М i-того и Н //B

		//buf = M^h= inpM
		int**buf = new int*[N];
		for (int j = 0;j < N;j++)
			buf[j] = new int[N];
		for (int l = 0;l < N;l++)
		{
			for (int j = 0;j < N;j++)
				buf[l][j] = inpM[l][j];
		}

		Cipher(inpM, H, N, r, SBox);//шифруем M^h
		Ex(inpM, buf, N);
		for (int l = 0;l < N;l++)
		{
			for (int j = 0;j < N;j++)
				H[l][j] = inpM[l][j];
		}
	}
	cout << "h:" << endl;
	for (int i = 0;i < N;i++)
	{
		for (int j = 0;j < N;j++)
			cout << H[j][i];
	}
	cout << endl;
	system("pause");
	return;
}
