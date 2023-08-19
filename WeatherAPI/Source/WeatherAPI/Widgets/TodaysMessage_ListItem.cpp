// Fill out your copyright notice in the Description page of Project Settings.


#include "TodaysMessage_ListItem.h"
#include "Message_Manager.h"
#include "ListItem_Data.h"

void UTodaysMessage_ListItem::NativeConstruct()
{
	Button_Delete->OnClicked.AddDynamic(this, &UTodaysMessage_ListItem::OnClicked_Button_Delete);
}

void UTodaysMessage_ListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UListItem_Data* list_item = Cast<UListItem_Data>(ListItemObject);
	parent = list_item->parent;
	data = list_item->data;

	FString temp1 = list_item->Main.ToString().Replace(L"\n", L" ").Left(55);
	FString temp2 = list_item->Sub.ToString().Replace(L"\n", L" ").Left(10);

	MainText->SetText(FText::FromString(temp1));
	SubText->SetText(FText::FromString(temp2));

	TextBlock_Index->SetText(FText::FromString(TEXT("텍스트 ") + FString::FromInt(parent->ListView_0->GetIndexForItem(data) + 1)));

	if (IsListItemSelected())
	{
		Image_BG->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_BG->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTodaysMessage_ListItem::SetTextData(FText main, FText sub)
{
	data->Main = main;
	data->Sub = sub;

	FString temp1 = data->Main.ToString().Replace(L"\r\n", L" ").Left(55);
	FString temp2 = data->Sub.ToString().Replace(L"\r\n", L" ").Left(10);

	temp1 = temp1.Replace(L"\n", L" ").Left(55);
	temp2 = temp2.Replace(L"\n", L" ").Left(10);

	MainText->SetText(FText::FromString(temp1));
	SubText->SetText(FText::FromString(temp2));
}

void UTodaysMessage_ListItem::SetTextIndex()
{
	TextBlock_Index->SetText(FText::FromString(TEXT("텍스트 ") + FString::FromInt(parent->ListView_0->GetIndexForItem(data) + 1)));
}

void UTodaysMessage_ListItem::OnClicked_Button_Delete()
{
	//편집중인 item이 삭제 된다면.
	if (data == parent->selected_item)
		parent->ActivateTextEditer(ESlateVisibility::Collapsed);

	UListView* listview = parent->ListView_0;
	listview->RemoveItem(data);

	TArray<UObject*> items = listview->GetListItems();
	for (auto item : items)
	{
		UTodaysMessage_ListItem* temp = Cast<UTodaysMessage_ListItem>(listview->GetEntryWidgetFromItem(item));
		
		if(temp != nullptr)
			temp->SetTextIndex();
	}
}