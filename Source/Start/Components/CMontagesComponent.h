// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CStateComponent.h"
#include "Components/ActorComponent.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EStateType Type;
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class START_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Data Table")
	UDataTable* DataTable;
public:
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayDeadMode();

private:
	void PlayAinMontage(EStateType InType);

private:
	ACharacter* OwnerCharacter;
	FMontagesData* Datas[(int32)EStateType::Max];

};