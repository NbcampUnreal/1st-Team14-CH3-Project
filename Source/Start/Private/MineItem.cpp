// Fill out your copyright notice in the Description page of Project Settings.

#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
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

void AMineItem::OverlapActivate(AActor* Activator)
{
	// 게임 월드에는 타이머 핸들러들을 관리하는 타이머 매니저가 있다.
	GetWorld()->GetTimerManager().SetTimer(
		ExplosiveTimerHandle, // 타이머 핸들러
		this, // 해당 객체
		&AMineItem::Explode, // 동작 메서드
		ExplosiveDelay, // 시간
		false); // 반복할 것인가?
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors; // 범위 내에 겹친 액터들을 저장해 줄 배열
	MineCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor, // 데미지 대상
				ExplosiveDamage,// 데미지 양
				nullptr, // 발생 주체 but 게임 구현상 없음
				this, // 데미지 원인 객체
				UDamageType::StaticClass()); // 데미지 유형, 기본 데미지 유형으로 설정
		}
	}
	DestroyItem();
}

