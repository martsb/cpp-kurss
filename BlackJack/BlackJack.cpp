#include <array>
#include <vector>
#include <iostream>
#include <iomanip>

using Byte = uint8_t;
using State = std::array<std::array<Byte, 4>, 4>; // 4x4 state array
using Word = std::array<Byte, 4>;                // A word of 4 bytes
using KeySchedule = std::vector<Word>;          // Key schedule as an array of words

const Byte SBox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};



const Byte InvSBox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};


const Byte RCON[10] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

std::vector<Byte> padPKCS7(const std::vector<Byte>& input) {
	size_t paddingLength = 16 - (input.size() % 16);
	std::vector<Byte> paddedInput = input;
	paddedInput.insert(paddedInput.end(), paddingLength, static_cast<Byte>(paddingLength));
	return paddedInput;
}

std::vector<Byte> unpadPKCS7(const std::vector<Byte>& paddedInput) {
	Byte paddingLength = paddedInput.back();
	return std::vector<Byte>(paddedInput.begin(), paddedInput.end() - paddingLength);
}

// XOR the state with the round key
void ADDROUNDKEY(State& state, const KeySchedule& w, size_t startIndex) {
	for (size_t c = 0; c < 4; ++c) {
		for (size_t r = 0; r < 4; ++r) {
			state[r][c] ^= w[startIndex + c][r];
		}
	}
}

// Byte substitution using the AES S-Box
void SUBBYTES(State& state) {
	for (auto& row : state) {
		for (auto& byte : row) {
			byte = SBox[byte];
		}
	}
}

// Inverse byte substitution using the AES inverse S-Box
void INVSUBBYTES(State& state) {
	for (auto& row : state) {
		for (auto& byte : row) {
			byte = InvSBox[byte];
		}
	}
}

// Shift rows left by their index
void SHIFTROWS(State& state) {
	for (size_t r = 1; r < 4; ++r) {
		std::rotate(state[r].begin(), state[r].begin() + r, state[r].end());
	}
}

// Inverse shift rows right by their index
void INVSHIFTROWS(State& state) {
	for (size_t r = 1; r < 4; ++r) {
		std::rotate(state[r].begin(), state[r].begin() + (4 - r), state[r].end());
	}
}

Byte GF_Multiply(Byte a, Byte b) {
	Byte result = 0;
	while (b) {
		if (b & 1) result ^= a;        // Add a if the least significant bit of b is set
		bool highBitSet = a & 0x80;   // Check if the high bit is set
		a <<= 1;                      // Multiply by x
		if (highBitSet) a ^= 0x1b;    // Reduce modulo m(x) = x^8 + x^4 + x^3 + x + 1
		b >>= 1;                      // Divide b by x
	}
	return result;
}

void MIXCOLUMNS(State& state) {
	for (size_t c = 0; c < 4; ++c) {
		Byte s0 = state[0][c], s1 = state[1][c], s2 = state[2][c], s3 = state[3][c];
		state[0][c] = GF_Multiply(0x02, s0) ^ GF_Multiply(0x03, s1) ^ s2 ^ s3;
		state[1][c] = s0 ^ GF_Multiply(0x02, s1) ^ GF_Multiply(0x03, s2) ^ s3;
		state[2][c] = s0 ^ s1 ^ GF_Multiply(0x02, s2) ^ GF_Multiply(0x03, s3);
		state[3][c] = GF_Multiply(0x03, s0) ^ s1 ^ s2 ^ GF_Multiply(0x02, s3);
	}
}

void INVMIXCOLUMNS(State& state) {
	for (size_t c = 0; c < 4; ++c) {
		Byte s0 = state[0][c], s1 = state[1][c], s2 = state[2][c], s3 = state[3][c];
		state[0][c] = GF_Multiply(0x0e, s0) ^ GF_Multiply(0x0b, s1) ^ GF_Multiply(0x0d, s2) ^ GF_Multiply(0x09, s3);
		state[1][c] = GF_Multiply(0x09, s0) ^ GF_Multiply(0x0e, s1) ^ GF_Multiply(0x0b, s2) ^ GF_Multiply(0x0d, s3);
		state[2][c] = GF_Multiply(0x0d, s0) ^ GF_Multiply(0x09, s1) ^ GF_Multiply(0x0e, s2) ^ GF_Multiply(0x0b, s3);
		state[3][c] = GF_Multiply(0x0b, s0) ^ GF_Multiply(0x0d, s1) ^ GF_Multiply(0x09, s2) ^ GF_Multiply(0x0e, s3);
	}
}


Word ROTWORD(const Word& w) {
	return { w[1], w[2], w[3], w[0] };
}

Word SUBWORD(const Word& w) {
	return { SBox[w[0]], SBox[w[1]], SBox[w[2]], SBox[w[3]] };
}

KeySchedule keyExpansion(const std::array<Byte, 16>& key, size_t Nk, size_t Nr) {
	KeySchedule w(4 * (Nr + 1));
	for (size_t i = 0; i < Nk; ++i) {
		w[i] = { key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3] };
	}

	size_t rconIndex = 1;
	for (size_t i = Nk; i < 4 * (Nr + 1); ++i) {
		Word temp = w[i - 1];
		if (i % Nk == 0) {
			temp = SUBWORD(ROTWORD(temp));
			temp[0] ^= RCON[rconIndex - 1]; // Apply RCON
			rconIndex++;
		}
		w[i] = { static_cast<Byte>(w[i - Nk][0] ^ temp[0]),
				 static_cast<Byte>(w[i - Nk][1] ^ temp[1]),
				 static_cast<Byte>(w[i - Nk][2] ^ temp[2]),
				 static_cast<Byte>(w[i - Nk][3] ^ temp[3]) };
	}

	return w;
}

void CIPHER(State& state, const KeySchedule& w, size_t Nr) {
	ADDROUNDKEY(state, w, 0);

	for (size_t round = 1; round < Nr; ++round) {
		SUBBYTES(state);
		SHIFTROWS(state);
		MIXCOLUMNS(state);
		ADDROUNDKEY(state, w, 4 * round);
	}

	SUBBYTES(state);
	SHIFTROWS(state);
	ADDROUNDKEY(state, w, 4 * Nr);
}

// AES INVCIPHER function
void INVCIPHER(State& state, const KeySchedule& w, size_t Nr) {
	ADDROUNDKEY(state, w, 4 * Nr);

	for (size_t round = Nr - 1; round > 0; --round) {
		INVSHIFTROWS(state);
		INVSUBBYTES(state);
		ADDROUNDKEY(state, w, 4 * round);
		INVMIXCOLUMNS(state);
	}

	INVSHIFTROWS(state);
	INVSUBBYTES(state);
	ADDROUNDKEY(state, w, 0);
}	
std::vector<Byte> encryptCBC(const std::vector<Byte>& plaintext, const KeySchedule& keySchedule, const std::array<Byte, 16>& iv, size_t Nr) {
	State state;
	std::array<Byte, 16> prevBlock = iv;
	std::vector<Byte> ciphertext;

	for (size_t i = 0; i < plaintext.size(); i += 16) {
		std::copy(plaintext.begin() + i, plaintext.begin() + i + 16, &state[0][0]);

		// XOR with previous ciphertext block
		for (size_t r = 0; r < 4; ++r)
			for (size_t c = 0; c < 4; ++c)
				state[r][c] ^= prevBlock[r * 4 + c];

		CIPHER(state, keySchedule, Nr);

		// Save ciphertext
		for (size_t r = 0; r < 4; ++r)
			for (size_t c = 0; c < 4; ++c)
				prevBlock[r * 4 + c] = state[r][c];

		ciphertext.insert(ciphertext.end(), &state[0][0], &state[0][0] + 16);
	}

	return ciphertext;
}
std::vector<Byte> decryptCBC(const std::vector<Byte>& ciphertext, const KeySchedule& keySchedule, const std::array<Byte, 16>& iv, size_t Nr) {
	State state;
	std::array<Byte, 16> prevBlock = iv;
	std::vector<Byte> plaintext;

	for (size_t i = 0; i < ciphertext.size(); i += 16) {
		std::array<Byte, 16> currentBlock;
		std::copy(ciphertext.begin() + i, ciphertext.begin() + i + 16, &state[0][0]);
		std::copy(ciphertext.begin() + i, ciphertext.begin() + i + 16, currentBlock.begin());

		INVCIPHER(state, keySchedule, Nr);

		// XOR with previous ciphertext block
		for (size_t r = 0; r < 4; ++r)
			for (size_t c = 0; c < 4; ++c)
				state[r][c] ^= prevBlock[r * 4 + c];

		prevBlock = currentBlock;

		plaintext.insert(plaintext.end(), &state[0][0], &state[0][0] + 16);
	}

	return plaintext;
}

// Helper function to print state
void printState(const State& state) {
	for (const auto& row : state) {
		for (const auto& byte : row) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}


// Main function
int main() {
	// Example input block (128 bits represented as 4x4 matrix)
	State input = { {
		{0x32, 0x43, 0xf6, 0xa8},
		{0x88, 0x5a, 0x30, 0x8d},
		{0x31, 0x31, 0x98, 0xa2},
		{0xe0, 0x37, 0x07, 0x34}
	} };

	std::array<Byte, 16> inputKey = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
									 0xab, 0xf7, 0xcf, 0x45, 0x30, 0x4e, 0x3b, 0x76 };
	KeySchedule keySchedule = keyExpansion(inputKey, 4, 10); // Nk = 4, Nr = 10

	size_t Nr = 10; // Number of rounds for AES-128

	std::cout << "Original State:\n";
	printState(input);

	// Encrypt
	State encrypted = input;
	CIPHER(encrypted, keySchedule, Nr);
	std::cout << "\nEncrypted State:\n";
	printState(encrypted);

	// Decrypt
	State decrypted = encrypted;
	INVCIPHER(decrypted, keySchedule, Nr);
	std::cout << "\nDecrypted State:\n";
	printState(decrypted);

	// Verify if decryption matches the original
	if (input == decrypted) {
		std::cout << "\nSuccess: Decrypted state matches the original!\n";
	}
	else {
		std::cout << "\nFailure: Decrypted state does NOT match the original.\n";
	}

	return 0;
}




/*
/////////////////////////////////
string encrypt(string text)
{
	vector<bitset<8>> bits = convertInBits(text);

	bits = addRoundKey(bits);
	for (int i = 0; i < 9; i++)
	{
		bits = substituteBytes(bits);
		bits = shiftRows(bits);
		bits = mixColumns(bits);
		bits = addRoundKey(bits);
	}
	bits = substituteBytes(bits);
	bits = shiftRows(bits);
	bits = addRoundKey(bits);

	text = convertInText(bits);

	return text;
}
////////////////////////////////

vector<bitset<8>> addRoundKey(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> substituteBytes(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> shiftRows(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> mixColumns(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> convertInBits(string text)
{
	vector<bitset<8>> bitArray; // Store each character as an 8-bit binary

	for (size_t i = 0; i < text.size(); ++i) 
	{ 	// Convert each character in myString to 8-bit binary and store in bitArray
		bitset<8> bits(text[i]); // Convert each character to 8 bits
		bitArray.push_back(bits); // Store the bits in the vector
	}
	return bitArray;
}
///////////////////////////////////
string decrypt(string text)
{

	vector<bitset<8>> bits = convertInBits(text);

	bits = inverseAddRoundKey(bits);
	for (int i = 0; i < 9; i++)
	{
		bits = inverseShiftRows(bits);
		bits = inverseSubstituteBytes(bits);
		bits = inverseAddRoundKey(bits);
		bits = inverseMixColumns(bits);
	}
	bits = inverseShiftRows(bits);
	bits = inverseSubstituteBytes(bits);
	bits = inverseAddRoundKey(bits);

	text = convertInText(bits);

	return text;
}
///////////////////////////////////
vector<bitset<8>> inverseAddRoundKey(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> inverseSubstituteBytes(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> inverseShiftRows(vector<bitset<8>> bitArray)
{

	return bitArray;
}
vector<bitset<8>> inverseMixColumns(vector<bitset<8>> bitArray)
{

	return bitArray;
}
string convertInText(const vector<bitset<8>>& bitArray)
{
	string text;

	for (size_t i = 0; i < 16; ++i)
	{
		// Convert each bitset<8> to a char and add it to the text
		char character = static_cast<char>(bitArray[i].to_ulong());
		text += character;
	}

	return text;
}
*/

/*
#include <string>
#include <bitset>
#include <vector>
#include <iostream>

using namespace std;

int main() {
	string myString = "Hello World";
	vector<bitset<8>> bitArray; // Store each character as an 8-bit binary

	// Convert each character in myString to 8-bit binary and store in bitArray
	for (size_t i = 0; i < myString.size(); ++i) {
		bitset<8> bits(myString[i]); // Convert each character to 8 bits
		bitArray.push_back(bits); // Store the bits in the vector
	}

	// Output stored bits (for verification)
	cout << "Binary representation:" << endl;
	for (const auto& bits : bitArray) {
		cout << bits << " ";
	}
	cout << endl;

	// XOR each bit with 11111111 and store in a new vector
	vector<bitset<8>> xorBitArray;
	for (const auto& bits : bitArray) {
		bitset<8> xorBits = bits ^ bitset<8>("11111111"); // XOR with 11111111
		xorBitArray.push_back(xorBits); // Store the XORed bits in a new vector
	}

	// Output XORed binary values
	cout << "XORed binary representation:" << endl;
	for (const auto& xorBits : xorBitArray) {
		cout << xorBits << " ";
	}
	cout << endl;

	// Convert XORed bits back to text (encrypted text)
	string encryptedText;
	for (const auto& xorBits : xorBitArray) {
		char encryptedChar = static_cast<char>(xorBits.to_ulong()); // Convert bitset to char
		encryptedText += encryptedChar; // Append to encrypted text
	}

	// Output the encrypted text
	cout << "Encrypted text: " << encryptedText << endl;

	return 0;
}



#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main() {
	ifstream f;
	ofstream f1;
	f.open("f.txt");
	f1.open("f1.txt");
	cout << "Enter search text!" << endl;
	char* text = new char[40];
	cin >> text;

	int number = 1;
	char* lineText = new char[255];

	while (f.getline(lineText, 255))
	{
		if (strstr(lineText, text))
		{
			f1 << number << ". line: " << lineText << endl;
		}
		number++;
	}

	f.close();
	f1.close();
	delete[] text;
	delete[] lineText;
	return 0;
}



#include <iostream>

class NumberArray
{
private:
	double arr[10];

public:
	//1.
	NumberArray()
	{
		for (int i = 0; i < 10; ++i)
		{
			arr[i] = 0.0;
		}
	}
	//2.
	~NumberArray()
	{
		std::cout << "Masîvs ir izdzçsts!";
	}

	//3.
	void Change(int i, double v)
	{
		arr[i] = v;
	}
	//4.
	double Mode()
	{
		int maxCount = 0;
		double modeValue = arr[0];

		for (int i = 0; i < 10; ++i)
		{
			int count = 0;
			for (int j = 0; j < 10; ++j)
			{
				if (arr[i] == arr[j])
				{
					count++;
				}
			}
			if (count > maxCount)
			{
				maxCount = count;
				modeValue = arr[i];
			}
		}

		return modeValue;
	}
	//5.
	double Median()
	{
		double tempArr[10];
		for (int i = 0; i < 10; ++i)
		{
			tempArr[i] = arr[i];
		}
		Sort(tempArr, 10);
		return (tempArr[4] + tempArr[5]) / 2.0;
	}

	void Sort(double arr[], int size)
	{
		for (int i = 0; i < size - 1; ++i)
		{
			for (int j = 0; j < size - 1 - i; ++j)
			{
				if (arr[j] > arr[j + 1])
				{
					Swap(arr[j], arr[j + 1]);
				}
			}
		}
	}

	void Swap(double& a, double& b)
	{
		double temp = a;
		a = b;
		b = temp;
	}


	//6.
	void Print()
	{
		for (int i = 0; i < 10; ++i)
		{
			std::cout << arr[i] << std::endl;
		}
		std::cout << std::endl;
	}

	//7.
	void Initialize()
	{
		for (int i = 0; i < 10; ++i) {
			arr[i] = 0.0;
		}
	}
};

int main() 
{
	NumberArray numArr;

	numArr.Initialize();

	numArr.Change(0, 1.0);
	numArr.Change(1, 2.0);
	numArr.Change(2, 2.0);
	numArr.Change(3, 3.0);
	numArr.Change(4, 4.0);
	numArr.Change(5, 5.0);
	numArr.Change(6, 6.0);
	numArr.Change(7, 6.0);
	numArr.Change(8, 7.0);
	numArr.Change(9, 8.0);

	std::cout << "Elementi: " << std::endl;
	numArr.Print();

	std::cout << "Moda: " << numArr.Mode() << std::endl;
	std::cout << "Mediâna: " << numArr.Median() << std::endl;

	return 0;
}



#include <iostream>
using namespace std;

struct number
{
	int value;
	number* next;
};

void removeLarge(number*& start, int limit)
{
	if (start == NULL) return;

	number* current = start;
	number* previous = NULL;

	while (current != NULL)
	{
		if (current->value > limit)
		{
			number* toDelete = current;

			if (previous == NULL)
			{
				start = current->next;
			}
			else
			{
				previous->next = current->next;
			}

			current = current->next;
			delete toDelete;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}


int main() 
{
	cout << "How many elements in the list?" << endl;
	int size;
	cin >> size;
	cout << "What is the max limit for elements?" << endl;
	int limit;
	cin >> limit;
	number* start = NULL, * last = NULL;
	for (int i = 0; i < size; i++) {
		cout << "Enter " << i + 1
			<< ". element: ";
		number* numb = new number;
		cin >> numb->value;
		numb->next = NULL;
		if (last != NULL) {
			last->next = numb;
			last = numb;
		}
		else start = last = numb;
	}
	removeLarge(start, limit);
	cout << "After discarding the largest elements the list looks like this:" << endl;
	number* p = start;
	while (p != NULL) {
		cout << p->value << endl;
		p = p->next;
	}
	while (start != NULL) {
		number* temp = start;
		start = start->next;
		delete temp;
	}
	return 0;
}




#include <iostream>
using namespace std;

int main()
{
    struct MyStruct
    {
        int number;
        MyStruct* next;
    };

    cout << "How many structures?" << endl;
    int times;
    cin >> times;

    MyStruct* first = NULL;
    MyStruct* last = NULL;

	cout << "Enter numbers!" << endl;
	for (int i = 0; i < times; i++)
	{
		// Dynamically allocate a new MyStruct instance
		MyStruct* temp = new MyStruct;
		cin >> temp->number;
		temp->next = NULL; // Initialize next pointer

		if (first == NULL)
		{
			first = temp; // Assign first node
			last = temp;  // Last also points to the first node initially
		}
		else
		{
			last->next = temp; // Link the last node to the new node
			last = temp;       // Move the last pointer to the new node
		}
	}
    // Display the numbers entered so far
    cout << "Your numbers:" << endl;
    MyStruct* temp2 = first;
    while (temp2 != NULL)
    {
        cout << temp2->number << endl;
        temp2 = temp2->next;
    }

    // Free the allocated memory
    while (first != NULL)
    {
        MyStruct* temp3 = first;
        first = first->next;
        delete temp3;
    }

    return 0;
}







#include <iostream>
#include <fstream>
using namespace std;
int main() {
	ifstream inputFile("aaa.txt", ios::in);
	char c;
	int len = 0;
	inputFile.get(c);
	while (inputFile) {
		cout << c;
		len++;
		inputFile.get(c);
	}
	inputFile.close();
	cout << endl << "Length of file is " << len << " characters\n";
	return 0;
}




#include <iostream>
using namespace std;

int main() {
	char c[255], uzvards[255];
	int count = 0;

	cout << "Ievadi masîva c burtus: ";
	cin.getline(c, 255);

	cout << "Ievadi savu uzvârdu: ";
	cin.getline(uzvards, 255);

	for (int i = 0; i < strlen(c); i++)
	{
		for (int j = 0; j < strlen(uzvards); i++)
		{
			if (c[i] == c[j])
			{
				count++;
			}
		}
	}

	cout << "Masîvam c un tavam uzvârdam ir " << count << " kopîgi burti!" << endl;

	return 0;
}

#include <iostream>
using namespace std;


int main()
{
	int a;
	int b;
	int count = 0;
	cout << "Ievadi naturâlus skaitïus a un b, kur a <= b" << endl;
	cin >> a >> b;

	cout << "Nepâra skaitïi, kas ir intervâlâ [a;b] un nedalâs ar 3:" << endl;
	for (int i = a; i <= b; i++)
	{
		if (i % 2 != 0 && i % 3 != 0)
		{
			count++;
			cout << i << endl;
		}
	}
	cout << "Kopâ: " << count << endl;

	return 0;
}









#include <iostream>
using namespace std;

long calculateFibonacci(long n);

int main()
{
	// Elementa n jautâðana lietotâjam un ievadîtâs vçrtîbas pârbaude

	long n;

	cout << "Ievadi, kuru Fibonaèi sakarîbas elementu vçlies atrast! (Naturâlu skaitli)" << endl;
	cin >> n;
	while (!cin.good() || n < 1 || cin.peek() != '\n')
	{
		cout << "Nederîga ievadi, mçìini vçlreiz!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> n;
	}

	// Rezultâta izvadîðana (Rezultâts tiek aprçíinâts funkcijâ calculateFibonacci(n))
	cout << "Fibonaèi sakarîbas " << n << ". elements ir: " << calculateFibonacci(n) << endl;

	return 0;
}

//Rezultâta aprçíinâðana atkarîbâ no n
long calculateFibonacci(long n)
{
	//Pirmâs divas vçrtîbas Fibonaèi virknç ir vienâdas ar 1
	if (n == 1 || n == 2) 
	{
		return 1;
	}
	//Rezultâta apºçíinâðana, ja n > 2
	long a = 1, b = 1, fib = 0;

	for (int i = 3; i <= n; ++i) {
		fib = a + b;
		a = b;
		b = fib;
	}
	return fib;
}






#include <iostream>
using namespace std;

int main()
{
	int n;
	int *array;
	int smallest;
	int largest;

	// Masîva garuma ievade un korektas vçrtîbas pârbaude
	cout << "Enter half-size of the array! (whole number 1-99)" << endl;
	cin >> n;
	n *= 2;
	array = new int[n];

	// Masîva vçrtîbu ievade un pârbaude
	cout << "Enter (" << n << ") array values" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> array[i];
	}

	// Largest vçrtîbas atraðana
	for (int i = 0; i < n / 2; i++)
	{
		int temp = array[i] + array[n - 1 - i];
		if (i == 0 || temp > largest)
		{
			largest = temp;
		}
	}

	// Smallest vçrtîbas atraðana
	for (int i = 0; i < n / 2; i++)
	{
		int temp = array[i] * array[n / 2 + i];
		if (i == 0 || temp < smallest)
		{
			smallest = temp;
		}
	}

	cout << "The largest value from the given sequence is: " << endl << largest << endl;
	cout << "The smallest value from the given sequence is: " << endl << smallest << endl;

	delete[] array;

	return 0;
}

//2. uzdevums


#include <iostream>
using namespace std;

int main()
{
	int n;
	double *aArray;
	double *bArray;

	// Masîva garuma ievade un korektas vçrtîbas pârbaude
	cout << "Enter how long the arrays will be! (whole number 1-99)" << endl;
	cin >> n;
	while (!cin.good() || n < 1 || n > 99 || cin.peek() != '\n')
	{
		cout << "Invalid input! Try again!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> n;
	}
	aArray = new double[n];
	bArray = new double[n];

	// aArray vçrtîbu ievade
	cout << "Enter any real numbers for the aArray!" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> aArray[i];
		if (!cin.good())
		{
			cout << "Invalid input! Try again!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			i = -1;
		}
	}

	// bArray vçrtîbu ievade
	cout << "Enter any real numbers for the bArray!" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> bArray[i];
		if (!cin.good())
		{
			cout << "Invalid input! Try again!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			i = -1;
		}
	}

	// Jauno vçrtîbu pieðíirðana
	for (int i = 0; i < n; i++)
	{
		if (aArray[i] < 0 && bArray[i] < 0)
		{
			aArray[i] += 0.5;
			bArray[i] += 0.5;
		}
		else if (aArray[i] < 0 && bArray[i] > 0)
		{
			aArray[i] *= aArray[i];
		}
		else if (aArray[i] > 0 && bArray[i] < 0)
		{
			bArray[i] *= bArray[i];
		}
		else if (aArray[i] > 0 && bArray[i] > 0)
		{
			aArray[i] = bArray[i] = (aArray[i] + bArray[i]) / 2;
		}
	}

	// Jauno vçrtîbu izvade abiem masîviem
	cout << "The new aArray values are:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << aArray[i] << endl;
	}

	cout << "The new bArray values are:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << bArray[i] << endl;
	}

	delete [] aArray;
	delete [] bArray;
	return 0;
}
*/


