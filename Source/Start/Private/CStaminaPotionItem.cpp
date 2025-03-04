// Fill out your copyright notice in the Description page of Project Settings.


#include "CStaminaPotionItem.h"

ACStaminaPotionItem::ACStaminaPotionItem()
{
	ItemType = EItemType::EIT_StaminaPotion;
    // ✅ StaticMeshComponent가 존재하는지 확인 후 메쉬 설정
    if (StaticMesh)
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Mesh/Item/Object_10.Object_10"));
        if (MeshAsset.Succeeded())
        {
            StaticMesh->SetStaticMesh(MeshAsset.Object);
            UE_LOG(LogTemp, Warning, TEXT("✅ 스테미나포션 StaticMesh 설정 완료!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 스테미나포션 StaticMesh를 찾을 수 없음!"));
        }
    }
}

void ACStaminaPotionItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	//GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add StaminaPotionItem To Inventory")));
}

void ACStaminaPotionItem::Use(AActor* Target)
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Green, FString::Printf(TEXT("Use StaminaPotionItem")));
}
