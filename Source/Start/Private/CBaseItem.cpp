#include "CBaseItem.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "CInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPlayer.h"

// 🔹 기본 생성자
ACBaseItem::ACBaseItem() :
    bCanUse(false),
    bConsumable(false)
{
    PrimaryActorTick.bCanEverTick = false;

    // ✅ RootComponent가 없으면 충돌 감지를 위한 UBoxComponent 생성
    if (!RootComponent)
    {
        InteractableCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableCollision"));
        RootComponent = InteractableCollision;  // ✅ RootComponent로 설정
        InteractableCollision->SetCollisionProfileName(TEXT("PhysicsActor"));
        InteractableCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        InteractableCollision->SetCollisionResponseToAllChannels(ECR_Block);
        InteractableCollision->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));
    }

    // ✅ StaticMeshComponent 생성 후 RootComponent에 부착
    if (!StaticMesh)
    {
        StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
        StaticMesh->SetupAttachment(RootComponent); // ✅ UBoxComponent에 부착
        StaticMesh->SetCollisionProfileName(TEXT("NoCollision")); // 메쉬 자체는 충돌 X
    }

    // ✅ 아이템 속성 초기화
    KeyPressedSound = nullptr;
    UseSound = nullptr;
    ItemIcon = nullptr;
}






// 🔹 인벤토리에 아이템 추가
bool ACBaseItem::PutIntoInventory(AActor* PlayerHavingInventory)
{
    ACPlayer* CPlayer = Cast<ACPlayer>(PlayerHavingInventory);
    if (CPlayer)
    {
        UCInventoryComponent* Inventory = CPlayer->FindComponentByClass<UCInventoryComponent>();
        if (Inventory)
        {
            bool bAdded = Inventory->AddToInventory(GetItemType());

            // ✅ 디버그 메시지 추가
            FString ItemName = UEnum::GetValueAsString(GetItemType());
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("인벤토리에 추가됨: %s"), *ItemName));
            if (bAdded)
            {
                if (KeyPressedSound)
                {
                    UGameplayStatics::PlaySoundAtLocation(this, KeyPressedSound, GetActorLocation());
                }
                UE_LOG(LogTemp, Warning, TEXT("아이템 %d 인벤토리에 추가됨!"), static_cast<int32>(GetItemType()));

                Inventory->PrintInventory(); // 🔹 인벤토리 상태 출력

                // 아이템을 인벤토리에 추가했으면, 월드에서 제거
                Destroy();
            }

            return bAdded;  // **return을 최종적으로 한 번만 사용**
        }
    }
    return false; // 플레이어나 인벤토리를 찾지 못하면 false 반환
}

// 🔹 아이템 사용
void ACBaseItem::Use(AActor* Target)
{
    if (UseSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, UseSound, GetActorLocation());
    }

    if (bConsumable)
    {
        ACPlayer* Player = Cast<ACPlayer>(Target);
        if (Player)
        {
            UCInventoryComponent* Inventory = Player->FindComponentByClass<UCInventoryComponent>();
            if (Inventory)
            {
                Inventory->RemoveItem(GetItemType());
            }
        }
    }
}

// 🔹 아이템 타입 반환
EItemType ACBaseItem::GetItemType() const
{
    return ItemType;
}

void ACBaseItem::BeginPlay()
{
    Super::BeginPlay();

    // ✅ 오버랩 이벤트 바인딩
    if (InteractableCollision)
    {
        InteractableCollision->OnComponentBeginOverlap.AddDynamic(this, &ACBaseItem::OnOverlap);
    }
}

void ACBaseItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACPlayer* Player = Cast<ACPlayer>(OtherActor);
    if (Player)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("아이템 감지됨!"));

        // ✅ 아이템을 자동으로 인벤토리에 추가
       /* PutIntoInventory(Player);*/
    }
}