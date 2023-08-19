// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TodaysMessage_ListItem.generated.h"

class UMessage_Manager;
class UListItem_Data;

UCLASS()
class WEATHERAPI_API UTodaysMessage_ListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void SetTextData(FText main, FText sub);
	void SetTextIndex();

	UFUNCTION()
		void OnClicked_Button_Delete();

	UPROPERTY()
		UMessage_Manager* parent;
	UPROPERTY()
		UListItem_Data* data;

	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_Index;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_Delete;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* MainText;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* SubText;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_BG;
};
