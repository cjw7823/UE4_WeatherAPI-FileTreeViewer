// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup_Save.h"
#include "Scene_Manager.h"
#include "Components/ListView.h"
#include "ListItem_Data.h"
#include "Message_Manager.h"

#define NEXTMESSAGE_TIME_MIN 1

FString JsonStr;
TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&JsonStr);

void UPopup_Save::SaveMessageScene(FString save_path, FString contents)
{
	JsonStr = FString();

	Writer = TJsonWriterFactory<>::Create(&JsonStr);
	Writer->WriteObjectStart();
	Writer->WriteValue(FString(TEXT("page")), 3);
	Writer->WriteValue(FString(TEXT("path")), contents);

	FString scene_name_without_path = FPaths::GetCleanFilename(contents);
	//'.csv' 제거
	scene_name_without_path =  scene_name_without_path.Left(scene_name_without_path.Len() - 4);
	//'[csv]' 제거
	scene_name_without_path = scene_name_without_path.Right(scene_name_without_path.Len() - 5);

	Writer->WriteValue(FString(TEXT("SceneName")), scene_name_without_path);
	Writer->WriteObjectEnd();
	Writer->Close();

	FFileHelper::SaveStringToFile(*JsonStr, *save_path);
}

void UPopup_Save::SaveMessageCSV(FString save_path, UScene_Manager* parent)
{
	FString message_scene = FString("Index,MainText,SubText,Time_min\n");

	UListView* listview = parent->Message_Manager->ListView_0;

	for (int i = 0; ; i++)
	{
		UListItem_Data* data = Cast<UListItem_Data>(listview->GetItemAt(i));
		if (data == nullptr)
			break;

		FString temp1 = data->Main.ToString().Replace(L"\n", L"\\n");
		FString temp2 = data->Sub.ToString().Replace(L"\n", L"\\n");

		message_scene += FString::FromInt(i + 1) + ",\"" + temp1 + "\",\"" + temp2 + "\"," + FString::FromInt(NEXTMESSAGE_TIME_MIN) + "\n";
	}
	FFileHelper::SaveStringToFile(message_scene, *save_path);

	//씬 제목 변경. 씬 로드 상태로 변경.
	FString scene_title_without_path = FPaths::GetCleanFilename(save_path);
	parent->TextBlock_SceneTitle->SetText(FText::FromString(scene_title_without_path));
	parent->is_loaded_scene = true;
	parent->loaded_CSV_path = save_path;
}
