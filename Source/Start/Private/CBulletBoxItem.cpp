﻿#include "CBulletBoxItem.h"
#include "CCharacter.h"
#include "CInventoryComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CWeapon.h"
#include "Kismet/KismetMathLibrary.h"

ACBulletBoxItem::ACBulletBoxItem()
{
    ItemType = EItemType::EIT_BulletBox;
    // ✅ BaseItem의 StaticMesh를 박스 본체(Object_4)로 설정
    if (StaticMesh && !StaticMesh->GetStaticMesh())
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshAsset(TEXT("/Game/Assets/Mesh/Item/box/Object_4.Object_4"));
        if (BoxMeshAsset.Succeeded())
        {
            StaticMesh->SetStaticMesh(BoxMeshAsset.Object);
            StaticMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
            UE_LOG(LogTemp, Warning, TEXT("✅ BaseItem의 StaticMesh를 이용하여 Bullet Box 생성"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ BaseItem의 StaticMesh에 사용할 Bullet Box 메쉬를 찾을 수 없음!"));
        }
    }

    // ✅ 뚜껑 추가 (LidMesh)
    LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
    LidMesh->SetupAttachment(RootComponent);  // 📌 RootComponent에 직접 부착

    static ConstructorHelpers::FObjectFinder<UStaticMesh> LidMeshAsset(TEXT("/Game/Assets/Mesh/Item/box/Object_6.Object_6"));
    if (LidMeshAsset.Succeeded())
    {
        LidMesh->SetStaticMesh(LidMeshAsset.Object);
        LidMesh->SetCollisionProfileName(TEXT("NoCollision"));

        // ✅ 뚜껑의 위치를 올바르게 조정 (박스 위에 배치)
        LidMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // 적절한 높이로 조정
        UE_LOG(LogTemp, Warning, TEXT("✅ 뚜껑 스태틱 메쉬 설정 완료!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 뚜껑 스태틱 메쉬를 찾을 수 없음!"));
    }
}

void ACBulletBoxItem::KeyPressedActivate(AActor* Activator)
{
    Super::KeyPressedActivate(Activator);
    // ✅ 자동 실행 방지: 플레이어가 컨트롤 중일 때만 실행
    ACCharacter* PlayerCharacter = Cast<ACCharacter>(Activator);
    if (!PlayerCharacter || !PlayerCharacter->IsPlayerControlled()) return;

    UCInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UCInventoryComponent>();
    if (!InventoryComponent) return;

    // 🔹 인벤토리에 총알 박스 추가 (1개만 가능)
    if (InventoryComponent->AddToInventory(ItemType))
    {
        UE_LOG(LogTemp, Warning, TEXT("📦 총알 박스가 인벤토리에 추가됨"));
    }
}

void ACBulletBoxItem::Use(AActor* Target)
{
    ACCharacter* Character = Cast<ACCharacter>(Target);
    if (!Character) return;

	ACPlayer* Player = Cast<ACPlayer>(Character);
    if (!Player) return;

	UCInventoryComponent* InventoryComponent = Cast<UCInventoryComponent>(Player->GetComponentByClass(UCInventoryComponent::StaticClass()));
    if (!InventoryComponent) return;

    // 🔹 5~10개 랜덤 개수의 총알 생성
    int32 BulletCount = UKismetMathLibrary::RandomIntegerInRange(5, 10);

    // 🔹 인벤토리에 총알 추가
    InventoryComponent->AddBulletsToInventory(BulletCount);

    // 🔹 현재 총알 개수 가져오기
    int32 CurrentBullets = InventoryComponent->GetBulletCount();

    UE_LOG(LogTemp, Warning, TEXT("📦 %d개의 총알이 인벤토리에 추가되었습니다!"), BulletCount);
}