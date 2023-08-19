// Fill out your copyright notice in the Description page of Project Settings.

#include "Message_Manager.h"
#include "ListItem_Data.h"
#include "TodaysMessage_ListItem.h"

UMessage_Manager::UMessage_Manager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMessage_Manager::NativeConstruct()
{
	Button_Input->OnClicked.AddDynamic(this, &UMessage_Manager::OnClicked_Button_Input);
	Button_AddText->OnClicked.AddDynamic(this, &UMessage_Manager::OnClicked_Button_AddText);
	ListView_0->OnItemClicked().AddUObject(this, &UMessage_Manager::OnClicked_ListItem);

	ActivateTextEditer(ESlateVisibility::Collapsed);
}

void UMessage_Manager::ActivateTextEditer(ESlateVisibility visible)
{
	MainText->SetVisibility(visible);
	SubText->SetVisibility(visible);
	Button_Input->SetVisibility(visible);

	/* 절차상 필요 없음.
	MainText->SetText(FText());
	SubText->SetText(FText());
	*/

	if(visible == ESlateVisibility::Collapsed)
		selected_item = nullptr;
}

void UMessage_Manager::OnClicked_ListItem(UObject* obj)
{
	ActivateTextEditer(ESlateVisibility::Visible);

	UListItem_Data* data = Cast<UListItem_Data>(obj);
	selected_item = data;
	
	MainText->SetText(data->Main);
	SubText->SetText(data->Sub);

	auto items = ListView_0->GetListItems();
	for (auto item : items)
	{
		auto temp = Cast<UTodaysMessage_ListItem>(ListView_0->GetEntryWidgetFromItem(item));

		if(temp != nullptr)
			temp->Image_BG->SetVisibility(ESlateVisibility::Collapsed);
	}
	Cast<UTodaysMessage_ListItem>(ListView_0->GetEntryWidgetFromItem(obj))->Image_BG->SetVisibility(ESlateVisibility::Visible);
	ListView_0->SetItemSelection(obj, true);

}

void UMessage_Manager::OnClicked_Button_Input()
{
	if (selected_item == nullptr)
		return;

	Cast<UTodaysMessage_ListItem>(ListView_0->GetEntryWidgetFromItem(selected_item))->SetTextData(MainText->GetText(), SubText->GetText());
}

void UMessage_Manager::OnClicked_Button_AddText()
{
	UListItem_Data* item = NewObject<UListItem_Data>();
	item->parent = this;
	item->data = item;
	item->Main = FText();
	item->Sub = FText();
	ListView_0->AddItem(item);
}