// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadScene.h"

#include "MyExpandableArea.h"
#include "File_Descriptor.h"

ULoadScene::ULoadScene(const FObjectInitializer& obj) : Super(obj)
{
	static ConstructorHelpers::FClassFinder<UFile_Descriptor> file_widget(TEXT("WidgetBlueprint'/LoadScenePlugin/BP_File_Descriptor.BP_File_Descriptor_C'"));
	subclass = file_widget.Class;
}

void ULoadScene::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Load->OnClicked.AddDynamic(this, &ULoadScene::OnClicked_Btn_Load);
	Btn_Cancel->OnClicked.AddDynamic(this, &ULoadScene::OnClicked_Btn_Cancel);

	loaded_file_path = FString();
	load_success = false;

	FindTreeList(Scene_Directory, scene_path);
}

void ULoadScene::FindTreeList(UMyExpandableArea* parents, FString file_path)
{
	TArray<FString> dir_names = GetFileList(file_path, true, true);

	if (dir_names.Num() == 0)
		return;

	for (auto dir_name : dir_names)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, file_path);

		if (dir_name.Contains("."))
		{
			//확장자 제거
			FString none_ext = dir_name.Left(dir_name.Find("."));
			//'[scene]'제거
			FString file_name = none_ext.RightChop(7);
			
			AddFile(parents, FText::FromString(file_name), file_path + "/" + dir_name);
		}
		else
		{
			auto temp = AddDirectory(parents, FText::FromString(dir_name));
			FindTreeList(temp, file_path + "/" + dir_name);
		}
	}
}

void ULoadScene::ReloadList()
{
	Scene_Directory->ResetDir();
	FindTreeList(Scene_Directory, scene_path);
}

TArray<FString> ULoadScene::GetFileList(FString RootFolderFullPath, bool find_files, bool find_dir)
{
	FPaths::NormalizeDirectoryName(RootFolderFullPath);
	TArray<FString> Files;
	Files.Empty();

	FString FinalPath = RootFolderFullPath + "/*";
	IFileManager::Get().FindFiles(Files, *FinalPath, find_files, find_dir);

	return Files;
}

UMyExpandableArea* ULoadScene::AddDirectory(UMyExpandableArea* parents, FText dir_name)
{
	return parents->AddDirectory(dir_name);
}

void ULoadScene::AddFile(UMyExpandableArea* parents, FText file_name, FString _path)
{
	auto temp = CreateWidget<UFile_Descriptor>(this, subclass);
	temp->pLoadScene = this;
	temp->TextBlock_Header->SetText(file_name);
	temp->path = _path;
	parents->AddFile(temp);
}

void ULoadScene::OnClicked_Btn_Load()
{
	load_success = true;

	SetVisibility(ESlateVisibility::Collapsed);
	LoadSceneName->SetText(FText::FromString(TEXT("불러올 씬 이름")));
}

void ULoadScene::OnClicked_Btn_Cancel()
{
	load_success = false;

	SetVisibility(ESlateVisibility::Collapsed);
	LoadSceneName->SetText(FText::FromString(TEXT("불러올 씬 이름")));
}

FString ULoadScene::BP_LoadScene(ESlateVisibility InVisibility, bool& isSuccess)
{
	ReloadList();

	if (InVisibility != ESlateVisibility::Visible
		&& this->load_success)
	{
		isSuccess = true;
		return this->loaded_file_path;
	}

	isSuccess = false;
	return FString();
}
