#include "CHealthPotionItem.h"

#include "SP_Pawn.h"
#include "CCharacter.h"

ACHealthPotionItem::ACHealthPotionItem()
{
	HealAmount = 20.f;
	ItemType = EItemType::EIT_HealthPotion;
  
    // ✅ StaticMeshComponent가 존재하는지 확인 후 메쉬 설정
    if (StaticMesh)
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cone.Cone"));
        if (MeshAsset.Succeeded())
        {
            StaticMesh->SetStaticMesh(MeshAsset.Object);
            UE_LOG(LogTemp, Warning, TEXT("✅ 힐스포션 StaticMesh 설정 완료!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 힐스포션 StaticMesh를 찾을 수 없음!"));
        }
    }
}

void ACHealthPotionItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	//GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add HealthPotionItem To Inventory")));
}

void ACHealthPotionItem::Use(AActor* Target)
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Green, FString::Printf(TEXT("Use HealthPotionItem")));
	if (ACCharacter* CCharacter = Cast<ACCharacter>(Target))
	{		
		CCharacter->Heal(HealAmount);
		
	}
}
