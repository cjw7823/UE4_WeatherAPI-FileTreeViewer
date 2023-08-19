// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableText.h"
#include "Components/Button.h"
#include "Components/ListView.h"

#include "CustomListView.h"

#include "Message_Manager.generated.h"

/**
 * 
 */
UCLASS()
class WEATHERAPI_API UMessage_Manager : public UUserWidget
{
	GENERATED_BODY()

public:
	UMessage_Manager(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
	void ActivateTextEditer(ESlateVisibility visible);
	void OnClicked_ListItem(UObject* obj);

	UFUNCTION()
		void OnClicked_Button_Input();
	UFUNCTION()
		void OnClicked_Button_AddText();

	UPROPERTY(meta = (bindwidget))
		UMultiLineEditableText* MainText;
	UPROPERTY(meta = (bindwidget))
		UMultiLineEditableText* SubText;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_Input;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_AddText;

	UPROPERTY(meta = (bindwidget))
		UCustomListView* ListView_0;
	UObject* selected_item;
};
