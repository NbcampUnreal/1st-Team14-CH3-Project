#include "CInventoryComponent.h"
#include "CPlayer.h"
#include "CBaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CPlayerController.h"
#include "CBulletBoxItem.h"
#include "GameFramework/Actor.h"

// 생성자: 최대 슬롯 수 초기화
UCInventoryComponent::UCInventoryComponent()
{
    MaxSlots = 12;
}

void UCInventoryComponent::BeginPlay()
{
    Super::BeginPlay();  // 🔹 부모 클래스의 BeginPlay 호출 (중요)

    // ✅ DropItemClasses에 BulletBoxItem 추가
    if (ACBulletBoxItem::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_BulletBox, ACBulletBoxItem::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ BulletBox 아이템이 DropItemClasses에 정상 등록됨."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ACBulletBoxItem::StaticClass()가 NULL임. DropItemClasses에 추가 실패!"));
    }
}

ACBaseItem* UCInventoryComponent::GetItemInstance(EItemType ItemType)
{
    if (!DropItemClasses.Contains(ItemType))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GetItemInstance 실패 - DropItemClasses에 해당 아이템(%d)이 없음!"), static_cast<int32>(ItemType));
        return nullptr;
    }

    // 🔹 아이템의 기본 오브젝트 가져오기
    TSubclassOf<ACBaseItem> ItemClass = DropItemClasses[ItemType];
    if (!ItemClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GetItemInstance 실패 - ItemClass(%d)가 NULL!"), static_cast<int32>(ItemType));
        return nullptr;
    }

    ACBaseItem* DefaultItem = ItemClass->GetDefaultObject<ACBaseItem>();
    if (!DefaultItem)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GetItemInstance 실패 - DefaultItem이 NULL (ItemType: %d)"), static_cast<int32>(ItemType));
        return nullptr;
    }

    return DefaultItem;
}


bool UCInventoryComponent::AddToInventory(EItemType ItemType)
{
    // 🔹 최대 아이템 스택 제한 설정 (예: 99개)
    const int32 MaxStackSize = 99;

    // 🔹 기존 아이템이 존재하면 개수만 증가
    if (InventoryItems.Contains(ItemType))
    {
        if (InventoryItems[ItemType] < MaxStackSize)
        {
            InventoryItems[ItemType]++;
            UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 추가됨: %d (현재 개수: %d)"), static_cast<int32>(ItemType), InventoryItems[ItemType]);

            // ✅ UI 업데이트
            OnInventoryUpdated.Broadcast();

            return true;
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("⚠️ 최대 개수를 초과할 수 없습니다!"));
            return false;
        }
    }

    // 🔹 새로운 아이템을 추가하는 경우, 인벤토리 슬롯 제한 체크
    if (InventoryItems.Num() >= MaxSlots)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("❌ 인벤토리가 가득 찼습니다!"));
        return false;
    }

    // 🔹 새로운 아이템 추가
    InventoryItems.Add(ItemType, 1);
    UE_LOG(LogTemp, Warning, TEXT("✅ 새로운 아이템 추가됨: %d (현재 개수: %d)"), static_cast<int32>(ItemType), InventoryItems[ItemType]);

    // ✅ UI 업데이트
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
        if (OnInventoryUpdated.IsBound())
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

bool UCInventoryComponent::UseItem(EItemType ItemType, ACPlayer* Player)
{
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ UseItem 실패 - Player가 NULL입니다!"));
        return false;
    }

    // 인벤토리에 해당 아이템이 있는지 확인
    if (!InventoryItems.Contains(ItemType) || InventoryItems[ItemType] <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ UseItem 실패 - 아이템이 인벤토리에 없음: %d"), static_cast<int32>(ItemType));
        return false;
    }

    // 아이템 인스턴스를 가져와 사용
    ACBaseItem* ItemInstance = GetItemInstance(ItemType);
    if (!ItemInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ UseItem 실패 - GetItemInstance(%d) 가 NULL을 반환함!"), static_cast<int32>(ItemType));
        return false;
    }

    // 🔹 Player가 AActor에서 상속되었는지 확인 후 캐스팅
    if (AActor* ActorPlayer = Cast<AActor>(Player))
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ UseItem 호출됨 - 아이템(%d) 사용 시작"), static_cast<int32>(ItemType));
        ItemInstance->Use(ActorPlayer);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ UseItem 실패 - ACPlayer를 AActor로 캐스팅할 수 없음!"));
        return false;
    }

    // 🔹 사용 후 수량 감소
    InventoryItems[ItemType]--;
    UE_LOG(LogTemp, Warning, TEXT("🛑 아이템(%d) 사용됨 - 남은 개수: %d"), static_cast<int32>(ItemType), InventoryItems[ItemType]);

    if (InventoryItems[ItemType] <= 0)
    {
        InventoryItems.Remove(ItemType);
        UE_LOG(LogTemp, Warning, TEXT("🚨 아이템(%d) 제거됨 - 개수 0"), static_cast<int32>(ItemType));
    }

    // 🔹 UI 업데이트
    OnInventoryUpdated.Broadcast();

    return true;
}

void UCInventoryComponent::AddBulletsToInventory(int32 BulletCount)
{
	if (BulletCount <= 0) return;

	// 🔹 인벤토리에 총알 개수 추가
	if (InventoryItems.Contains(EItemType::EIT_Bullet))
	{
		InventoryItems[EItemType::EIT_Bullet] += BulletCount;
	}
	else
	{
		InventoryItems.Add(EItemType::EIT_Bullet, BulletCount);
	}

	UE_LOG(LogTemp, Warning, TEXT("📦 %d개의 총알이 인벤토리에 추가되었습니다! (현재 총알: %d)"), 
        BulletCount, InventoryItems[EItemType::EIT_Bullet]);

	// 🔹 UI 업데이트
	OnInventoryUpdated.Broadcast();
}