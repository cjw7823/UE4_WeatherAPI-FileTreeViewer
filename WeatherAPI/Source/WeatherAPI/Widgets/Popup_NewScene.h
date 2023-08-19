// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Popup_NewScene.generated.h"

/**
 * 
 */
class UScene_Manager;

UCLASS()
class WEATHERAPI_API UPopup_NewScene : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClicked_Button_Save();
	UFUNCTION()
		void OnClicked_Button_Reset();

	UPROPERTY(meta =(bindwidget))
		UButton* Button_Save;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_Reset;

	UScene_Manager* Scene_Manager;
};
