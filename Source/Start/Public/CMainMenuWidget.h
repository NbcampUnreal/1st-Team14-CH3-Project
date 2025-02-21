// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMainMenuWidget.generated.h"

/**
 * 
 */

 // ���� ���� ��û ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGameRequested);
// ���� ���� ��û ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitGameRequested);

UCLASS()
class START_API UCMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// �ٸ� �ý��ۿ��� �� �̺�Ʈ�� ���ε��Ͽ� �޴� Ŭ���� ó���� �� ����
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStartGameRequested OnStartGameRequested;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnQuitGameRequested OnQuitGameRequested;

protected:
	virtual void NativeConstruct() override;

	// UMG���� ���ε��� ��ư�� (���� �������Ʈ�� ���� �̸��� ����ؾ� ��)
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	// ��ư Ŭ�� �̺�Ʈ �ڵ鷯
	UFUNCTION()
	void OnStartGameButtonClicked();
	UFUNCTION()
	void OnQuitClicked();

	// �����ͳ� �������Ʈ���� ������ �� �ֵ��� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
	FName LevelName = FName("Map_Post-Apocalyptic_NightLight");
};
