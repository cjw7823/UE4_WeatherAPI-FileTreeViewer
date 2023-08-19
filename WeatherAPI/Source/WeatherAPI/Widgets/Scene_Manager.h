// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Scene_Manager.generated.h"

class UDataTable;
class UMessage_Manager;
class UPopup_NewScene;
class UPopup_PreView;
class UPopup_Save;
class ULoadScene;

UCLASS()
class WEATHERAPI_API UScene_Manager : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//내부가 토글로 구현됨.
	void PopupActivate_NewScene();
	//내부가 토글로 구현됨.
	void PopupActivate_PreView();

	UFUNCTION()
		void OnClicked_Button_LoadScene();
	UFUNCTION()
		void OnClicked_Button_PreView();
	UFUNCTION()
		void OnClicked_Button_SaveScene_AnotherName();
	//해당 함수는 BP에서 구현. 기존 내용은 BP함수에 이어붙임.(2023/01/06 수정내용.)
	UFUNCTION(BlueprintCallable)
		void OnClicked_Button_SaveScene();
	UFUNCTION()
		void OnClicked_Button_NewScene();
	UFUNCTION()
		void OnVisibilityChanged_BP_LoadScene(ESlateVisibility InVisibility);
	
	UFUNCTION(BlueprintCallable)
		void ResetText();

	//수정님 요청사항. 재생중인 씬의 편집을 막기 위해 사용.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FString GetSceneName();

private:
	bool GetOpenFileDialog(FString DirectoryPath, TArray<FString>& Directory);
	bool GetSaveFileDialog(FString DirectoryPath, TArray<FString>& Directory);

public:
	//BP에서 값 설정.
	UPROPERTY(BlueprintReadWrite)
		FString GI_save_path;

	UPROPERTY()
		UDataTable* DT;
	UPROPERTY(meta = (bindwidget))
		UMessage_Manager* Message_Manager;
	UPROPERTY(meta = (bindwidget))
		UPopup_NewScene* Popup_NewScene;
	UPROPERTY(meta = (bindwidget))
		UPopup_PreView* Popup_PreView_;
	UPROPERTY(meta = (bindwidget))
		UPopup_Save* Popup_Save;
	UPROPERTY(meta = (bindwidget), BlueprintReadOnly)
		ULoadScene* BP_LoadScene;

	UPROPERTY(meta = (bindwidget))
		UButton* Button_LoadScene;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_PreView;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_SaveScene_AnotherName;
	UPROPERTY(BlueprintReadWrite,meta = (bindwidget))
		UButton* Button_SaveScene;
	UPROPERTY(meta = (bindwidget))
		UButton* Button_NewScene;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_SceneTitle;
	
	bool is_loaded_scene;
	FString loaded_CSV_path;
};
