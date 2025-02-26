#include "CBulletBoxItem.h"
#include "CCharacter.h"
#include "CInventoryComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CWeapon.h"
#include "Kismet/KismetMathLibrary.h"

ACBulletBoxItem::ACBulletBoxItem()
{
    ItemType = EItemType::EIT_BulletBox;
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

    UE_LOG(LogTemp, Warning, TEXT("📦 %d개의 총알이 인벤토리에 추가되었습니다!"), BulletCount);
}