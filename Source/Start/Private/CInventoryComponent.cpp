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
#include "Weapon/CWeapon_Rifle.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// 생성자: 최대 슬롯 수 초기화
UCInventoryComponent::UCInventoryComponent()
{
    MaxSlots = 12;
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/DataTable/ItemDataTable.ItemDataTable"));
    if (DataTableRef.Succeeded())
    {
        ItemDataTable = DataTableRef.Object;
        UE_LOG(LogTemp, Warning, TEXT("✅ C++에서 ItemDataTable을 강제로 설정했습니다."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ C++에서 ItemDataTable을 찾을 수 없습니다. 경로를 확인하세요."));
    }
    // ✅ 초기 총알 개수 설정 (70발)
    InventoryItems.Add(EItemType::EIT_Bullet, 70);
}

void UCInventoryComponent::BeginPlay()
{
    Super::BeginPlay();  // 🔹 부모 클래스의 BeginPlay 호출 (중요)

    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemDataTable이 설정되지 않았습니다! 블루프린트에서 확인하세요."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ ItemDataTable이 정상적으로 로드되었습니다."));
    }
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
  /*  if (ACWeapon_Pistol::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_Pistol, ACWeapon_Pistol::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ Pistol 아이템이 DropItemClasses에 정상 등록됨."));
    }
    if (ACWeapon_Rifle::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_Rifle, ACWeapon_Rifle::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ Pistol 아이템이 DropItemClasses에 정상 등록됨."));
    }*/
  /*  if (ACWeapon_Shotgun::StaticClass())
    {
        DropItemClasses.Add(EItemType::EIT_Shotgun, ACWeapon_Shotgun::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("✅ Pistol 아이템이 DropItemClasses에 정상 등록됨."));
    }*/

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
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemDataTable이 설정되지 않았습니다."));
        return false;
    }

    // ✅ 데이터 테이블에서 해당 아이템의 정보 찾기
    FString ContextString;
    FName RowName = IIItemInterface::GetRowNameFromItemType(ItemType);
    FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName, ContextString);

    if (!ItemData)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 해당 아이템(%s)의 데이터를 찾을 수 없습니다."), *RowName.ToString());
        return false;
    }

    // ✅ 아이템별 최대 개수 설정
    static TMap<EItemType, int32> MaxStackLimits = {
        {EItemType::EIT_Bullet, 300},
        {EItemType::EIT_HealthPotion, 10},
        {EItemType::EIT_StaminaPotion, 10},
        {EItemType::EIT_Grenades, 5},
        {EItemType::EIT_BulletBox, 5}
    };

    int32 MaxStackSize = MaxStackLimits.Contains(ItemType) ? MaxStackLimits[ItemType] : 999; // 기본값 999

    // ✅ 기존 아이템 개수 증가 (최대 개수 제한 확인)
    if (InventoryItems.Contains(ItemType))
    {
        if (InventoryItems[ItemType] < MaxStackSize)
        {
            InventoryItems[ItemType]++;
            ItemDetails.Add(ItemType, *ItemData);  // ✅ 아이템 정보를 저장
            UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 추가됨: %s (현재 개수: %d / 최대: %d)"),
                *ItemData->Name.ToString(), InventoryItems[ItemType], MaxStackSize);

            OnInventoryUpdated.Broadcast();
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("⚠️ 최대 개수를 초과할 수 없습니다! (아이템: %s)"), *ItemData->Name.ToString());
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("⚠️ 최대 개수를 초과할 수 없습니다!"));
            return false;
        }
    }

    // ✅ 새로운 아이템 추가 (인벤토리 슬롯 제한 체크)
    if (InventoryItems.Num() >= MaxSlots)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 인벤토리가 가득 찼습니다! (현재 슬롯: %d / 최대 슬롯: %d)"), InventoryItems.Num(), MaxSlots);
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("❌ 인벤토리가 가득 찼습니다!"));
        return false;
    }

    InventoryItems.Add(ItemType, 1);
    ItemDetails.Add(ItemType, *ItemData);  // ✅ 새로운 아이템 정보 저장

    UE_LOG(LogTemp, Warning, TEXT("✅ 새로운 아이템 추가됨: %s (현재 개수: %d / 최대: %d)"),
        *ItemData->Name.ToString(), InventoryItems[ItemType], MaxStackSize);

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

        // 🔹 UI 업데이트는 한 번만 실행
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

    TSubclassOf<ACBaseItem> ItemClass = DropItemClasses[ItemType];
    if (!ItemClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ ItemClass가 유효하지 않음"));
        return false;
    }

    FVector PlayerLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector DropStart = PlayerLocation + ForwardVector * 50.0f + FVector(0.0f, 0.0f, 50.0f);  // 플레이어 앞쪽, 약간 위에서 시작

    // ✅ Raycast(라인 트레이스)로 지면 위치 찾기
    FVector DropEnd = DropStart - FVector(0.0f, 0.0f, 200.0f);  // 200 유닛 아래로 검사
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());  // 플레이어 충돌 무시

    FVector FinalDropLocation = DropStart;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, DropStart, DropEnd, ECC_Visibility, QueryParams))
    {
        // ✅ 지면을 찾으면 그 위치에 스폰
        FinalDropLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, 5.0f);  // 지면 위 약간 띄움
    }

    FActorSpawnParameters SpawnParams;

    SpawnParams.Owner = GetOwner();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    //FVector DefaultScale = GetDefault<ACBaseItem>(ItemClass)->GetActorScale3D();
    FVector DefaultScale = FVector(0.2f, 0.2f, 0.2f);
    // ✅ 아이템 스폰
    ACBaseItem* SpawnedItem = GetWorld()->SpawnActor<ACBaseItem>(ItemClass, FinalDropLocation, FRotator::ZeroRotator, SpawnParams);
    if (!SpawnedItem)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SpawnActor 실패!"));
        return false;
    }

    // ✅ 블루프린트에서 설정된 크기로 유지
    SpawnedItem->SetActorScale3D(DefaultScale);

    // ✅ 충돌 설정 (콜리전 활성화)
    UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(SpawnedItem->GetRootComponent());
    if (RootPrimitive)
    {
        RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        RootPrimitive->SetCollisionResponseToAllChannels(ECR_Block);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SpawnedItem의 RootComponent가 UPrimitiveComponent가 아닙니다! (콜리전 설정 실패)"));
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 드랍 성공: %d (위치: %s, 크기: %s)"),
        static_cast<int32>(ItemType),
        *FinalDropLocation.ToString(),
        *DefaultScale.ToString());

    // 🔹 UI 업데이트 강제 실행
    OnInventoryUpdated.Broadcast();

    return true;
}


// 현재 인벤토리 상태 출력 (디버그용)
void UCInventoryComponent::PrintInventory()
{
    FString InventoryText = TEXT("🔹 현재 인벤토리 상태:\n");

    for (const auto& Item : InventoryItems)
    {
        FString ItemName = ItemDetails.Contains(Item.Key) ? ItemDetails[Item.Key].Name.ToString() : TEXT("Unknown Item");
        InventoryText += FString::Printf(TEXT("- %s: %d 개\n"), *ItemName, Item.Value);
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

    // 🔹 사용 후 개수 감소 
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

    // 🔹 기존 총알 개수에 추가 (최대 300발 제한)
    if (InventoryItems.Contains(EItemType::EIT_Bullet))
    {
        InventoryItems[EItemType::EIT_Bullet] = FMath::Min(InventoryItems[EItemType::EIT_Bullet] + BulletCount, 300);
    }
    else
    {
        InventoryItems.Add(EItemType::EIT_Bullet, FMath::Min(BulletCount, 300));
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

int32 UCInventoryComponent::UseBulletCount(int32 Amount)
{
    if (InventoryItems.Contains(EItemType::EIT_Bullet))
    {
        InventoryItems[EItemType::EIT_Bullet] += (Amount * -1);
        if (InventoryItems[EItemType::EIT_Bullet] <= 0)
        {
            InventoryItems[EItemType::EIT_Bullet] = 0;
            return 0;
        }
        return InventoryItems[EItemType::EIT_Bullet]; // 🔹 현재 총알 개수 반환
    }
    return 0;
}

//void UCInventoryComponent::EquipWeapon(EItemType WeaponType, ACPlayer* Player)
//{
//    if (!Player)
//    {
//        UE_LOG(LogTemp, Error, TEXT("❌ EquipWeapon 실패 - Player 또는 WeaponComponent가 없음"));
//        return;
//    }
//
//    UCWeaponComponent* WeaponComp = Cast<UCWeaponComponent>(Player->GetComponentByClass(UCWeaponComponent::StaticClass()));
//
//    // 🔹 EItemType을 WeaponClasses 배열의 인덱스로 변환
//    int32 WeaponIndex = WeaponComp->GetWeaponIndexFromItemType(WeaponType);
//
//    // 🔹 배열 범위 초과 방지
//    if (WeaponIndex < 0 || !WeaponComp->WeaponClasses.IsValidIndex(WeaponIndex))
//    {
//        UE_LOG(LogTemp, Error, TEXT("❌ EquipWeapon 실패 - 유효하지 않은 무기 인덱스! (%d)"), WeaponIndex);
//        return;
//    }
//
//    // ✅ 변환된 인덱스를 이용해 무기 장착
//    //WeaponComp->SetMode((EWeaponType)WeaponIndex);
//
//    UE_LOG(LogTemp, Warning, TEXT("✅ 무기 장착 완료: %d (WeaponIndex: %d)"), static_cast<int32>(WeaponType), WeaponIndex);
//}


