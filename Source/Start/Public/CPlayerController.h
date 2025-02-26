#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipRifleAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipPistolAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipKnifeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PickupItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ToggleAutoFireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ToggleZoomAction;

	// HUD 위젯 블루프린트 클래스 (게임 플레이 시)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// 메인 메뉴 위젯 블루프린트 클래스 (메인 메뉴 레벨 시)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	UFUNCTION()
	void PickupItem();
	UFUNCTION()
	void UpdateInventoryUI();  // 🔹 UI 업데이트 함수 추가

	UFUNCTION()
	void HandleLeftClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCWBP_CInventory> InventoryWidgetClass;

	UPROPERTY()
	UCWBP_CInventory* InventoryWidget;

	bool bIsInventoryOpen;

	UCWBP_CInventory* GetInventoryWidget() const { return InventoryWidget; }
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UCInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* CurrentWidget; //	현재 화면에 보이는 위젯

	// ✅ 클래스 멤버 변수로 InventoryComponent 추가

	// ✅ Pawn 변경 시 델리게이트 재설정용 캐시 변수
	APawn* CachedPawn;

	// ✅ Pawn 변경 감지를 위한 타이머
	FTimerHandle DelegateCheckTimerHandle;

	// ✅ Pawn 변경을 감지하고 델리게이트를 재설정하는 함수
	void CheckPawnAndUpdateDelegate();
};
