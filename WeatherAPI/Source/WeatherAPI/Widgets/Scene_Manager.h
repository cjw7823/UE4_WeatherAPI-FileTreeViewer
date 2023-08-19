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

	//���ΰ� ��۷� ������.
	void PopupActivate_NewScene();
	//���ΰ� ��۷� ������.
	void PopupActivate_PreView();

	UFUNCTION()
		void OnClicked_Button_LoadScene();
	UFUNCTION()
		void OnClicked_Button_PreView();
	UFUNCTION()
		void OnClicked_Button_SaveScene_AnotherName();
	//�ش� �Լ��� BP���� ����. ���� ������ BP�Լ��� �̾����.(2023/01/06 ��������.)
	UFUNCTION(BlueprintCallable)
		void OnClicked_Button_SaveScene();
	UFUNCTION()
		void OnClicked_Button_NewScene();
	UFUNCTION()
		void OnVisibilityChanged_BP_LoadScene(ESlateVisibility InVisibility);
	
	UFUNCTION(BlueprintCallable)
		void ResetText();

	//������ ��û����. ������� ���� ������ ���� ���� ���.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FString GetSceneName();

private:
	bool GetOpenFileDialog(FString DirectoryPath, TArray<FString>& Directory);
	bool GetSaveFileDialog(FString DirectoryPath, TArray<FString>& Directory);

public:
	//BP���� �� ����.
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
