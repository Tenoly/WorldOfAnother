// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"

#include <string>

void UEntity::Init(UEntityTemplate* temp, UEntity* parent, const FRandomStream& rnd) {
	Template = temp;
	Parent = parent;
	if (Template->UsedInCreation) {
		for (auto t : Template->SubEntities) {
			int32 rand = rnd.RandRange(t.Value.X, t.Value.Y);
			for (int i = 0; i < rand; ++i) {
				UEntity* entity = NewObject<UEntity>(this->GetOuter());
				SubEntities.Add(entity);
				entity->Init(t.Key, this, rnd);
			}
		}
	}

	CreateName(rnd);
}

void UEntity::CreateName(const FRandomStream& rnd) {
	FString base = Template->NameBases[rnd.RandRange(0, Template->NameBases.Num()-1)];
	Name = base + FString::FromInt(rnd.RandRange(0, 1000));
}

bool UEntity::IsRoot() const {
	return !IsValid(Parent);
}

bool UEntity::IsIndividual() const {
	return Template->SubEntities.IsEmpty();
}

TArray<UEntity*> UEntity::GetAllIndividuals() {
	TArray<UEntity*> all;
	if (IsIndividual())
		all.Add(this);
	else {
		for (UEntity* entity : SubEntities) {
			all.Append(entity->GetAllIndividuals());
		}
	}
	return all;
}

int32 UEntity::GetSize() const {
	if (IsIndividual())	return 1;
	
	int32 size = 0;
	for (UEntity* entity : SubEntities) {
		size += entity->GetSize();
	}
	return size;
}
