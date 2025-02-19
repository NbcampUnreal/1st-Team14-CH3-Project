#include "CGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CCharacter.h"

ACGameState::ACGameState()
{
    CurrentState = EGameState::Menu; // 게임 시작 시 메뉴 상태
}

void ACGameState::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(EGameState::Menu); // 게임 시작 시 메뉴 상태로 초기화
}

void ACGameState::SetGameState(EGameState NewState)
{
    CurrentState = NewState;

    switch (CurrentState)
    {
    case EGameState::Menu:
        UE_LOG(LogTemp, Warning, TEXT("게임 상태: 메뉴"));
        break;

    case EGameState::CityMap:
        UE_LOG(LogTemp, Warning, TEXT("도시 맵 로드"));
        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/PA_UrbanCity/Maps/Map_Post-Apocalyptic_NightLight"));
        break;

    case EGameState::MercenaryBattle:
        UE_LOG(LogTemp, Warning, TEXT("용병단과 전투 시작"));
        break;

    case EGameState::ScoreCheck:
        UE_LOG(LogTemp, Warning, TEXT("점수 체크 진행"));
        break;

    case EGameState::ElevatorSelection:
        UE_LOG(LogTemp, Warning, TEXT("엘리베이터 선택"));
        break;

    case EGameState::Labyrinth:
        UE_LOG(LogTemp, Warning, TEXT("연구소 미로 맵 로드"));
        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/LapMap/ModSci_Engineer/Maps/MAIN_MAP"));
        break;

    case EGameState::BossArea:
        UE_LOG(LogTemp, Warning, TEXT("보스가 있는 연구소 맵 로드"));
        UGameplayStatics::OpenLevel(this, TEXT("/Game/Map/LapMap/ModSci_Engineer/Maps/MAIN_MAP"));
        break;

    case EGameState::BossFight:
        UE_LOG(LogTemp, Warning, TEXT("보스전 시작"));
        break;

    case EGameState::Symbiosis:
        UE_LOG(LogTemp, Warning, TEXT("심비오 공생"));
        break;

    case EGameState::Ending1:
        UE_LOG(LogTemp, Warning, TEXT("엔딩1: 새로운 인류"));
        break;

    case EGameState::Ending2:
        UE_LOG(LogTemp, Warning, TEXT("엔딩2: 자유로 돌아가다"));
        break;

    case EGameState::GameOver:
        UE_LOG(LogTemp, Warning, TEXT("게임 오버"));
        break;

    default:
        break;
    }
}

//void ACGameState::SetGameState(EGameState NewState)
//{
//    if (NewState == EGameState::Playing)
//    {
//        // 🔹 게임 인스턴스에 현재 체력 저장
//        ACCharacter* Player = Cast<ACCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
//        if (Player)
//        {
//            Player->SaveHealthToGameInstance();
//        }
//
//        // 🔹 다음 맵 로드
//        UGameplayStatics::OpenLevel(GetWorld(), "NextLevel");
//    }
//
//    CurrentState = NewState;
//}