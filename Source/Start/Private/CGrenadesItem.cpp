#include "CGrenadesItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ACGrenadesItem::ACGrenadesItem()
{
	ExplosiveDelay = 3;
	ExplosiveRadius = 300;
	ExplosiveDamage = 30;
	ItemType = "Mine";
	
	MineCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MineCollision"));
	MineCollision->InitSphereRadius(ExplosiveRadius);
	MineCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MineCollision->SetupAttachment(Scene);
}

void ACGrenadesItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("Add GrenadesItem To Inventory")));
}

void ACGrenadesItem::Use()
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Green, FString::Printf(TEXT("Use GrenadesItem")));

}

void ACGrenadesItem::Explode()
{
	TArray<AActor*> OverlappingActors; // 범위 내에 겹친 액터들을 저장해 줄 배열
	MineCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Enemy"))
		{
			UGameplayStatics::ApplyDamage(
				Actor, // 데미지 대상
				ExplosiveDamage,// 데미지 양
				nullptr, // 발생 주체 but 게임 구현상 없음
				this, // 데미지 원인 객체
				UDamageType::StaticClass()); // 데미지 유형, 기본 데미지 유형으로 설정
		}
	}
}

