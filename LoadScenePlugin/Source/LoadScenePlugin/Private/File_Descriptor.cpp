// Fill out your copyright notice in the Description page of Project Settings.


#include "File_Descriptor.h"

#include "LoadScene.h"

UFile_Descriptor::UFile_Descriptor(const FObjectInitializer& obj) : Super(obj), pLoadScene(nullptr)
{
	path = FString();
}

void UFile_Descriptor::NativeConstruct()
{
	BG_Click->OnMouseButtonDownEvent.BindUFunction(this, FName("OnMouseButtonDownEvent_BG_Click"));
}

FEventReply UFile_Descriptor::OnMouseButtonDownEvent_BG_Click(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TextBlock_Header->GetText().ToString());
	
	if (pLoadScene != nullptr)
	{
		pLoadScene->LoadSceneName->SetText(TextBlock_Header->GetText());
		pLoadScene->loaded_file_path = path;
	}

	return FEventReply();
}
