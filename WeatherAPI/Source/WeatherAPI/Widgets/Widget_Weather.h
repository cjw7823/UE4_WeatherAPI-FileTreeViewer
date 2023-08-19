// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"
#include "ImgMediaSource.h"
#include "Http_weather.h"

#include "Engine/Font.h"
#include "Engine/FontFace.h"

#include "Widget_Weather.generated.h"


class UDataTable;

UCLASS()
class WEATHERAPI_API UWidget_Weather : public UUserWidget
{
	GENERATED_BODY()

		UWidget_Weather(const FObjectInitializer& objectinitializer);
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


	UFUNCTION()
		void SetClock();
	UFUNCTION()
		void SetTomorrowWeather(); // �ܱ⿹�� ���. 1�� 8ȸ API����

	/// <summary>
	/// �ʴܱ� ��ȲAPI �� SKY�ڵ尡 ���� ���� ������ ������ �Ұ����� �����,
	/// �ѽð� ���� �ʴܱ� ���� API�� ȣ���� �Ľ��ϴ� Ư���� ���� �Լ��� ȣ��.
	/// ���� �Լ��� UHttp_weather�� ������.
	/// </summary>
	UFUNCTION()
		void SetNowWeather(); // �ʴܱ��Ȳ ���. �Žð� API����
	UFUNCTION()
		void SetNearFutureWeather(); // �ʴܱ⿹�� ���. �Žð� API����
	void FirstSetTodayMessage();

	//Called BP  ���� �ִ� �̺�Ʈ�� ���ε� �� �Լ�.
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_BG();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_BG_close();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_now();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_now_close();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_near();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_near_close();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_low_high();
	UFUNCTION(BlueprintCallable, Category = "WeatherAPI")
		void OnEventAnim_low_high_close();

	UFUNCTION(BlueprintPure)
		FString GetUpdateTime() { return update_time; };

	void SetBackgroundVideo();
	void SetImgSequence_Now();
	void SetImgSequence_nearfuture_01();
	void SetImgSequence_nearfuture_02();
	void SetImgSequence_nearfuture_03();
	void SetImgSequence_lowest();
	void SetImgSequence_highest();

private:
	FString ChooseImgPath(hyundai_weather weather);

	//������ �޼��� ��Ʈ ũ�� ���� ����
	UFUNCTION()
		void SetCustomFont();

	/// <summary>
	/// APIȣ��� �Ű����� ����.
	/// </summary>
	/// <param name="base_date"> baseDate </param> 
	/// <param name="base_time"> baseTime </param> 
	/// <param name="calledAPI"> 1 : �ܱ⿹��, 2 : �ʴܱ��Ȳ, 3 : �ʴܱ⿹�� </param>
	void SetDateAndTime(FString& base_date, FString& base_time, uint8 calledAPI);
	void SetDateAndTimeOneminus(FString& base_date, FString& base_time);
	void SetNearFutureText(); // �ʴܱ⿹�� �����Ϳ� �ش��ϴ� text ����.
	UFUNCTION()
		void SetTodayMessage(); //Ÿ�̸ӿ� ���� �Լ�.
	UFUNCTION()
		void CheckTodayMessagePath(); //today_message_path�� ��ȭ�� üũ�� ���� ����.

	//PlayAnimation() ����.
	UFUNCTION()
		void PlayMainAnim(UWidgetAnimation* mainanim);

public:
	FTimerHandle clock_timer;
	FTimerHandle tomorrow_timer;
	FTimerHandle now_timer;
	FTimerHandle nearfuture_timer;
	FTimerHandle mutex_timer;
	int32 todaymessage_index;
	int32 maxindex;

private:
	//PlayAnimation() ������ ���� ��������Ʈ&�ڵ�.
	FTimerDelegate animplayDel;
	FTimerHandle animplayHandle;

	FString update_time;

	//���� ������ �񱳸� ���� �ӽ� �����
	hyundai_weather weather_bg;
	hyundai_weather weather_now;
	hyundai_weather weather_nearfuture_01;
	hyundai_weather weather_nearfuture_02;
	hyundai_weather weather_nearfuture_03;
	hyundai_weather weather_lowest;
	hyundai_weather weather_highest;

public:
	FCriticalSection mutex_today_message_timer;

	FString pre_today_message_path;
	UPROPERTY(BlueprintReadWrite)
		FString today_message_path;
	UPROPERTY(BlueprintReadWrite)
		FString background_video_path;
	UPROPERTY(BlueprintReadWrite)
		FString img_sequence_path;

private:
	UPROPERTY()
		UDataTable* DT;

	UFont* Font_bold;
	UFont* Font_regular;
	UFont* Font_semibold;
	UFont* Font_Noto_bold;
	UFont* Font_Noto_regular;

	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_Date;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_Time;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_lowest;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_highest;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_Now;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_Now_;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_nearfuture_01;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_nearfuture_02;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_nearfuture_03;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_maintext;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_subtext;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_nearfuture_tmp_01;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_nearfuture_tmp_02;
	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_nearfuture_tmp_03;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* MainAnim;

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_bg;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_bg_sec;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_now;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_now_sec;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_near;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_near_sec;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_low_high;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Dissolve_low_high_sec;

	//������ ȿ���� ��� ������ ���� ���� true = origin, false = second
	bool bImage_bg;
	bool bImage_now;
	bool bImage_near;
	bool bImage_low_high;

private:
	UPROPERTY(meta = (bindwidget))
		UImage* Image_Background;
	UPROPERTY()
		UMediaTexture* MediaTexture;
	UPROPERTY()
		UMediaPlayer* MediaPlayer;
	UPROPERTY()
		UFileMediaSource* FileMediaSource;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_Background_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_sec;
	UPROPERTY()
		UFileMediaSource* FileMediaSource_sec;

	// -------------------------�̹��� ������ ����� ���� ��� ����.------------------------------ //
	UPROPERTY(meta = (bindwidget))
		UImage* Image_now;
	UPROPERTY()
		UMediaTexture* MediaTexture_now;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_now;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_now;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_now_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_now_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_now_sec;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_now_sec;

	UPROPERTY(meta = (bindwidget))
		UImage* Image_nearfuture_01;
	UPROPERTY()
		UMediaTexture* MediaTexture_nearfuture_01;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_nearfuture_01;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_nearfuture_01;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_nearfuture_01_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_nearfuture_01_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_nearfuture_01_sec;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_nearfuture_01_sec;

	UPROPERTY(meta = (bindwidget))
		UImage* Image_nearfuture_02;
	UPROPERTY()
		UMediaTexture* MediaTexture_nearfuture_02;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_nearfuture_02;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_nearfuture_02;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_nearfuture_02_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_nearfuture_02_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_nearfuture_02_sec;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_nearfuture_02_sec;

	UPROPERTY(meta = (bindwidget))
		UImage* Image_nearfuture_03;
	UPROPERTY()
		UMediaTexture* MediaTexture_nearfuture_03;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_nearfuture_03;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_nearfuture_03;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_nearfuture_03_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_nearfuture_03_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_nearfuture_03_sec;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_nearfuture_03_sec;

	UPROPERTY(meta = (bindwidget))
		UImage* Image_lowest;
	UPROPERTY()
		UMediaTexture* MediaTexture_lowest;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_lowest;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_lowest;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_lowest_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_lowest_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_lowest_sec;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_lowest_sec;

	UPROPERTY(meta = (bindwidget))
		UImage* Image_highest;
	UPROPERTY()
		UMediaTexture* MediaTexture_highest;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_highest;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_highest;
	UPROPERTY(meta = (bindwidget))
		UImage* Image_highest_sec;
	UPROPERTY()
		UMediaTexture* MediaTexture_highest_sec;
	UPROPERTY()
		UMediaPlayer* MediaPlayer_highest_sec;
	UPROPERTY()
		UImgMediaSource* ImgMediaSource_highest_sec;

	// -------------------------�׽�Ʈ �뵵------------------------------ //
public:
	UPROPERTY(BlueprintReadWrite)
		hyundai_weather TEST_Weather;

	UFUNCTION(BlueprintCallable)
		void SetWeather();

	public:
		bool bFirstActivate;
};
