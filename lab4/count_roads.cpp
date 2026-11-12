//
// Created by x3r1x on 11/6/26.
//

/*

29. Имеется сеть автомобильных дорог. По  некоторым дорогам
можно  проехать  только  в одном  направлении. Известна  длина
каждой дороги, причем она может быть разной в  зависимости  от
направления. Один  из  городов  является  столицей.  Требуется
вывести список вторых по минимальности путей из столицы в другие
города. Допускается присутствие циклических путей. Второй путь
может быть одинаковой длины с первым (12).

Автор: Рыжов Глеб Игоревич

Среда выполнения: CLion, g++

Источники:
1. Презентация Галочкина Владимира Ивановича - https://docs.google.com/presentation/d/1JVs6F_JKClJ36kgMVMK0Au0RTWebMWyP/edit?slide=id.p16#slide=id.p16
2.

*/

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct EdgeTarget
{
	int target;
	int weight;
};

struct Edge
{
	vector<EdgeTarget> targets;
};

vector<Edge> GetEdgesFromFile(ifstream& stream)
{
	vector<Edge> edges;
	edges.reserve(100);

	while (!stream.eof())
	{
		int startEdge;
		int endEdge;
		int weight;

		stream >> startEdge >> endEdge >> weight;

		edges[startEdge].targets.push_back({
			endEdge,
			weight
		});
	}

	return edges;
}

int main(int _, char* argv[])
{
	ifstream inputFile(argv[1]);

	if (!inputFile.is_open())
	{
		cerr << "Error opening file: " << argv[1] << endl;
		return 1;
	}

	int i = 0;

	vector<Edge> edges = GetEdgesFromFile(inputFile);

	for (const auto& edge: edges)
	{
		cout << "Edge " << i << ": ";

		for (auto target: edge.targets)
		{
			cout << target.target << "goes by" << target.weight << "; ";
		}

		cout << endl;
	}
}