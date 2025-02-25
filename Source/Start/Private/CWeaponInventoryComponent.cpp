#include "CWeaponInventoryComponent.h"

UCWeaponInventoryComponent::UCWeaponInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UCWeaponInventoryComponent::AddWeaponToInventory(TSubclassOf<ACWeapon> NewWeaponClass, UTexture2D* WeaponIcon)
{
    if (WeaponInventory.Num() >= 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 무기 슬롯이 가득 찼습니다!"));
        return false;
    }

    FWeaponInventoryEntry NewSlot;  // ✅ 이름 변경된 구조체 사용
    NewSlot.WeaponClass = NewWeaponClass;
    NewSlot.WeaponIcon = WeaponIcon;

    WeaponInventory.Add(NewSlot);
    OnWeaponInventoryUpdated.Broadcast();

    UE_LOG(LogTemp, Warning, TEXT("🔫 무기 추가됨: %s"), *NewWeaponClass->GetName());
    return true;
}

bool UCWeaponInventoryComponent::DropWeapon(int32 SlotIndex)
{
    if (!WeaponInventory.IsValidIndex(SlotIndex)) return false;

    FWeaponInventoryEntry DroppedWeapon = WeaponInventory[SlotIndex];  // ✅ 변경된 구조체 사용

    // ✅ 월드에 무기 스폰
    if (DroppedWeapon.WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        GetWorld()->SpawnActor<ACWeapon>(
            DroppedWeapon.WeaponClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
    }

    // ✅ 인벤토리에서 제거
    WeaponInventory.RemoveAt(SlotIndex);
    OnWeaponInventoryUpdated.Broadcast();

    UE_LOG(LogTemp, Warning, TEXT("🛑 무기 드랍됨: %s"), *DroppedWeapon.WeaponClass->GetName());
    return true;
}
