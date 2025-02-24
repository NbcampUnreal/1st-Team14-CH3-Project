#include "Components/CStateComponent.h"

UCStateComponent::UCStateComponent()
{
}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetInventoryMode()
{
	ChangeType(EStateType::Inventory);
}

void UCStateComponent::ChangeType(EStateType NewType)
{
	PreveType = Type;
	Type = NewType;
	if (OnStateTypeChanged.IsBound() == true)
		OnStateTypeChanged.Broadcast(PreveType, Type);
}
