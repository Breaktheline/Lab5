#include "Vertex.h"

Vertex::Vertex(Arch* incomeArch)
{
	if (incomeArch != NULL)
	{
		incomeArch->_nextVertex = this;
	}

	_incomeArch = incomeArch;
	_linkedVertex = NULL;
	_outcomeArches = new TList<Arch*>();
}

Vertex::~Vertex()
{
	int archesCount = _outcomeArches->GetCount();
	for (int i = 0; i < archesCount; i++)
	{
		delete _outcomeArches->GetElement(i);
	}

	delete _outcomeArches;
}

void Vertex::AddOutcomeArch(Arch* outcomeArch)
{
	outcomeArch->_prevVertex = this;
	_outcomeArches->Add(outcomeArch);
}

void Vertex::AddLinkedVertex(Vertex* linkedVertex)
{
	_linkedVertex = linkedVertex;
}

bool Vertex::IsRoot()
{
	return _incomeArch == NULL;
}

TList<Arch*>* Vertex::GetOutcomeArches()
{
	return _outcomeArches;
}

bool Vertex::HasLinkedVertex()
{
	return _linkedVertex != NULL;
}

Vertex* Vertex::GetLinkedVertex()
{
	return _linkedVertex;
}

Arch* Vertex::GetIncomeArch()
{
	return _incomeArch;
}

void Vertex::RemoveOutcomeArch(Arch* archToRemove)
{
	_outcomeArches->Remove(archToRemove);
	archToRemove->_prevVertex = NULL;
}

Arch* Vertex::GetOutcomeArch(int index)
{
	return _outcomeArches->GetElement(index);
}


