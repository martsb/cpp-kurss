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


