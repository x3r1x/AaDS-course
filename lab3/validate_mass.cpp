//
// Created by x3r1x on 10/22/26.
//

/*

В  листьях  И-ИЛИ  дерева,  соответствующего некоторому
множеству  конструкций,  заданы   значения   массы.   Известно
максимально допустимое значение массы изделия. Требуется усечь
дерево   так,   чтобы   дерево    включало    все    элементы,
соответствующие  допустимым  значениям массы,  но не содержало
"лишних" вершин.  Конечное дерево выдать на экран в  наглядном
виде (12).

Автор: Рыжов Глеб Игоревич

Среда выполнения: CLion, g++

Источники:
1. Презентация Галочкина Владимира Ивановича: https://docs.google.com/presentation/d/13h_tcxfONXDOPFAaH2P4Yo9fjCwQ13x0/edit?slide=id.p2#slide=id.p2

*/

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

enum class NodeType
{
	AND = 'a',
	OR = 'o',
	LEAF = 'l'
};

struct Node
{
	char name;
	NodeType type;
	vector<unique_ptr<Node>> children;

	int mass;
	int minMass;
	int maxMass;
	bool isCorrect;
};

constexpr int MAX_MASS = 5000;

NodeType TransformToNodeType(const char type)
{
	return static_cast<NodeType>(type);
}

char TransfromToChar(NodeType type)
{
	return static_cast<char>(type);
}

unique_ptr<Node> ReadTree(ifstream& stream)
{
	unique_ptr<Node> root;
	vector<Node*> path;
	string line;

	while (getline(stream, line))
	{
		int depth = 0;

		while (line[depth] == '.')
		{
			++depth;
		}

		const char name = line[depth];

		const string lineWithMass = line.substr(depth + 2);
		const int mass = stoi(lineWithMass);
		const char type = line[line.length() - 1];

		auto newNode = std::make_unique<Node>(Node{
			.name = name,
			.type = TransformToNodeType(type),
			.mass = mass,
			.minMass = 0,
			.maxMass = 0 });

		const auto currentNode = newNode.get();

		if (depth == 0)
		{
			if (root)
			{
				cerr << "Root exists!" << endl;
				return nullptr;
			}

			root = move(newNode);
		}
		else
		{
			path[depth - 1]->children.push_back(move(newNode));
		}

		path.resize(depth);
		path.push_back(currentNode);
	}

	return root;
}

void PrintTree(const unique_ptr<Node>& node, const int depth)
{
	string depthString;

	for (int i = 0; i < depth; i++)
	{
		depthString += '.';
	}

	const auto currentNode = node.get();

	cout << depthString << currentNode->name << " " << currentNode->mass << " " << TransfromToChar(currentNode->type) << endl;

	for (auto& child : currentNode->children)
	{
		PrintTree(child, depth + 1);
	}
}

bool CalculateMasses(const unique_ptr<Node>& node);

bool CalculateLeafMass(const unique_ptr<Node>& leafNode)
{
	leafNode->minMass = leafNode->mass;
	leafNode->maxMass = leafNode->mass;
	leafNode->isCorrect = true;
	return true;
}

bool CalculateAndMass(const unique_ptr<Node>& andNode)
{
	for (auto& child: andNode->children)
	{
		if (!CalculateMasses(child))
		{
			andNode->isCorrect = false;
			return false;
		}

		andNode->minMass += child->minMass;
		andNode->maxMass += child->maxMass;
	}

	andNode->isCorrect = true;
	return true;
}

bool CalculateOrMass(const unique_ptr<Node>& orNode)
{
	orNode->minMass = MAX_MASS;
	orNode->maxMass = 0;
	bool found = false;

	for (auto& child: orNode->children)
	{
		if (CalculateMasses(child))
		{
			found = true;
			orNode->minMass = min(orNode->minMass, child->minMass);
			orNode->maxMass = max(orNode->maxMass, child->maxMass);
		}
	}

	orNode->isCorrect = found;
	return found;
}

bool CalculateMasses(const unique_ptr<Node>& node)
{
	if (node->type == NodeType::LEAF)
	{
		return CalculateLeafMass(node);
	}

	if (node->type == NodeType::AND)
	{
		return CalculateAndMass(node);
	}

	if (node->type == NodeType::OR)
	{
		return CalculateOrMass(node);
	}

	return false;
}

bool ValidateMassTree(const unique_ptr<Node>& node, int maxNeededMass);

bool ValidateLeafNode(const unique_ptr<Node>& leafNode, const int maxNeededMass)
{
	return leafNode->mass <= maxNeededMass;
}

bool ValidateOrNode(const unique_ptr<Node>& orNode, const int maxNeededMass)
{
	auto child = orNode->children.begin();

	while (child != orNode->children.end())
	{
		if (ValidateMassTree(*child, maxNeededMass))
		{
			++child;
		}
		else
		{
			child = orNode->children.erase(child);
		}
	}

	return !orNode->children.empty();
}

bool ValidateAndNode(const unique_ptr<Node>& andNode, const int maxNeededMass)
{
	for (const auto& child : andNode->children)
	{
		const int otherChildrenMinimum = andNode->minMass - child->minMass;
		int thisChileMaxNeededMass = maxNeededMass - otherChildrenMinimum;

		if (!ValidateMassTree(child, thisChileMaxNeededMass))
		{
			return false;
		}
	}

	return true;
}


bool ValidateMassTree(const unique_ptr<Node>& node, const int maxNeededMass)
{
	if (!node->isCorrect || node->minMass > maxNeededMass)
	{
		return false;
	}

	if (node->maxMass <= maxNeededMass)
	{
		return true;
	}

	if (node->type == NodeType::LEAF)
	{
		return ValidateLeafNode(node, maxNeededMass);
	}

	if (node->type == NodeType::OR)
	{
		return ValidateOrNode(node, maxNeededMass);
	}

	if (node->type == NodeType::AND)
	{
		return ValidateAndNode(node, maxNeededMass);
	}

	return false;
}

int main(int _, char* argv[])
{
	ifstream inputFile(argv[1]);

	if (!inputFile.is_open())
	{
		cerr << "Error opening file!" << endl;
		return 1;
	}

	const auto root = ReadTree(inputFile);
	CalculateMasses(root);

	int maxMass;

	cin >> maxMass;

	if (ValidateMassTree(root, maxMass))
	{
		PrintTree(root, 0);
	}
}