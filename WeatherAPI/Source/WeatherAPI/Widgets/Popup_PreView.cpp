// Fill out your copyright notice in the Description page of Project Settings.


#include "Popup_PreView.h"
#include "CustomDataTable.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Scene_Manager.h"

void UPopup_PreView::NativeConstruct()
{
	Button_Close->OnClicked.AddDynamic(this, &UPopup_PreView::OnClicked_Button_Close);

	//Canvas Pannel�� �پ��ִٸ� parent == Canvas Pannel
	UPanelWidget* parent = GetParent();

	//Canvas Pannel�� outer -> WidgetTree
	UObject* outer = parent->GetOuter();
	UWidgetTree* widget_tree = Cast<UWidgetTree>(outer);

	//WidgetTree�� outer -> UserWidget
	UObject* parent_widget = widget_tree->GetOuter();
	UUserWidget* user_widget = Cast<UUserWidget>(parent_widget);

	Scene_Manager = Cast<UScene_Manager>(user_widget);
}

void UPopup_PreView::FirstSetTodayMessage(FString csv_string)
{
	todaymessage_index = 0;
	maxindex = 0;

	DT = NewObject<UDataTable>(GetTransientPackage(), FName(TEXT("CSV_DT")));
	DT->RowStruct = FCustomDataTable::StaticStruct();

	auto error = DT->CreateTableFromCSVString(csv_string);// '����Ʈ' �� ���� �׸� ����

	animplayDel.Unbind();
	animplayDel.BindUObject(this, &UPopup_PreView::PlayMainAnim, MainAnim);

	SetTodayMessage();//���ο��� Ÿ�̸� ����
}

void UPopup_PreView::OnEventAnim()
{
	SetTodayMessage();
}

void UPopup_PreView::SetTodayMessage()
{
	todaymessage_index++;

	FName index = FName(FString::FromInt(todaymessage_index));
	FString debuging;

	//csv���Ͽ� index�� ���ٸ�. �Լ� ��� ����.
	if (DT->FindRow<FCustomDataTable>("1", debuging) == nullptr)
	{
		TextBlock_maintext->SetText(FText::FromString(""));
		TextBlock_subtext->SetText(FText::FromString(""));
		return;
	}

	//csv���Ͽ� index == 1, ���� �ִ��� ������� ����.
	if (maxindex != 1 && DT->FindRow<FCustomDataTable>("2", debuging) == nullptr)
		maxindex = 1;

	FCustomDataTable* today_message = DT->FindRow<FCustomDataTable>(index, debuging);

	if (today_message == nullptr) // ������ ���̺��� ���� �����ϸ� �ε����� �ʱ�ȭ��.
	{
		todaymessage_index = 0;
		SetTodayMessage();
		return;
	}

	//�ؽ�Ʈ ����
	FString temp1 = today_message->MainText.Replace(L"\\n", L"\n");
	FString temp2 = today_message->SubText;
	TextBlock_maintext->SetText(FText::FromString(temp1));
	TextBlock_subtext->SetText(FText::FromString(temp2));

	//��ġ ����
	UCanvasPanelSlot* imageSlot_m = Cast<UCanvasPanelSlot>(TextBlock_maintext->Slot);
	UCanvasPanelSlot* imageSlot_s = Cast<UCanvasPanelSlot>(TextBlock_subtext->Slot);

	int32 viewYsize = 268;
	
	if (temp2 == FString())
	{
		imageSlot_m->SetPosition(FVector2D(0, viewYsize/2));
	}
	else
	{
		Super::ForceLayoutPrepass();

		int32 between_pixel = 10;

		int32 mainTextSize_Y_half = TextBlock_maintext->GetDesiredSize().Y / 2;
		int32 subTextSize_Y_half = TextBlock_subtext->GetDesiredSize().Y / 2;
		int32 mainTextPos = (viewYsize / 2) - subTextSize_Y_half - (between_pixel / 2);
		int32 subTextPos = (viewYsize / 2) + mainTextSize_Y_half + (between_pixel / 2);

		imageSlot_m->SetPosition(FVector2D(0, mainTextPos));
		imageSlot_s->SetPosition(FVector2D(0, subTextPos));
	}

	//Ÿ�̸� ����. �̸����� �̹Ƿ� �ð��� 1.5�ʷ� ����.
	animplayHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(animplayHandle, animplayDel, 1.5f, false);
}

void UPopup_PreView::PlayMainAnim(UWidgetAnimation* mainanim)
{
	if (maxindex == 1)
		return;
	PlayAnimation(mainanim);
}

void UPopup_PreView::OnClicked_Button_Close()
{
	Scene_Manager->PopupActivate_PreView();
	GetWorld()->GetTimerManager().ClearTimer(animplayHandle);
	SetVisibility(ESlateVisibility::Collapsed);
}
