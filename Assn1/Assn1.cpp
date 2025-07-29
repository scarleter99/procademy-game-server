#include <iostream>
using namespace std;

void assn1(const unsigned char c)
{
	cout << static_cast<int>(c) << "의 바이너리: ";

	unsigned char temp = 1 << 7;
	for (int i = 0; i < 8; ++i)
	{
		if (c & temp)
			cout << "1";
		else
			cout << "0";

		temp >>= 1;
	}
}

void assn2()
{
	int bitIdx;
	int onFlag;
	unsigned short data = 0;

	while (1) {
		cout << "비트위치(1~16): ";
		cin >> bitIdx;

		cout << "OFF/ON [0,1] :";
		cin >> onFlag;

		if (onFlag) {
			data |= 1 << (bitIdx - 1);
		}
		else {
			data &= ~(1 << (bitIdx - 1));
		}

		for (int i = 15; i >= 0; --i) {
			const char* c = (data & (1 << i)) ? "ON" : "OFF";
			cout << i + 1 << "번 Bit: " << c << endl;
		}

		cout << endl;
	}
}

void assn3()
{
	int byteIdx;
	int tempByte;
	unsigned int data = 0;

	while (1) {
		cout << "바이트위치 (1~4): ";
		cin >> byteIdx;

		cout << "값 [0~255] :";
		cin >> tempByte;

		data &= ~(0xFF << ((byteIdx - 1) * 8));
		data |= tempByte << (byteIdx - 1) * 8;

		for (int i = 0; i < 4; ++i) {
			const unsigned char c = (data >> (8 * i)) & 0xFF;
			cout << i + 1 << "번째 Byte: " << (int)c << endl;
		}

		cout << "전체 4바이트 값: 0x" << hex << uppercase;
		cout.width(8);
		cout.fill('0');
		cout << data << dec << endl << endl;
	}
}

int main()
{
	//assn1(40);
	//assn2();
	assn3();

	return 0;
}