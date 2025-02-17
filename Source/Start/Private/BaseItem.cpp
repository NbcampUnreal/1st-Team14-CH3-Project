// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseItem.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 메시 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetupAttachment(Scene);
	
	// 콜리전 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);
	
	// Overlap 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
	
	
}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
		//디버그
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!"))); 
	}
}

void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlapPrimitiveComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
{
	
}

void ABaseItem::ActivateItem(AActor* Activator)
{
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}
