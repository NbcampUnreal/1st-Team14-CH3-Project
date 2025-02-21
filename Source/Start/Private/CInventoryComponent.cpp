#include "CInventoryComponent.h"
#include "CBaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CPlayerController.h"
#include "GameFramework/Actor.h"

// 생성자: 최대 슬롯 수 초기화
UCInventoryComponent::UCInventoryComponent()
{
    MaxSlots = 10;
}

// 아이템을 인벤토리에 추가
bool UCInventoryComponent::AddToInventory(EItemType ItemType)
{
    if (InventoryItems.Num() >= MaxSlots)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("인벤토리 가득 참!"));
        return false;
    }

    // 아이템 추가
    InventoryItems.FindOrAdd(ItemType)++;

    UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 추가됨: %d (현재 개수: %d)"), static_cast<int32>(ItemType), InventoryItems[ItemType]);

    // 🔹 인벤토리 UI 업데이트 강제 실행
    OnInventoryUpdated.Broadcast();

    return true;
}

// 인벤토리에서 아이템 제거
bool UCInventoryComponent::RemoveItem(EItemType ItemType)
{
    if (InventoryItems.Contains(ItemType))
    {
        if (--InventoryItems[ItemType] <= 0)
        {
            InventoryItems.Remove(ItemType);
        }

        UE_LOG(LogTemp, Warning, TEXT("🛑 아이템 제거됨: %d (남은 개수: %d)"),
            static_cast<int32>(ItemType),
            InventoryItems.Contains(ItemType) ? InventoryItems[ItemType] : 0);

        // 🔹 UI 업데이트 무조건 실행
        OnInventoryUpdated.Broadcast();

        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("❌ 제거하려는 아이템이 없습니다!"));
    return false;
}

bool UCInventoryComponent::RemoveItemAndUpdateUI(EItemType ItemType)
{
    if (RemoveItem(ItemType))
    {
        OnInventoryUpdated.Broadcast();
        return true;
    }
    return false;
}
// 아이템을 바닥에 드롭 (삭제가 아니라 월드에 아이템 스폰)
bool UCInventoryComponent::DropItem(EItemType ItemType)
{
    if (!DropItemClasses.Contains(ItemType))
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ DropItemClasses에서 %d를 찾을 수 없음"), static_cast<int32>(ItemType));
        return false;
    }

    if (!RemoveItem(ItemType))
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ RemoveItem 실패: %d"), static_cast<int32>(ItemType));
        return false;
    }

    TSubclassOf<ACBaseItem> ItemClass = DropItemClasses[ItemType];
    if (!ItemClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ ItemClass가 유효하지 않음"));
        return false;
    }

    FVector PlayerLocation = GetOwner()->GetActorLocation();
    FVector DropLocation = PlayerLocation + FVector(50.0f, 0.0f, 10.0f); // 🔹 살짝 띄워서 드랍

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ACBaseItem* SpawnedItem = GetWorld()->SpawnActor<ACBaseItem>(ItemClass, DropLocation, FRotator::ZeroRotator, SpawnParams);
    if (!SpawnedItem)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SpawnActor 실패!"));
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 드랍 성공: %d (위치: %s)"), static_cast<int32>(ItemType), *DropLocation.ToString());
    // 🔹 UI 업데이트 강제 실행
    OnInventoryUpdated.Broadcast();

    return true;
}

// 현재 인벤토리 상태 출력 (디버그용)
void UCInventoryComponent::PrintInventory()
{
    FString InventoryText = TEXT("🔹 현재 인벤토리:\n");
    for (const auto& Item : InventoryItems)
    {
        InventoryText += FString::Printf(TEXT("- %s: %d 개\n"), *UEnum::GetValueAsString(Item.Key), Item.Value);
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, InventoryText);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *InventoryText);
}
