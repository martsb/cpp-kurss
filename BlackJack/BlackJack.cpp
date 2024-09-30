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


/*

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

	cout << "Ievadi mas�va c burtus: ";
	cin.getline(c, 255);

	cout << "Ievadi savu uzv�rdu: ";
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

	cout << "Mas�vam c un tavam uzv�rdam ir " << count << " kop�gi burti!" << endl;

	return 0;
}

#include <iostream>
using namespace std;


int main()
{
	int a;
	int b;
	int count = 0;
	cout << "Ievadi natur�lus skait�us a un b, kur a <= b" << endl;
	cin >> a >> b;

	cout << "Nep�ra skait�i, kas ir interv�l� [a;b] un nedal�s ar 3:" << endl;
	for (int i = a; i <= b; i++)
	{
		if (i % 2 != 0 && i % 3 != 0)
		{
			count++;
			cout << i << endl;
		}
	}
	cout << "Kop�: " << count << endl;

	return 0;
}









#include <iostream>
using namespace std;

long calculateFibonacci(long n);

int main()
{
	// Elementa n jaut��ana lietot�jam un ievad�t�s v�rt�bas p�rbaude

	long n;

	cout << "Ievadi, kuru Fibona�i sakar�bas elementu v�lies atrast! (Natur�lu skaitli)" << endl;
	cin >> n;
	while (!cin.good() || n < 1 || cin.peek() != '\n')
	{
		cout << "Neder�ga ievadi, m��ini v�lreiz!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> n;
	}

	// Rezult�ta izvad��ana (Rezult�ts tiek apr��in�ts funkcij� calculateFibonacci(n))
	cout << "Fibona�i sakar�bas " << n << ". elements ir: " << calculateFibonacci(n) << endl;

	return 0;
}

//Rezult�ta apr��in��ana atkar�b� no n
long calculateFibonacci(long n)
{
	//Pirm�s divas v�rt�bas Fibona�i virkn� ir vien�das ar 1
	if (n == 1 || n == 2) 
	{
		return 1;
	}
	//Rezult�ta ap���in��ana, ja n > 2
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

	// Mas�va garuma ievade un korektas v�rt�bas p�rbaude
	cout << "Enter half-size of the array! (whole number 1-99)" << endl;
	cin >> n;
	n *= 2;
	array = new int[n];

	// Mas�va v�rt�bu ievade un p�rbaude
	cout << "Enter (" << n << ") array values" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> array[i];
	}

	// Largest v�rt�bas atra�ana
	for (int i = 0; i < n / 2; i++)
	{
		int temp = array[i] + array[n - 1 - i];
		if (i == 0 || temp > largest)
		{
			largest = temp;
		}
	}

	// Smallest v�rt�bas atra�ana
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

	// Mas�va garuma ievade un korektas v�rt�bas p�rbaude
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

	// aArray v�rt�bu ievade
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

	// bArray v�rt�bu ievade
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

	// Jauno v�rt�bu pie��ir�ana
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

	// Jauno v�rt�bu izvade abiem mas�viem
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


