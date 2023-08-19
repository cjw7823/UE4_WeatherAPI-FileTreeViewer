// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup_NewScene.h"
#include "Blueprint/WidgetTree.h"
#include "Scene_Manager.h"
#include "Message_Manager.h"

void UPopup_NewScene::NativeConstruct()
{
	Button_Save->OnClicked.AddDynamic(this, &UPopup_NewScene::OnClicked_Button_Save);
	Button_Reset->OnClicked.AddDynamic(this, &UPopup_NewScene::OnClicked_Button_Reset);

	//Canvas Pannel에 붙어있다면 parent == Canvas Pannel
	UPanelWidget* parent = GetParent();

	//Canvas Pannel의 outer -> WidgetTree
	UObject* outer = parent->GetOuter();
	UWidgetTree* widget_tree = Cast<UWidgetTree>(outer);

	//WidgetTree의 outer -> UserWidget
	UObject* parent_widget = widget_tree->GetOuter();
	UUserWidget* user_widget = Cast<UUserWidget>(parent_widget);

	Scene_Manager = Cast<UScene_Manager>(user_widget);
}

void UPopup_NewScene::OnClicked_Button_Save()
{
	Scene_Manager->OnClicked_Button_SaveScene();

	OnClicked_Button_Reset();
}

void UPopup_NewScene::OnClicked_Button_Reset()
{
	Scene_Manager->TextBlock_SceneTitle->SetText(FText::FromString(TEXT("신규 씬 구성 진행 중")));
	Scene_Manager->is_loaded_scene = false;
	Scene_Manager->loaded_CSV_path = FString();
	Scene_Manager->Message_Manager->ListView_0->ClearListItems();
	Scene_Manager->Message_Manager->ActivateTextEditer(ESlateVisibility::Collapsed);

	Scene_Manager->PopupActivate_NewScene();
}
