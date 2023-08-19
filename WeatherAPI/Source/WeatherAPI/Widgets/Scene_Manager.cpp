// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene_Manager.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "CustomDataTable.h"
#include "ListItem_Data.h"
#include "Message_Manager.h"
#include "Popup_NewScene.h"
#include "Popup_PreView.h"
#include "Popup_Save.h"
#include "LoadScene.h"

#define OPENFILEPATH "/scene"
#define NEXTMESSAGE_TIME_MIN 1
#define NEXTMESSAGE_TIME_MIN_PREVIEW 0.05

void UScene_Manager::NativeConstruct()
{
	Super::NativeConstruct();

	Button_LoadScene->OnClicked.AddDynamic(this, &UScene_Manager::OnClicked_Button_LoadScene);
	Button_PreView->OnClicked.AddDynamic(this, &UScene_Manager::OnClicked_Button_PreView);
	Button_SaveScene_AnotherName->OnClicked.AddDynamic(this, &UScene_Manager::OnClicked_Button_SaveScene_AnotherName);
	//해당 함수는 BP에서 구현. 기존 내용은 BP함수에 이어붙임.(2023/01/06 수정내용.)
	//Button_SaveScene->OnClicked.AddDynamic(this, &UScene_Manager::OnClicked_Button_SaveScene);
	Button_NewScene->OnClicked.AddDynamic(this, &UScene_Manager::OnClicked_Button_NewScene);

	BP_LoadScene->OnVisibilityChanged.AddDynamic(this, &UScene_Manager::OnVisibilityChanged_BP_LoadScene);

	is_loaded_scene = false;
	loaded_CSV_path = FString();

	Popup_NewScene->SetVisibility(ESlateVisibility::Collapsed);
	Popup_PreView_->SetVisibility(ESlateVisibility::Collapsed);
	Popup_Save->SetVisibility(ESlateVisibility::Collapsed);
	BP_LoadScene->SetVisibility(ESlateVisibility::Collapsed);
}

void UScene_Manager::OnClicked_Button_LoadScene()
{
	TArray<FString> FileNames;

	if (is_loaded_scene)
	{
		OnClicked_Button_NewScene();
		return;
	}

	BP_LoadScene->SetVisibility(ESlateVisibility::Visible);

	/*
	//blocking 함수
	if (GetOpenFileDialog(GI_save_path + OPENFILEPATH, FileNames))
	{
		FString scene_file;
		FFileHelper::LoadFileToString(scene_file, *FileNames[0]);
		//json 파싱
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(scene_file);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(Reader, JsonObject);

		FString csvfile_path = JsonObject->GetStringField(TEXT("path"));

		FString csv_file;
		if (FFileHelper::LoadFileToString(csv_file, *csvfile_path))
		{
			loaded_CSV_path = csvfile_path;
			FString filename_without_path = FPaths::GetCleanFilename(FileNames[0]);
			TextBlock_SceneTitle->SetText(FText::FromString(filename_without_path));

			DT = NewObject<UDataTable>(GetTransientPackage(), FName(TEXT("CSV_DT")));
			DT->RowStruct = FCustomDataTable::StaticStruct();

			auto error = DT->CreateTableFromCSVString(csv_file);// '임포트' 중 에러 항목 리턴

			int32 raw_count = 0;
			while (true)
			{
				raw_count++;
				FName index = FName(FString::FromInt(raw_count));
				FString debuging;
				FCustomDataTable* today_message = DT->FindRow<FCustomDataTable>(index, debuging);
				if (today_message == nullptr)
					break;
				FString temp1 = today_message->MainText.Replace(L"\\n", L"\n");
				FString temp2 = today_message->SubText.Replace(L"\\n", L"\n");

				UListItem_Data* item = NewObject<UListItem_Data>();
				item->parent = Message_Manager;
				item->data = item;
				item->Main = FText::FromString(temp1);
				item->Sub = FText::FromString(temp2);
				Message_Manager->ListView_0->AddItem(item);
			}

			is_loaded_scene = true;
		}
	}
	*/
}

void UScene_Manager::OnClicked_Button_PreView()
{
	PopupActivate_PreView();

	FString csv_string = FString("Index,MainText,SubText,Time_min\n");

	UListView* listview = Message_Manager->ListView_0;

	for (int i = 0; ; i++)
	{
		UListItem_Data* data = Cast<UListItem_Data>(listview->GetItemAt(i));
		if (data == nullptr)
			break;

		FString temp1 = data->Main.ToString().Replace(L"\n", L"\\n");
		FString temp2 = data->Sub.ToString().Replace(L"\n", L"\\n");

		csv_string += FString::FromInt(i + 1) + ",\"" + temp1 + "\",\"" + temp2 + "\"," + FString::FromInt(NEXTMESSAGE_TIME_MIN_PREVIEW) + "\n";
	}

	//GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Purple, csv_string);
	Popup_PreView_->FirstSetTodayMessage(csv_string);
	Popup_PreView_->SetVisibility(ESlateVisibility::Visible);
}

void UScene_Manager::OnClicked_Button_SaveScene_AnotherName()
{
	Popup_Save->SetVisibility(ESlateVisibility::Visible);
}

void UScene_Manager::OnClicked_Button_SaveScene()
{
	//기존 씬 수정중.
	if (is_loaded_scene)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, "OnClicked_Button_SaveScene");

		FString message_scene = FString("Index,MainText,SubText,Time_min\n");

		UListView* listview = Message_Manager->ListView_0;

		for (int i = 0; ; i++)
		{
			UListItem_Data* data = Cast<UListItem_Data>(listview->GetItemAt(i));
			if (data == nullptr)
				break;

			FString temp1 = data->Main.ToString().Replace(L"\n", L"\\n");
			FString temp2 = data->Sub.ToString();

			message_scene += FString::FromInt(i + 1) + ",\"" + temp1 + "\",\"" + temp2 + "\"," + FString::FromInt(NEXTMESSAGE_TIME_MIN) + "\n";
		}

		if(loaded_CSV_path != FString())
			FFileHelper::SaveStringToFile(message_scene, *loaded_CSV_path);
		else
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, "Fail_SaveStringToFile() cuz loaded_Scene_path is NULL");

	}
	//신규 씬 저장. ( 다른이름으로 저장과 같음.)
	else
	{
		OnClicked_Button_SaveScene_AnotherName();
	}
}

void UScene_Manager::OnClicked_Button_NewScene()
{
	//신규 씬을 편집중이고, ListView에 편집중인 내용이 없으면 동작x
	if (is_loaded_scene || Message_Manager->ListView_0->GetNumItems() != 0)
	{
		PopupActivate_NewScene();
	}
	else
	{
		TextBlock_SceneTitle->SetText(FText::FromString(TEXT("신규 씬 구성 진행 중")));
		is_loaded_scene = false;
		loaded_CSV_path = FString();
		Message_Manager->ListView_0->ClearListItems();
		Message_Manager->ActivateTextEditer(ESlateVisibility::Collapsed);
	}
}

void UScene_Manager::OnVisibilityChanged_BP_LoadScene(ESlateVisibility InVisibility)
{
	BP_LoadScene->ReloadList();

	if (InVisibility != ESlateVisibility::Visible
		&& BP_LoadScene->load_success)
	{
		auto flie_path = BP_LoadScene->loaded_file_path;
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Purple, flie_path);
	
		FString scene_file;
		FFileHelper::LoadFileToString(scene_file, *flie_path);
		//json 파싱
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(scene_file);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(Reader, JsonObject);

		FString csvfile_path = JsonObject->GetStringField(TEXT("path"));

		FString csv_file;
		if (FFileHelper::LoadFileToString(csv_file, *csvfile_path))
		{
			loaded_CSV_path = csvfile_path;
			FString filename_without_path = FPaths::GetCleanFilename(flie_path);
			TextBlock_SceneTitle->SetText(FText::FromString(filename_without_path));

			DT = NewObject<UDataTable>(GetTransientPackage(), FName(TEXT("CSV_DT")));
			DT->RowStruct = FCustomDataTable::StaticStruct();

			auto error = DT->CreateTableFromCSVString(csv_file);// '임포트' 중 에러 항목 리턴

			int32 raw_count = 0;
			while (true)
			{
				raw_count++;
				FName index = FName(FString::FromInt(raw_count));
				FString debuging;
				FCustomDataTable* today_message = DT->FindRow<FCustomDataTable>(index, debuging);
				if (today_message == nullptr)
					break;
				FString temp1 = today_message->MainText.Replace(L"\\n", L"\n");
				FString temp2 = today_message->SubText.Replace(L"\\n", L"\n");

				UListItem_Data* item = NewObject<UListItem_Data>();
				item->parent = Message_Manager;
				item->data = item;
				item->Main = FText::FromString(temp1);
				item->Sub = FText::FromString(temp2);
				Message_Manager->ListView_0->AddItem(item);
			}

			is_loaded_scene = true;
		}
	}
}

void UScene_Manager::ResetText()
{
	if (is_loaded_scene == false)
	{
		Message_Manager->MainText->SetText(FText());
		Message_Manager->SubText->SetText(FText());
		Message_Manager->ListView_0->ClearListItems();
	}
}

FString UScene_Manager::GetSceneName()
{
	FString temp = TextBlock_SceneTitle->GetText().ToString();

	//확장자 제거
	FString none_ext = temp.Left(temp.Find("."));
	//'[scene]'제거
	FString file_name = none_ext.RightChop(7);

	return file_name;
}

void UScene_Manager::PopupActivate_NewScene()
{
	if (Popup_NewScene->GetVisibility() == ESlateVisibility::Visible)
	{
		Popup_NewScene->SetVisibility(ESlateVisibility::Collapsed);
		Message_Manager->SetVisibility(ESlateVisibility::Visible);
		Button_LoadScene->SetVisibility(ESlateVisibility::Visible);
		Button_NewScene->SetVisibility(ESlateVisibility::Visible);
		Button_PreView->SetVisibility(ESlateVisibility::Visible);
		Button_SaveScene_AnotherName->SetVisibility(ESlateVisibility::Visible);
		Button_SaveScene->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Popup_NewScene->SetVisibility(ESlateVisibility::Visible);
		Message_Manager->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_LoadScene->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_NewScene->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_PreView->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_SaveScene_AnotherName->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_SaveScene->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UScene_Manager::PopupActivate_PreView()
{
	if (Popup_PreView_->GetVisibility() == ESlateVisibility::Visible)
	{
		Popup_PreView_->SetVisibility(ESlateVisibility::Collapsed);
		Message_Manager->SetVisibility(ESlateVisibility::Visible);
		Button_LoadScene->SetVisibility(ESlateVisibility::Visible);
		Button_NewScene->SetVisibility(ESlateVisibility::Visible);
		Button_PreView->SetVisibility(ESlateVisibility::Visible);
		Button_SaveScene_AnotherName->SetVisibility(ESlateVisibility::Visible);
		Button_SaveScene->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Popup_PreView_->SetVisibility(ESlateVisibility::Visible);
		Message_Manager->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_LoadScene->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_NewScene->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_PreView->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_SaveScene_AnotherName->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_SaveScene->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

bool UScene_Manager::GetOpenFileDialog(FString DirectoryPath, TArray<FString>& Directory)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform) {
		DesktopPlatform->OpenFileDialog(
			NULL,
			TEXT("Select Message Scene"),	// 파일탐색기 제목
			DirectoryPath,	// 열고싶은 파일탐색기 경로
			FString(),	// 동작하지 않는 파라미터같음 (지정해도 파일 안열림)
			FString("Scene file|*.txt"),	// 열고싶은 파일 종류 [설명|확장자;확장자...] 형식
			EFileDialogFlags::None, // 파일 다중 선택 플래그
			Directory // 파일 목록 저장
		);
	}

	return Directory.Num() > 0 ? true : false;
}

bool UScene_Manager::GetSaveFileDialog(FString DirectoryPath, TArray<FString>& Directory)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform) {
		DesktopPlatform->SaveFileDialog(
			NULL,
			TEXT("Save Message Scene"),	// 파일탐색기 제목
			DirectoryPath,	// 열고싶은 파일탐색기 경로
			FString(),	// 동작하지 않는 파라미터같음 (지정해도 파일 안열림)
			FString("CSV file|*.csv"),	// 열고싶은 파일 종류 [설명|확장자;확장자...] 형식
			EFileDialogFlags::None, // 파일 다중 선택 플래그
			Directory // 파일 목록 저장
		);
	}

	return Directory.Num() > 0 ? true : false;
}
