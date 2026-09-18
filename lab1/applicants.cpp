//
// Created by x3r1x on 9/4/26.
//

/*
Вступительные испытания в некоторый вуз состоят из  трех
экзаменов:  математика  (максимальный  балл  – 9),  информатика
(максимальный балл – 9), литература  (максимальный балл – 5). В
текстовом входе имеются  сведения  о  сдаче  абитуриентами  этих
экзаменов. Строки файла имеют следующий формат:
<Фамилия> <оценка1> <оценка2> <оценка3>, где <Фамилия> – строка,
состоящая не более, чем из 20 символов, оценки – числа от 0  до
максимальной оценки по предмету соответственно. Ноль ставится в
случае, если экзамен не сдавался, например, после полученной на
предыдущем экзамене двойки. Все  баллы,  большие  2,  считаются
удовлетворительными). Пример входных строк:
Иванов 8 9 3
Петров 2 0 0
Задается количество мест K,  на которые претендуют абитуриенты.
Требуется определить список абитуриентов с полупроходным баллом
или  сообщить,  что  такой   балл   отсутствует.  Полупроходным
считается такой балл, что лишь  часть  абитуриентов,  набравших
этот балл и не получивших ни одной неудовлетворительной оценки,
попадает в K лучших, которые должны быть зачислены (8).

Автор: Рыжов Глеб Игоревич

Среда выполнения: CLion, g++
*/


#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

constexpr string HELP_MARKER = "-h";
constexpr int8_t NOT_A_DIGIT = -1;
constexpr int8_t OVERFLOW = -2;

typedef struct
{
	string surname;
	int mathsMark;
	int informaticsMark;
	int literatureMark;
	float averageMark;
} ApplicantResult;

static string ReadString(ifstream& stream)
{
	string newString;
	char ch;
	stream.get(ch);

	while (ch != ' ' && ch != '\n' && ch != EOF)
	{
		if (stream.eof())
		{
			return newString;
		}

		newString += ch;
		stream.get(ch);
	}

	return newString;
}

static int charToDigit(const signed char ch)
{
	switch (ch)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return NOT_A_DIGIT;
	}
}

static bool CheckForOverflow(const int currentNumber, const int digit)
{
	return currentNumber > numeric_limits<int>::max() / 10 || currentNumber * 10 > numeric_limits<int>::max() - digit;
}

static int StrToInt(const string& str)
{
	int num = 0;

	for (const signed char letter : str)
	{
		const int digit = charToDigit(letter);

		if (digit == NOT_A_DIGIT)
		{
			return NOT_A_DIGIT;
		}

		if (CheckForOverflow(num, digit))
		{
			return OVERFLOW;
		}

		num = num * 10 + digit;
	}

	return num;
}

static void GatherInformation(ApplicantResult& result, ifstream& stream)
{
	result.surname = ReadString(stream);

	const string mathsMark = ReadString(stream);
	result.mathsMark = StrToInt(mathsMark);

	const string informaticsMark = ReadString(stream);
	result.informaticsMark = StrToInt(informaticsMark);

	const string literatureMark = ReadString(stream);
	result.literatureMark = StrToInt(literatureMark);

	result.averageMark = static_cast<float>(result.mathsMark + result.informaticsMark + result.literatureMark) / 3;
}

static bool resultComparator(const ApplicantResult& a, const ApplicantResult& b)
{
	if (a.mathsMark <= 2 || a.informaticsMark <= 2 || a.literatureMark <= 2)
	{
		return false;
	}

	if (b.mathsMark <= 2 || b.informaticsMark <= 2 || b.literatureMark <= 2)
	{
		return true;
	}

	return a.averageMark > b.averageMark;
}

static bool DoesScoreExist(const vector<ApplicantResult>* results, const int K)
{
	if (K >= results->size())
	{
		return false;
	}

	int passed = 0;

	for (const ApplicantResult& result: *results)
	{
		if (result.mathsMark <= 2 || result.informaticsMark <= 2 || result.literatureMark <= 2)
		{
			break;
		}

		passed++;

		if (passed == K)
		{
			break;
		}
	}

	return (*results)[passed].averageMark == (*results)[passed - 1].averageMark;
}

static void PrintPivotedList(const vector<ApplicantResult>* results, const int start, const int end)
{
	for (int i = start; i <= end; i++)
	{
		const ApplicantResult& result = (*results)[i];

		cout << result.surname << " " << result.averageMark << endl;
	}
}

static void PrintCandidatesList(const vector<ApplicantResult>* results, const int K)
{
	int start = K;
	int end = K;

	while (start - 1 >= 0 && (*results)[start - 1].averageMark == (*results)[K].averageMark)
	{
		start--;
	}

	while (end + 1 <= results->size() && (*results)[end + 1].averageMark == (*results)[K].averageMark)
	{
		end++;
	}

	PrintPivotedList(results, start, end);
}

static void CountPasses(ifstream& stream, int K)
{
	vector<ApplicantResult> results;

	while (!stream.eof())
	{
		ApplicantResult result;

		GatherInformation(result, stream);

		results.push_back(result);
	}

	ranges::sort(results, resultComparator);

	if (DoesScoreExist(&results, K))
	{
		PrintCandidatesList(&results, K);
	}
	else
	{
		cout << "Не существует полупроходного балла!" << endl;
	}
}

static void PrintHelpInfo()
{
	cout << "Использование программы: "
		 "\n\t./applicants <имя файла> <К>"
		 "\n\t<имя файла> - название обходимого файла"
		 "\n\t<К> - число мест, на которые претендуют абитуриенты\n";
}

static bool IsInputCorrect(const int argc, char** argv[])
{
	if (argc == 3 && StrToInt((*argv)[2]) != NOT_A_DIGIT)
	{
		return true;
	}

	if (argc == 2 && (*argv)[1] == HELP_MARKER)
	{
		PrintHelpInfo();
		return false;
	}

	cerr << "Неправильное использование программы!\n./applicants -h для помощи" << endl;
	return false;
}

int main(const int argc, char* argv[])
{
	if (!IsInputCorrect(argc, &argv))
	{
		return 0;
	}

	ifstream inputFile(argv[1]);
	if (!inputFile.is_open())
	{
		cerr << "Error opening file: " << argv[1] << endl;
		return 0;
	}

	const int K = StrToInt(argv[2]);

	CountPasses(inputFile, K);

	inputFile.close();
}