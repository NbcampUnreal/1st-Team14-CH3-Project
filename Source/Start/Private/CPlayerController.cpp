#include "CPlayerController.h"
#include "CBaseItem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

ACPlayerController::ACPlayerController()
{
	InputMappingContext = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	JumpAction = nullptr;
	RunAction = nullptr;
	CrouchAction = nullptr;
	SwitchViewAction = nullptr;
	QuitAction = nullptr;
	ToggleInventoryAction = nullptr;
	EquipRifleAction = nullptr;
	EquipPistolAction = nullptr;
	EquipKnifeAction = nullptr;
	PickupItemAction = nullptr;
	InventoryWidget = nullptr;
	bIsInventoryOpen = false;
	CachedPawn = nullptr;
	InventoryComponent = nullptr;
	FireAction = nullptr;

	HUDWidgetClass = nullptr;
	MainMenuWidgetClass = nullptr;
	CurrentWidget = nullptr;
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// ✅ Pawn 변경 감지를 위한 타이머 설정 (1초마다 체크)
	GetWorld()->GetTimerManager().SetTimer(
		DelegateCheckTimerHandle,
		this,
		&ACPlayerController::CheckPawnAndUpdateDelegate,
		1.0f,
		true
	);

	// UI 설정
	UWorld* World = GetWorld();
	if (!World) return;

	FString CurrentLevelName = World->GetMapName();
	UE_LOG(LogTemp, Log, TEXT("Current Level: %s"), *CurrentLevelName);

	if (CurrentLevelName.Contains(TEXT("MenuLevel")))
	{
		if (MainMenuWidgetClass)
		{
			CurrentWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport(100);
				CurrentWidget->SetIsFocusable(true);
				
				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);
				bShowMouseCursor = true;
			}
		}
	}
	else
	{
		if (HUDWidgetClass)
		{
			CurrentWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
				FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
				bShowMouseCursor = false;
			}
		}
	}
}

// ✅ Pawn 변경 감지 및 델리게이트 재설정
void ACPlayerController::CheckPawnAndUpdateDelegate()
{
	APawn* CurrentPawn = GetPawn();
	if (CurrentPawn != CachedPawn)
	{
		CachedPawn = CurrentPawn;
		if (CachedPawn)
		{
			// ✅ 클래스 멤버 변수를 사용하도록 `this->InventoryComponent`로 명확하게 지정
			this->InventoryComponent = CachedPawn->FindComponentByClass<UCInventoryComponent>();
			if (this->InventoryComponent)
			{
				// ✅ 기존 바인딩 제거 후 다시 바인딩
				this->InventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &ACPlayerController::UpdateInventoryUI);
				this->InventoryComponent->OnInventoryUpdated.AddDynamic(this, &ACPlayerController::UpdateInventoryUI);
			}
		}
	}
}


// 🔹 UI 업데이트 함수 추가
void ACPlayerController::UpdateInventoryUI()
{
	if (InventoryWidget && GetPawn())
	{
		// ✅ 클래스 멤버 변수를 명확히 사용하기 위해 `this->InventoryComponent` 사용
		this->InventoryComponent = GetPawn()->FindComponentByClass<UCInventoryComponent>();
		if (this->InventoryComponent)
		{
			InventoryWidget->UpdateInventory();
			UE_LOG(LogTemp, Warning, TEXT("🔹 인벤토리 UI 업데이트됨"));
		}
	}
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Triggered, this, &ACPlayerController::ToggleInventory);
		EnhancedInputComponent->BindAction(PickupItemAction, ETriggerEvent::Triggered, this, &ACPlayerController::PickupItem);

		// ✅ 왼쪽 클릭 입력을 받을 때 UI가 열려있으면 동작하지 않도록 예외 처리
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ACPlayerController::HandleLeftClick);
	}
}

void ACPlayerController::HandleLeftClick()
{
	if (bIsInventoryOpen)
	{
		// ✅ UI에 클릭할 수 있는 버튼이 있는지 체크
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		if (HitResult.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("🔹 UI 클릭 감지됨 - 입력 허용"));
			return; // 🚨 UI 위에서 클릭한 경우 입력을 허용함
		}

		UE_LOG(LogTemp, Warning, TEXT("❌ 인벤토리가 열려 있어서 왼쪽 클릭 입력을 무시함!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("🔫 총 발사 실행"));
	// 원래의 총 발사 기능 실행 코드...
}


void ACPlayerController::ToggleInventory()
{
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryWidgetClass가 설정되지 않음!"));
		return;
	}

	UCInventoryComponent* PawnInventoryComponent = nullptr;
	if (APawn* MyPawn = GetPawn())
	{
		PawnInventoryComponent = MyPawn->FindComponentByClass<UCInventoryComponent>();
	}

	if (bIsInventoryOpen)
	{
		if (InventoryWidget && InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly()); // UI 닫을 때 입력 모드 변경
			UE_LOG(LogTemp, Warning, TEXT("🔹 인벤토리 닫기"));
		}

		if (PawnInventoryComponent)
		{
			PawnInventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &ACPlayerController::UpdateInventoryUI);
			UE_LOG(LogTemp, Warning, TEXT("🛑 InventoryComponent 델리게이트 해제됨."));
		}
	}
	else
	{
		if (!InventoryWidget)
		{
			InventoryWidget = CreateWidget<UCWBP_CInventory>(this, InventoryWidgetClass);

			if (!InventoryWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("❌ InventoryWidget 생성 실패!"));
				return;
			}
		}

		if (PawnInventoryComponent)
		{
			InventoryWidget->InitializeInventory(PawnInventoryComponent);
			if (!InventoryComponent->OnInventoryUpdated.IsBound())
			{
				PawnInventoryComponent->OnInventoryUpdated.AddDynamic(this, &ACPlayerController::UpdateInventoryUI);
				UE_LOG(LogTemp, Warning, TEXT("✅ InventoryComponent 델리게이트 추가됨."));
			}
		}

		if (!InventoryWidget->IsInViewport())
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			bShowMouseCursor = true;

			// ✅ UI 입력 모드로 변경하여 클릭 이벤트가 정상 작동하도록 설정
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);

			UE_LOG(LogTemp, Warning, TEXT("✅ InventoryWidget 화면에 추가됨"));
		}
	}

	bIsInventoryOpen = !bIsInventoryOpen;
	UE_LOG(LogTemp, Warning, TEXT("현재 인벤토리 상태: %s"), bIsInventoryOpen ? TEXT("열림") : TEXT("닫힘"));
}



void ACPlayerController::PickupItem()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn을 찾을 수 없습니다."));
		return;
	}

	FVector PawnLocation = MyPawn->GetActorLocation();
	float PickupRadius = 200.0f; // 적절한 반경 설정

	// Overlap 결과를 AActor 배열로 받음
	TArray<AActor*> OverlappedActors;

	// 객체 타입 설정: 아이템이 속한 Collision ObjectType을 지정 (예시로 ECC_WorldDynamic 사용)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	// 제외할 액터 (플레이어)
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MyPawn);

	// SphereOverlapActors 호출
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		PawnLocation,
		PickupRadius,
		ObjectTypes,
		ACBaseItem::StaticClass(), // ACBaseItem 클래스만 검색
		IgnoreActors,
		OverlappedActors
	);

	// 디버그용 구체 그리기
	DrawDebugSphere(GetWorld(), PawnLocation, PickupRadius, 32, FColor::Green, false, 2.0f);

	if (OverlappedActors.Num() > 0)
	{
		ACBaseItem* NearestItem = nullptr;
		float MinDistanceSq = FLT_MAX;

		for (AActor* Actor : OverlappedActors)
		{
			ACBaseItem* Item = Cast<ACBaseItem>(Actor);
			if (Item)
			{
				float DistSq = FVector::DistSquared(PawnLocation, Item->GetActorLocation());
				if (DistSq < MinDistanceSq)
				{
					MinDistanceSq = DistSq;
					NearestItem = Item;
				}
			}
		}

		if (NearestItem)
		{
			bool bPickedUp = NearestItem->PutIntoInventory(MyPawn);
			if (bPickedUp)
			{
				UE_LOG(LogTemp, Warning, TEXT("아이템 %d(이)가 인벤토리에 추가되었습니다."), static_cast<int32>(NearestItem->GetItemType()));
				// 클릭 한 번 후 즉시 UI 업데이트를 강제
				UpdateInventoryUI();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("아이템 줍기에 실패했습니다."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("주변에 줍을 아이템이 없습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("주변에 줍을 아이템이 없습니다."));
	}
}

