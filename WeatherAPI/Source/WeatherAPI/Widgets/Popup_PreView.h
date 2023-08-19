// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Popup_PreView.generated.h"

/**
 * 
 */
class UDataTable;
class UScene_Manager;

UCLASS()
class WEATHERAPI_API UPopup_PreView : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void FirstSetTodayMessage(FString csv_string);

	//Called BP
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim();//위젯 애님 이벤트에 바인딩 된 함수.

private:
	UFUNCTION()
		void SetTodayMessage(); //타이머용 내부 함수.

	//PlayAnimation() 래핑.
	UFUNCTION()
		void PlayMainAnim(UWidgetAnimation* mainanim);

	UFUNCTION()
		void OnClicked_Button_Close();

	//PlayAnimation() 래핑을 위한 델리게이트&핸들.
	FTimerDelegate animplayDel;
	FTimerHandle animplayHandle;

	int32 todaymessage_index;
	int32 maxindex;

	UPROPERTY()
		UDataTable* DT;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_maintext;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_subtext;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* MainAnim;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_Close;

	UScene_Manager* Scene_Manager;
};
