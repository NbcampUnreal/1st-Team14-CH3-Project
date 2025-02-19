#include "AElevatorTrigger.h"
#include "Components/BoxComponent.h"
#include "CGameState.h"
#include "CPlayer.h"
#include "Kismet/GameplayStatics.h"

AAElevatorTrigger::AAElevatorTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    // 🔹 트리거 박스 컴포넌트 생성
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = BoxComponent;

    // 🔹 오버랩 이벤트 연결
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAElevatorTrigger::OnOverlap);
}

void AAElevatorTrigger::BeginPlay()
{
    Super::BeginPlay();
}

void AAElevatorTrigger::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // 🔹 내가 만든 플레이어 캐릭터인지 확인
    ACPlayer* PlayerCharacter = Cast<ACPlayer>(OtherActor);
    if (!PlayerCharacter) return; 

    // 🔹 CGameState 가져오기
    ACGameState* GameState = GetWorld()->GetGameState<ACGameState>();
    if (GameState)
    {
        if (bIsN_Elevator)
        {
            UE_LOG(LogTemp, Warning, TEXT("N엘리베이터 선택 → 연구소 미로 이동"));
            GameState->SetGameState(EGameState::Labyrinth);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("R엘리베이터 선택 → 보스 연구소 이동"));
            GameState->SetGameState(EGameState::BossArea);
        }
    }
}


