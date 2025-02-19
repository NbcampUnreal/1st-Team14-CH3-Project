// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameState.h"

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