#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CInventoryComponent.h"
#include "CWBP_CInventory.h"
#include "CPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UCWBP_CInventory;

UCLASS()
class START_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SwitchViewAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* QuitAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ToggleInventoryAction;

	// HUD 위젯 블루프린트 클래스 (게임 플레이 시)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// 메인 메뉴 위젯 블루프린트 클래스 (메인 메뉴 레벨 시)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UFUNCTION()
	void UpdateInventoryUI();  // 🔹 UI 업데이트 함수 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCWBP_CInventory> InventoryWidgetClass;

	UPROPERTY()
	UCWBP_CInventory* InventoryWidget;

	bool bIsInventoryOpen;
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* CurrentWidget; //	현재 화면에 보이는 위젯

};
