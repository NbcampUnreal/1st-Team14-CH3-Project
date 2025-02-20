// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMainMenuWidget.generated.h"

/**
 * 
 */

 // 게임 시작 요청 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGameRequested);
// 게임 종료 요청 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitGameRequested);

UCLASS()
class START_API UCMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 다른 시스템에서 이 이벤트를 바인딩하여 메뉴 클릭을 처리할 수 있음
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStartGameRequested OnStartGameRequested;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnQuitGameRequested OnQuitGameRequested;

protected:
	virtual void NativeConstruct() override;

	// UMG에서 바인딩된 버튼들 (위젯 블루프린트와 동일 이름을 사용해야 함)
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	// 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnStartGameButtonClicked();
	UFUNCTION()
	void OnQuitClicked();

	// 에디터나 블루프린트에서 수정할 수 있도록 함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
	FName LevelName = FName("Map_Post-Apocalyptic_NightLight");
};
