// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CSpawnVolume.h"
#include "CoinItem.h"
#include "SP_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AMyGameState::AMyGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectCoinCount = 0;
	LevelDuration = 30.f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f, //0.1초마다 업데이트
		true
		);
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
}

void AMyGameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
		}
	}
	
	SpawnedCoinCount = 0;
	CollectCoinCount = 0;

	//int32 ItemToSpawn = 30; // 기본값
	//if (ItemsPerLevel.IsValidIndex(CurrentLevelIndex)) {
	//	ItemToSpawn = ItemsPerLevel[CurrentLevelIndex]; // 현재 레벨에 맞는 아이템 개수 적용
	//}

	//// 현재 맵의 SpawnVolume을 찾아 XX개 아이템 스폰
	//TArray<AActor*> FoundVolumes;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnVolume::StaticClass(),FoundVolumes);
	//
	//for (int32 i = 0; i < ItemToSpawn; i++)
	//{
	//	if (FoundVolumes.Num() > 0)
	//	{
	//		if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
	//		{
	//			AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
	//			if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass())) // CoinItem의 하위 클래스인 Big, Small도 인식된다
	//			{
	//				SpawnedCoinCount++;
	//			}
	//		}
	//	}
	//}

	UpdateHUD();
	
	// 30초 후 OnLevelTimeUp() 실행
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AMyGameState::OnLevelTimeUp,
		LevelDuration,
		false
		);
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Yellow,
		FString::Printf(TEXT("✅ Wave 레벨 %d 시작 , ➡ 소환된 비트코인 %d 사토시"),CurrentLevelIndex+1, SpawnedCoinCount));
}

void AMyGameState::OnLevelTimeUp()
{
	// 시간 오버 처리
	EndLevel();
	
}

void AMyGameState::OnCoinCollected()
{
	CollectCoinCount++;
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Green,
		FString::Printf(TEXT("Coin collected: %d / %d"),CollectCoinCount, SpawnedCoinCount));

	if (SpawnedCoinCount > 0 && CollectCoinCount >= SpawnedCoinCount) {
		EndLevel();
	}
}

void AMyGameState::EndLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			AddScore(Score);
			CurrentLevelIndex++;
			MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(),LevelMapNames[CurrentLevelIndex]); // 레벨 이름 가져와서 오픈해라
	}
	else
	{
		OnGameOver();
	}
}

void AMyGameState::UpdateHUD() const
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASP_PlayerController* SP_PlayerController = Cast<ASP_PlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SP_PlayerController->GetHUDWidget())
			{
				//시간
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f 초"), RemainingTime)));
				}
				//점수
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
						}
					}
				}
				// 레벨 인덱스
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex+1)));
				}
			}
		}
	}
}

void AMyGameState::OnGameOver()
{
	UpdateHUD(); // 어느 위치에 놓느냐에 따라 갱신 위치가 달라진다.
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!!"));
}
