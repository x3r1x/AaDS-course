//
// Created by x3r1x on 9/21/26.
//

/*
Организовать  в основной  памяти  с помощью указателей
стек из стеков. Обеспечить операции ведения внутреннего стека
из вершины внешнего стека, расширения и  сокращения  внешнего
стека, выдачи содержимого содержимого всей структуры (9)

Автор: Рыжов Глеб Игоревич

Среда выполнения: CLion, g++

Источники:
1. Презентация Галочкина Владимира Ивановича - https://docs.google.com/presentation/d/1Z98KHqSmGZsxG8AVDNnvIcsyQfeGUrfY/edit?slide=id.p3#slide=id.p3
2. Тред на stackoverflow - https://ru.stackoverflow.com/questions/857066/%D0%9E%D0%B1%D1%8A%D1%8F%D0%B2%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5-string-%D0%B2-switch-%D0%BF%D1%80%D0%B8%D0%B2%D0%BE%D0%B4%D0%B8%D1%82-%D0%BA-%D0%BE%D1%88%D0%B8%D0%B1%D0%BA%D0%B5-cannot-jump-from-switch-statement

*/

#include <iostream>
using namespace std;

constexpr int PUSH_TO_STACK = 1;
constexpr int POP_FROM_STACK = 2;
constexpr int PRINT_STACK = 3;
constexpr int DELETE_STACK = 4;
constexpr int HELP = 5;
constexpr int EXIT = 6;

struct StackStruct
{
	StackStruct* next;
	int value;
};

struct HyperStackStruct
{
	HyperStackStruct* next;
	StackStruct* stack;
};

static void Push(StackStruct*& head, const int elem)
{
	StackStruct* newElement = new StackStruct;
	newElement->value = elem;
	newElement->next = head;
	head = newElement;
}

static int Pop(StackStruct*& head)
{
	const StackStruct* previousHead = head;
	const int poppedValue = previousHead->value;

	head = head->next;
	delete previousHead;

	return poppedValue;
}

static void PrintStack(const StackStruct* head)
{
	const StackStruct* element = head;

	while (element)
	{
		cout << element->value << " ";
		element = element->next;
	}

	cout << endl;
}

static void ClearStack(const StackStruct* head)
{
	while (head)
	{
		const StackStruct* element = head;
		head = head->next;
		delete element;
	}

	head = nullptr;
}

static void PushToHyper(HyperStackStruct*& head, StackStruct* stack)
{
	HyperStackStruct* newStack = new HyperStackStruct;
	newStack->next = head;
	newStack->stack = stack;
	head = newStack;
}

static StackStruct* PopFromHyper(HyperStackStruct*& head)
{
	const HyperStackStruct* previousHead = head;
	StackStruct* poppedStack = head->stack;

	head = head->next;
	delete previousHead;

	return poppedStack;
}

static void PrintHyperStack(const HyperStackStruct* head)
{
	const HyperStackStruct* element = head;

	while (element)
	{
		PrintStack(element->stack);
		element = element->next;
	}
}

static void ClearHyperStack(HyperStackStruct*& head)
{
	while (head)
	{
		const HyperStackStruct* element = head;
		head = head->next;
		ClearStack(element->stack);
		delete element;
	}
}

static void PrintMainHelp()
{
	cout << "1. Добавление в киберстек" << endl
		 << "2. Достать из киберстека" << endl
		 << "3. Печать киберстека" << endl
		 << "4. Удаление киберстека" << endl
		 << "5. Помощь" << endl
		 << "6. Выход" << endl;
}

static void PrintStackHelp()
{
	cout << "1. Включение в стек" << endl
		 << "2. Достать из стека" << endl
		 << "3. Печать стека" << endl
		 << "4. Удаление стека" << endl
		 << "5. Помощь" << endl
		 << "6. Выход из создания стека" << endl;
}

static StackStruct* CreateNewStack()
{
	StackStruct* top = nullptr;
	int answer = 1;

	PrintStackHelp();

	while (answer != 6)
	{
		cin >> answer;

		switch (answer)
		{
		case PUSH_TO_STACK:
			int value;
			cout << "Введите число" << endl;
			cin >> value;
			Push(top, value);
			break;
		case POP_FROM_STACK:
			if (top)
			{
				const int poppedValue = Pop(top);
				cout << "Вы достали " << poppedValue << endl;
			}
			else
			{
				cout << "Стэк пуст!" << endl;
			}

			break;
		case PRINT_STACK:
			if (top)
			{
				PrintStack(top);
			}
			else
			{
				cout << "Стэк пуст!" << endl;
			}

			break;
		case DELETE_STACK:
			if (top)
			{
				ClearStack(top);
				cout << "Стэк очищен!" << endl;
			}
			else
			{
				cout << "Стэк пуст!" << endl;
			}

			break;
		case HELP:
			PrintStackHelp();
			break;
		default:
			return top;
		}
	}

	return top;
}

int main()
{
	HyperStackStruct* hyperTop = nullptr;
	int answer = 0;

	PrintMainHelp();

	while (answer != 6)
	{
		cin >> answer;

		switch (answer)
		{
		case PUSH_TO_STACK:
		{
			cout << "Создайте новый стэк!" << endl;

			StackStruct* newStack = CreateNewStack();

			PushToHyper(hyperTop, newStack);

			PrintMainHelp();
			break;
		}
		case POP_FROM_STACK:
			if (hyperTop)
			{
				const StackStruct* poppedStack = PopFromHyper(hyperTop);

				cout << "Вы достали стек: ";
				PrintStack(poppedStack);
			}
			else
			{
				cout << "Киберстек пуст!" << endl;
			}

			break;
		case PRINT_STACK:
			if (hyperTop)
			{
				PrintHyperStack(hyperTop);
			}
			else
			{
				cout << "Киберстек пуст!" << endl;
			}

			break;
		case DELETE_STACK:
			if (hyperTop)
			{
				ClearHyperStack(hyperTop);
				cout << "Киберстек очищен!" << endl;
			}
			else
			{
				cout << "Киберстек пуст!" << endl;
			}

			break;
		case HELP:
			PrintMainHelp();
			break;
		default:
			return 0;
		}
	}
}