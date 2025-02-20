#include "Components/CMontagesComponent.h"

#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{
}


// Called when the game starts
void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();
	if (DataTable == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable is not Selected"));
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<FMontagesData*> datas;
	DataTable->GetAllRows("", datas);

	for (int i = 0; i < (int32)EStateType::Max; i++)
		for (FMontagesData* data : datas)
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
				continue;
			}
}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAinMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAinMontage(EStateType InType)
{
	if (OwnerCharacter == nullptr)
		return;

	FMontagesData* data = Datas[(int32)InType];

	if (data == nullptr || data->Montage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("None Montage data"));
		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}