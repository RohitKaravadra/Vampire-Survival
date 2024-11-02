#include <iostream>

using namespace std;

void game();

enum choice
{
	GAME = 1,
	LEVEL_EDITOR,
	EXIT
};

void menu()
{
	int ch;
	do
	{
		cout << "\n\n1. Game\n2. Level Editor\n3. Exit\nInput : ";
		cin >> ch;
		switch (ch)
		{
		case choice::GAME:game();
			break;
		case choice::LEVEL_EDITOR:
			break;
		case choice::EXIT:return;
		default: cout << "Enter valid Choice\n";
		}
	} while (ch != choice::EXIT);
}


int main()
{
	game();
	return 0;
}