// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Popup_Save.generated.h"


class UScene_Manager;

UCLASS()
class WEATHERAPI_API UPopup_Save : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void SaveMessageScene(FString save_path, FString contents);
	UFUNCTION(BlueprintCallable)
		void SaveMessageCSV(FString save_path, UScene_Manager* parent);

};