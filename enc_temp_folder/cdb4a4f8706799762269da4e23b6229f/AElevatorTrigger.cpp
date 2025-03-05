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

    // 앨리베이터 사운드 재생
    if (ElevatorSound)
    {
        // 플레이어 움직임 제한
        PlayerCharacter->DisableInput(Cast<APlayerController>(PlayerCharacter->GetController()));

        // 필요한 경우 여기서 플레이어의 대기 애니메이션 재생
        // PlayerCharacter->PlayWaitAnimation();

        float SoundDuration = ElevatorSound->GetDuration();
        UGameplayStatics::PlaySound2D(this, ElevatorSound);

		// 연구소 미로(N, Labyinth)인지 보스 연구소(BossArea)인지에 따라 다음 레벨 설정
        EGameState NewState = bIsN_Elevator ? EGameState::Labyrinth : EGameState::BossArea;

        // 사운드 재생이 끝난 후 레벨 전환
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewState]() {
            LoadNextLevel(NewState);
            }, SoundDuration, false);
    }

}

void AAElevatorTrigger::LoadNextLevel(EGameState NewState)
{
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