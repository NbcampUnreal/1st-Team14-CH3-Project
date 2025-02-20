#include "CInventoryComponent.h"
#include "CBaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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
    // 🔹 디버깅 로그 추가 (아이템 추가 확인)
    UE_LOG(LogTemp, Warning, TEXT("아이템 추가됨: %d (현재 개수: %d)"), static_cast<int32>(ItemType), InventoryItems[ItemType]);
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

        // 🔹 디버깅 로그 추가 (아이템 제거 확인)
        UE_LOG(LogTemp, Warning, TEXT("아이템 제거됨: %d (남은 개수: %d)"), static_cast<int32>(ItemType), InventoryItems.Contains(ItemType) ? InventoryItems[ItemType] : 0);

        return true;
    }
    UE_LOG(LogTemp, Warning, TEXT("제거하려는 아이템이 없습니다!"));
    return false;
}

// 아이템을 바닥에 드롭 (삭제가 아니라 월드에 아이템 스폰)
bool UCInventoryComponent::DropItem(EItemType ItemType)
{
    if (RemoveItem(ItemType))
    {
        if (DropItemClasses.Contains(ItemType))
        {
            UE_LOG(LogTemp, Warning, TEXT("바닥에 아이템 드랍됨: %d"), static_cast<int32>(ItemType));
            TSubclassOf<ACBaseItem> ItemClass = DropItemClasses[ItemType];

            FVector PlayerLocation = GetOwner()->GetActorLocation();
            FVector DropLocation = PlayerLocation + FVector(50.0f, 0.0f, 0.0f);

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwner();
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            GetWorld()->SpawnActor<ACBaseItem>(ItemClass, DropLocation, FRotator::ZeroRotator, SpawnParams);

            FString ItemName = UEnum::GetValueAsString(ItemType);
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%s 드롭됨!"), *ItemName));
        }
        return true;
    }
    return false;
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
