// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

#include "LoadScene.generated.h"

class UMyExpandableArea;
class UFile_Descriptor;

UCLASS()
class LOADSCENEPLUGIN_API ULoadScene : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULoadScene(const FObjectInitializer& obj);

	virtual void NativeConstruct() override;

	void FindTreeList(UMyExpandableArea* parents, FString file_path);
	void ReloadList();

	TArray<FString> GetFileList(FString RootFolderFullPath, bool find_files, bool find_dir);

	UMyExpandableArea* AddDirectory(UMyExpandableArea* parents, FText dir_name);
	void AddFile(UMyExpandableArea* parents, FText file_name, FString _path);

	UFUNCTION()
		void OnClicked_Btn_Load();
	UFUNCTION()
		void OnClicked_Btn_Cancel();

	UFUNCTION(BlueprintCallable, meta=(ToolTip = "OnVisibilityChanged에 바인딩해 사용.\nparam : 바인딩 할 이벤트의 InVisibility 연결.\nreturns : isSuccess가 참일 때, Return Value 유효"))
		FString BP_LoadScene(ESlateVisibility InVisibility, bool& isSuccess);

	UPROPERTY(BlueprintReadWrite)
		FString scene_path;
	UPROPERTY(meta = (bindwidget))
		UMyExpandableArea* Scene_Directory;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* LoadSceneName;

	UPROPERTY(meta = (bindwidget))
		UButton* Btn_Load;
	UPROPERTY(meta = (bindwidget))
		UButton* Btn_Cancel;

	TSubclassOf<UFile_Descriptor> subclass;

	//이 위젯의 계층 구조와 관계없이, 해당 변수를 읽어와 활용하면 된다.
	UPROPERTY(BlueprintReadOnly)
		FString loaded_file_path;
	UPROPERTY(BlueprintReadOnly)
		bool load_success;
};
