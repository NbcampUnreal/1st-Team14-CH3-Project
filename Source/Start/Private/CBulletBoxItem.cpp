#include "CBulletBoxItem.h"
#include "CCharacter.h"
#include "CInventoryComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CWeapon.h"
#include "Kismet/KismetMathLibrary.h"

ACBulletBoxItem::ACBulletBoxItem()
{
    ItemType = EItemType::EIT_Bullet;
}

void ACBulletBoxItem::KeyPressedActivate(AActor* Activator)
{
    Super::KeyPressedActivate(Activator);

    //ACCharacter* PlayerCharacter = Cast<ACCharacter>(Activator);
    //if (!PlayerCharacter) return;

    //UCWeaponComponent* WeaponComponent = PlayerCharacter->FindComponentByClass<UCWeaponComponent>();
    //if (!WeaponComponent) return;

    //ACWeapon* CurrentWeapon = WeaponComponent->GetCurrentWeapon();
    //if (!CurrentWeapon) return;

    //// 🔹 랜덤으로 5~10개의 총알 생성
    //int32 BulletCount = UKismetMathLibrary::RandomIntegerInRange(5, 10);

    //// 🔹 현재 탄창이 가득 차지 않았으면 장전
    //if (CurrentWeapon->GetCurrentMagazineCount() < CurrentWeapon->GetMaxMagazineCount())
    //{
    //    int32 NeededBullets = CurrentWeapon->GetMaxMagazineCount() - CurrentWeapon->GetCurrentMagazineCount();
    //    int32 BulletsToLoad = FMath::Min(BulletCount, NeededBullets);

    //    CurrentWeapon->AddBullets(BulletsToLoad); // 🔹 탄창에 직접 추가
    //    UE_LOG(LogTemp, Warning, TEXT("🔫 %d개의 총알이 장전되었습니다!"), BulletsToLoad);
    //}
    //else
    //{
    //    // 🔹 탄창이 가득 차 있으면 인벤토리에 추가
    //    UCInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UCInventoryComponent>();
    //    if (InventoryComponent)
    //    {
    //        InventoryComponent->AddBulletsToInventory(BulletCount);
    //        UE_LOG(LogTemp, Warning, TEXT("📦 %d개의 총알이 인벤토리에 추가되었습니다!"), BulletCount);
    //    }
    //}
}
