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
#include <limits.h>
#include <vector>

using namespace std;


constexpr int MAX_EDGE_AMOUNT = 100;

struct Edge
{
	int to;
	int weight;
};

struct Vertex
{
	vector<Edge> edges;
};

static void FindSecondShortestPath(const vector<Vertex>& graph, int startVertex)
{
	vector<int> distance1(graph.size(), INT_MAX);
	vector<int> distance2(graph.size(), INT_MAX);
	distance1[startVertex] = 0;

	//         distance
	vector<pair<int, int>> queue;
	//				 number_of_city


	queue.emplace_back(0, startVertex);

	while (!queue.empty())
	{
		int minIndex = 0;

		for (int i = 0; i < queue.size(); i++)
		{
			if (queue[i].first < queue[minIndex].first)
			{
				minIndex = i;
			}
		}

		auto info = queue[minIndex];

		queue.erase(queue.begin() + minIndex);

		if (info.first > distance2[info.second])
		{
			continue;
		}

		for (const Edge& edge: graph[info.second].edges)
		{
			int newDistance = info.first + edge.weight;

			if (newDistance < distance1[edge.to])
			{
				distance2[edge.to] = distance1[edge.to];
				distance1[edge.to] = newDistance;

				queue.emplace_back(newDistance, edge.to);
			}
			else if (newDistance < distance2[edge.to])
			{
				distance2[edge.to] = newDistance;

				queue.emplace_back(newDistance, edge.to);
			}
		}
	}

	for (int i = 0; i < graph.size(); i++)
	{
		if (distance2[i] != INT_MAX)
		{
			cout << "Vertex " << i << " distance 1 " << distance1[i] << "; distance 2 " << distance2[i] << ";" << endl;
		}
	}
}

static vector<Vertex> GetVertexFromFile(ifstream& stream)
{
	vector<Vertex> edges;
	edges.reserve(MAX_EDGE_AMOUNT);

	while (!stream.eof())
	{
		int startEdge;
		int endEdge;
		int weight;

		stream >> startEdge >> endEdge >> weight;

		if (startEdge >= edges.size())
		{
			edges.resize(startEdge + 1);
		}

		edges[startEdge].edges.push_back({
			endEdge,
			weight
		});
	}

	return edges;
}

static void PrintGraph(const vector<Vertex>& edges)
{
	int i = 0;

	for (const auto& edge: edges)
	{
		cout << "Edge " << i << ": ";

		for (auto target: edge.edges)
		{
			cout << target.to << " goes by " << target.weight << "; ";
		}

		cout << endl;
		i++;
	}
}

int main(int _, char* argv[])
{
	ifstream inputFile(argv[1]);

	if (!inputFile.is_open())
	{
		cerr << "Error opening file: " << argv[1] << endl;
		return 1;
	}

	const vector<Vertex> vertex = GetVertexFromFile(inputFile);

	int capital;
	cout << "Введите номер вершины, которая является столицей" << endl;
	cin >> capital;

	FindSecondShortestPath(vertex, capital);
}