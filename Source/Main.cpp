#include <iostream>

using namespace std;

void game();
void map_editor();

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
		case 1:game();
			break;
		case 2:map_editor();
			break;
		case 3:return;
		default: cout << "Enter valid Choice\n";
		}
	} while (ch != 3);
}


int main()
{
	map_editor();
	return 0;
}