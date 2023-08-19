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
		void SetTomorrowWeather(); // 단기예보 사용. 1일 8회 API갱신

	/// <summary>
	/// 초단기 실황API 에 SKY코드가 없어 날씨 아이콘 선별이 불가능한 관계로,
	/// 한시간 전의 초단기 예보 API를 호출해 파싱하는 특수한 내부 함수를 호출.
	/// 내부 함수는 UHttp_weather에 구현됨.
	/// </summary>
	UFUNCTION()
		void SetNowWeather(); // 초단기실황 사용. 매시간 API갱신
	UFUNCTION()
		void SetNearFutureWeather(); // 초단기예보 사용. 매시간 API갱신
	void FirstSetTodayMessage();

	//Called BP  위젯 애님 이벤트에 바인딩 된 함수.
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

	//오늘의 메세지 폰트 크기 동적 설정
	UFUNCTION()
		void SetCustomFont();

	/// <summary>
	/// API호출시 매개변수 세팅.
	/// </summary>
	/// <param name="base_date"> baseDate </param> 
	/// <param name="base_time"> baseTime </param> 
	/// <param name="calledAPI"> 1 : 단기예보, 2 : 초단기실황, 3 : 초단기예보 </param>
	void SetDateAndTime(FString& base_date, FString& base_time, uint8 calledAPI);
	void SetDateAndTimeOneminus(FString& base_date, FString& base_time);
	void SetNearFutureText(); // 초단기예보 데이터에 해당하는 text 세팅.
	UFUNCTION()
		void SetTodayMessage(); //타이머용 내부 함수.
	UFUNCTION()
		void CheckTodayMessagePath(); //today_message_path의 변화를 체크해 새로 적용.

	//PlayAnimation() 래핑.
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
	//PlayAnimation() 래핑을 위한 델리게이트&핸들.
	FTimerDelegate animplayDel;
	FTimerHandle animplayHandle;

	FString update_time;

	//이전 날씨와 비교를 위한 임시 저장소
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

	//디졸브 효과의 토글 형식을 위한 변수 true = origin, false = second
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

	// -------------------------이미지 시퀀스 재생을 위한 멤버 변수.------------------------------ //
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

	// -------------------------테스트 용도------------------------------ //
public:
	UPROPERTY(BlueprintReadWrite)
		hyundai_weather TEST_Weather;

	UFUNCTION(BlueprintCallable)
		void SetWeather();

	public:
		bool bFirstActivate;
};
