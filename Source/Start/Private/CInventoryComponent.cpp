#include "CInventoryComponent.h"
#include "CPlayer.h"
#include "CBaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CPlayerController.h"
#include "CBulletBoxItem.h"
#include "CHealthPotionItem.h"
#include "CStaminaPotionItem.h"
#include "Weapon/CBullet.h"
#include "CGrenadesItem.h"
#include "Weapon/CWeapon_Pistol.h"
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
    if (ACHealthPotionItem::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_HealthPotion, ACHealthPotionItem::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ HealthPotion 아이템이 DropItemClasses에 정상 등록됨."));
    }
    if (ACStaminaPotionItem::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_StaminaPotion, ACStaminaPotionItem::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ StaminaPotion 아이템이 DropItemClasses에 정상 등록됨."));
    }
    if (ACBullet::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_Bullet, ACBullet::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ Bullet 아이템이 DropItemClasses에 정상 등록됨."));
    }
    if (ACGrenadesItem::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_Grenades, ACGrenadesItem::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ GrenadesItem 아이템이 DropItemClasses에 정상 등록됨."));
    }
    if (ACWeapon_Pistol::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_Pistol, ACWeapon_Pistol::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ Pistol 아이템이 DropItemClasses에 정상 등록됨."));
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
	UE_LOG(LogTemp, Warning, TEXT("%d"), ItemType);
    if (ItemClass.Get() == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GetItemInstance 실패 - ItemClass(%d)가 NULL!"), static_cast<int32>(ItemType));
        return nullptr;
    }
    else
        UE_LOG(LogTemp, Error, TEXT("❌ GetItemInstance 성공"), static_cast<int32>(ItemType));


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
    // 🔹 아이템별 최대 스택 제한 설정 (기본값 99)
    static TMap<EItemType, int32> MaxStackLimits = {
        {EItemType::EIT_Bullet, 300},  // 🔹 총알은 최대 300개까지 저장 가능
        {EItemType::EIT_HealthPotion, 10},  // 🔹 체력 포션은 최대 10개까지
        {EItemType::EIT_StaminaPotion, 10},
        {EItemType::EIT_Grenades, 5},  // 🔹 수류탄은 최대 5개까지
        {EItemType::EIT_BulletBox, 5},  // 🔹 총알 박스는 최대 5개까지
        {EItemType::EIT_Pistol, 1 }, // 🔹 권총은 최대 1개까지
        {EItemType::EIT_Rifle, 1 }, // 🔹 라이플은 최대 1개까지
        {EItemType::EIT_Shotgun, 1 } // 🔹 샷건은 최대 1개까지
    };

    const int32 MaxStackSize = MaxStackLimits.Contains(ItemType) ? MaxStackLimits[ItemType] : 999;  // 기본값 99

    // 🔹 기존 아이템이 있는 경우 개수 증가
    if (InventoryItems.Contains(ItemType))
    {
        if (InventoryItems[ItemType] < MaxStackSize)
        {
            InventoryItems[ItemType]++;
            UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 추가됨: %d (현재 개수: %d / 최대: %d)"),
                static_cast<int32>(ItemType), InventoryItems[ItemType], MaxStackSize);

            // ✅ UI 업데이트
            OnInventoryUpdated.Broadcast();
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("⚠️ 최대 개수를 초과할 수 없습니다! (아이템 타입: %d)"), static_cast<int32>(ItemType));
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("⚠️ 최대 개수를 초과할 수 없습니다!"));
            return false;
        }
    }

    // 🔹 새로운 아이템을 추가하는 경우, 인벤토리 슬롯 제한 체크
    if (InventoryItems.Num() >= MaxSlots)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 인벤토리가 가득 찼습니다! (현재 슬롯: %d / 최대 슬롯: %d)"), InventoryItems.Num(), MaxSlots);
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("❌ 인벤토리가 가득 찼습니다!"));
        return false;
    }

    // 🔹 새로운 아이템 추가
    InventoryItems.Add(ItemType, 1);
    UE_LOG(LogTemp, Warning, TEXT("✅ 새로운 아이템 추가됨: %d (현재 개수: %d / 최대: %d)"),
        static_cast<int32>(ItemType), InventoryItems[ItemType], MaxStackSize);

    // 🚨 Bullet Box가 잘못 추가되었는지 확인
    if (ItemType == EItemType::EIT_BulletBox)
    {
        UE_LOG(LogTemp, Error, TEXT("🚨 오류: AddToInventory에서 Bullet Box가 잘못 추가됨! 원인 확인 필요."));
    }

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

    // 🔹 무기인지 확인 (무기면 장착, 아이템이면 기존 방식 사용)
    if (ItemType == EItemType::EIT_Pistol || ItemType == EItemType::EIT_Rifle || ItemType == EItemType::EIT_Shotgun)
    {
        EquipWeapon(ItemType, Player);  // 🔹 무기 장착 함수 호출
        return true;  // ✅ 무기는 사용해도 개수 감소 X
    }

    // 🔹 일반 아이템 사용 로직
    ACBaseItem* ItemInstance = GetItemInstance(ItemType);
    if (!ItemInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ UseItem 실패 - GetItemInstance(%d) 가 NULL을 반환함!"), static_cast<int32>(ItemType));
        return false;
    }

    // 🔹 아이템 사용 (AActor로 캐스팅)
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

    // 🔹 사용 후 개수 감소 (무기는 여기서 제외)
    if (InventoryItems.Contains(ItemType) && InventoryItems[ItemType] > 0)
    {
        RemoveItem(ItemType);
        UE_LOG(LogTemp, Warning, TEXT("🚨 아이템(%d) 제거됨 - 개수 0"), static_cast<int32>(ItemType));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ RemoveItem() 호출 전에 아이템이 이미 인벤토리에 없음: %d"), static_cast<int32>(ItemType));
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

    // 🚨 Bullet Box가 잘못 추가되는지 확인
    if (InventoryItems.Contains(EItemType::EIT_BulletBox))
    {
    }

    // 🔹 UI 업데이트
    OnInventoryUpdated.Broadcast();
}

IIItemInterface* UCInventoryComponent::FindItemByType(EItemType ItemType)
{
    ACBaseItem* FoundItem = GetItemInstance(ItemType);
    if (!FoundItem)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ FindItemByType 실패 - GetItemInstance(%d) 가 NULL을 반환!"), static_cast<int32>(ItemType));
        return nullptr;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ FindItemByType 성공 - 아이템(%d) 반환!"), static_cast<int32>(ItemType));
    return Cast<IIItemInterface>(FoundItem);
}

int32 UCInventoryComponent::GetBulletCount() const
{
    if (InventoryItems.Contains(EItemType::EIT_Bullet))
    {
        return InventoryItems[EItemType::EIT_Bullet]; // 🔹 현재 총알 개수 반환
    }
    return 0; // 🔹 인벤토리에 총알이 없으면 0 반환
}

void UCInventoryComponent::EquipWeapon(EItemType WeaponType, ACPlayer* Player)
{
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ EquipWeapon 실패 - Player 또는 WeaponComponent가 없음"));
        return;
    }

    UCWeaponComponent* WeaponComp = Cast<UCWeaponComponent>(Player->GetComponentByClass(UCWeaponComponent::StaticClass()));

    // 🔹 EItemType을 WeaponClasses 배열의 인덱스로 변환
    int32 WeaponIndex = WeaponComp->GetWeaponIndexFromItemType(WeaponType);

    // 🔹 배열 범위 초과 방지
    if (WeaponIndex < 0 || !WeaponComp->WeaponClasses.IsValidIndex(WeaponIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ EquipWeapon 실패 - 유효하지 않은 무기 인덱스! (%d)"), WeaponIndex);
        return;
    }

    // ✅ 변환된 인덱스를 이용해 무기 장착
    //WeaponComp->SetMode((EWeaponType)WeaponIndex);

    UE_LOG(LogTemp, Warning, TEXT("✅ 무기 장착 완료: %d (WeaponIndex: %d)"), static_cast<int32>(WeaponType), WeaponIndex);
}


