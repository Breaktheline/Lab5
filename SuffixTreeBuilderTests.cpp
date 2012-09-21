#include "gtest/gtest.h"
#include "Common.h"
#include "SuffixTreeBuilder.h"
#include "SuffixTree.h"
#include "UnitTestsHelper.h"

void CheckVertex();

//void CheckArch(Arch* arch, bool isLeaf, int startIndex, int finishIndex, 
//	Vertex* prevVertex, Vertex* nextVertex, int archNumber, int lastAddedIndex)
//{
//	ASSERT_TRUE(NULL != arch);
//	ASSERT_EQ(isLeaf, arch->IsLeaf());
//	ASSERT_EQ(startIndex, arch->startIndex);
//	ASSERT_EQ(finishIndex, arch->GetFinishIndex(lastAddedIndex));
//	ASSERT_EQ(prevVertex, arch->prevVertex);
//	ASSERT_EQ(nextVertex, arch->nextVertex);
//	ASSERT_EQ(archNumber, arch->GetNumber());
//}
//
//void CheckLeafArch(Arch* arch, int startIndex, int finishIndex, Vertex* prevVertex, int archNumber,  int lastAddedIndex)
//{
//	CheckArch(arch, true, startIndex, finishIndex, prevVertex, NULL, archNumber, lastAddedIndex);
//}
//
//void CheckNotLeafArch(Arch* arch, int startIndex, int finishIndex, Vertex* prevVertex, Vertex* nextVertex, int lastAddedIndex)
//{
//	CheckArch(arch, false, startIndex, finishIndex, prevVertex, nextVertex, -1, lastAddedIndex);
//}

void CheckVertex(Vertex* vertex, bool isRoot, Arch* incomeArch, Vertex* linkedVertex, int outcomeArchCount)
{
	ASSERT_EQ(isRoot, vertex->IsRoot());
	ASSERT_EQ(incomeArch, vertex->GetIncomeArch());
	ASSERT_EQ(linkedVertex, vertex->GetLinkedVertex());
	ASSERT_EQ(outcomeArchCount, vertex->GetOutcomeArchesCount());
}

TEST(BuildTree, CanBuildOneCharTree)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("a", 1);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();

	//Asserts
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 2);

	//Arch** outcomeArches = root->GetOutcomeArches();
	//int lastAddedIndex = 1;
	//CheckLeafArch(outcomeArches[0], 0, lastAddedIndex, root, 0, lastAddedIndex);
	//CheckLeafArch(outcomeArches[1], 1, lastAddedIndex, root, 1, lastAddedIndex);
}

TEST(BuildTree, CanBuildTwoDifferentCharTree)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("ab", 2);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 3);
	int lastAddedIndex = 2;/*
	Arch** outcomeArches = root->GetOutcomeArches();

	CheckLeafArch(outcomeArches[0], 0, lastAddedIndex, root, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches[1], 1, lastAddedIndex, root, 1, lastAddedIndex);
	CheckLeafArch(outcomeArches[2], 2, lastAddedIndex, root, 2, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildThreeDifferentCharTree)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("abc", 3);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 4);
	int lastAddedIndex = 3;/*
	Arch** outcomeArches = root->GetOutcomeArches();

	CheckLeafArch(outcomeArches[0], 0, lastAddedIndex, root, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches[1], 1, lastAddedIndex, root, 1, lastAddedIndex);
	CheckLeafArch(outcomeArches[2], 2, lastAddedIndex, root, 2, lastAddedIndex);
	CheckLeafArch(outcomeArches[3], 3, lastAddedIndex, root, 3, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildTwoSameCharTree)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("aa", 2);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 2);
	int lastAddedIndex = 2;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex = outcomeArches[0]->nextVertex;
	CheckNotLeafArch(outcomeArches[0], 0, 0, root, nextVertex, lastAddedIndex);
	CheckLeafArch(outcomeArches[1], 2, lastAddedIndex, root, 2, lastAddedIndex);

	CheckVertex(nextVertex, false, outcomeArches[0], root, 2);
	outcomeArches = nextVertex->GetOutcomeArches();

	CheckLeafArch(outcomeArches[0], 1, lastAddedIndex, nextVertex, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches[1], 2, lastAddedIndex, nextVertex, 1, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildThreeCharTree)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("aba", 3);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 3);
	int lastAddedIndex = 3;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex = outcomeArches[1]->nextVertex;
	CheckLeafArch(outcomeArches[0], 1, lastAddedIndex, root, 1, lastAddedIndex);
	CheckNotLeafArch(outcomeArches[1], 0, 0, root, nextVertex, lastAddedIndex);
	CheckLeafArch(outcomeArches[2], 3, lastAddedIndex, root, 3, lastAddedIndex);

	CheckVertex(nextVertex, false, outcomeArches[1], root, 2);
	outcomeArches = nextVertex->GetOutcomeArches();

	CheckLeafArch(outcomeArches[0], 1, lastAddedIndex, nextVertex, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches[1], 3, lastAddedIndex, nextVertex, 2, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildThreeSameCharTree)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("aaa", 3);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 2);
	int lastAddedIndex = 3;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex = outcomeArches[0]->nextVertex;
	
	CheckNotLeafArch(outcomeArches[0], 0, 0, root, nextVertex, lastAddedIndex);
	CheckLeafArch(outcomeArches[1], 3, lastAddedIndex, root, 3, lastAddedIndex);

	CheckVertex(nextVertex, false, outcomeArches[0], root, 2);
	Arch** outcomeArches2 = nextVertex->GetOutcomeArches();
	Vertex* nextVertex2 = outcomeArches2[0]->nextVertex;
	CheckNotLeafArch(outcomeArches2[0], 1, 1, nextVertex, nextVertex2, lastAddedIndex);
	CheckLeafArch(outcomeArches2[1], 3, lastAddedIndex, nextVertex, 2, lastAddedIndex);

	CheckVertex(nextVertex2, false, outcomeArches2[0], nextVertex, 2);
	Arch** outcomeArches3 = nextVertex2->GetOutcomeArches();

	CheckLeafArch(outcomeArches3[0], 2, lastAddedIndex, nextVertex2, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches3[1], 3, lastAddedIndex, nextVertex2, 1, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildThreeCharTreeBAA)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("baa", 3);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 3);
	int lastAddedIndex = 3;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex = outcomeArches[1]->nextVertex;

	CheckLeafArch(outcomeArches[0], 0, lastAddedIndex, root, 0, lastAddedIndex);
	CheckNotLeafArch(outcomeArches[1], 1, 1, root, nextVertex, lastAddedIndex);
	CheckLeafArch(outcomeArches[2], 3, lastAddedIndex, root, 3, lastAddedIndex);

	CheckVertex(nextVertex, false, outcomeArches[1], root, 2);
	Arch** outcomeArches2 = nextVertex->GetOutcomeArches();

	CheckLeafArch(outcomeArches2[0], 2, lastAddedIndex, nextVertex, 1, lastAddedIndex);
	CheckLeafArch(outcomeArches2[1], 3, lastAddedIndex, nextVertex, 2, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildCharTreeBABA)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("baba", 4);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 3);
	int lastAddedIndex = 4;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex1 = outcomeArches[0]->nextVertex;
	Vertex* nextVertex2 = outcomeArches[1]->nextVertex;

	CheckNotLeafArch(outcomeArches[0], 0, 1, root, nextVertex1, lastAddedIndex);
	CheckNotLeafArch(outcomeArches[1], 1, 1, root, nextVertex2, lastAddedIndex);
	CheckLeafArch(outcomeArches[2], 4, lastAddedIndex, root, 4, lastAddedIndex);

	CheckVertex(nextVertex1, false, outcomeArches[0], nextVertex2, 2);
	Arch** outcomeArches2 = nextVertex1->GetOutcomeArches();
	
	CheckLeafArch(outcomeArches2[0], 2, lastAddedIndex, nextVertex1, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches2[1], 4, lastAddedIndex, nextVertex1, 2, lastAddedIndex);

	CheckVertex(nextVertex2, false, outcomeArches[1], root, 2);
	Arch** outcomeArches3 = nextVertex2->GetOutcomeArches();

	CheckLeafArch(outcomeArches3[0], 2, lastAddedIndex, nextVertex2, 1, lastAddedIndex);
	CheckLeafArch(outcomeArches3[1], 4, lastAddedIndex, nextVertex2, 3, lastAddedIndex);*/
}

TEST(BuildTree, CanBuildCharTreeABACA)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("abaca", 5);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 4);
	int lastAddedIndex = 5;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex1 = outcomeArches[1]->nextVertex;

	CheckLeafArch(outcomeArches[0], 1, lastAddedIndex, root, 1, lastAddedIndex);
	CheckNotLeafArch(outcomeArches[1], 0, 0, root, nextVertex1, lastAddedIndex);
	CheckLeafArch(outcomeArches[2], 3, lastAddedIndex, root, 3, lastAddedIndex);
	CheckLeafArch(outcomeArches[3], 5, lastAddedIndex, root, 5, lastAddedIndex);

	CheckVertex(nextVertex1, false, outcomeArches[1], root, 3);
	Arch** outcomeArches2 = nextVertex1->GetOutcomeArches();

	CheckLeafArch(outcomeArches2[0], 1, lastAddedIndex, nextVertex1, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches2[1], 3, lastAddedIndex, nextVertex1, 2, lastAddedIndex);
	CheckLeafArch(outcomeArches2[2], 5, lastAddedIndex, nextVertex1, 4, lastAddedIndex);*/

}

TEST(BuildTree, CanBuildCharTreeABCABB)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("abcabb", 6);
	SuffixTreeBuilder builder(inputString);
	SuffixTree* tree = builder.BuildTree();
	ASSERT_TRUE(NULL != tree);
	Vertex* root = tree->GetRoot();
	CheckVertex(root, true, NULL, NULL, 4);
	int lastAddedIndex = 6;/*
	Arch** outcomeArches = root->GetOutcomeArches();
	Vertex* nextVertex1 = outcomeArches[1]->nextVertex;
	Vertex* nextVertex2 = outcomeArches[2]->nextVertex;

	CheckLeafArch(outcomeArches[0], 2, lastAddedIndex, root, 2, lastAddedIndex);
	CheckNotLeafArch(outcomeArches[1], 0, 1, root, nextVertex1, lastAddedIndex);
	CheckNotLeafArch(outcomeArches[2], 1, 1, root, nextVertex2, lastAddedIndex);
	CheckLeafArch(outcomeArches[3], 6, lastAddedIndex, root, 6, lastAddedIndex);

	CheckVertex(nextVertex1, false, outcomeArches[1], nextVertex2, 2);
	Arch** outcomeArches2 = nextVertex1->GetOutcomeArches();

	CheckLeafArch(outcomeArches2[0], 2, lastAddedIndex, nextVertex1, 0, lastAddedIndex);
	CheckLeafArch(outcomeArches2[1], 5, lastAddedIndex, nextVertex1, 3, lastAddedIndex);

	CheckVertex(nextVertex2, false, outcomeArches[2], root, 3);
	Arch** outcomeArches3 = nextVertex2->GetOutcomeArches();

	CheckLeafArch(outcomeArches3[0], 2, lastAddedIndex, nextVertex2, 1, lastAddedIndex);
	CheckLeafArch(outcomeArches3[1], 5, lastAddedIndex, nextVertex2, 4, lastAddedIndex);
	CheckLeafArch(outcomeArches3[2], 6, lastAddedIndex, nextVertex2, 5, lastAddedIndex);*/
}
