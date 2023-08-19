// Fill out your copyright notice in the Description page of Project Settings.

#include "Http_weather.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Widgets/Widget_Weather.h"

#define MID_URL "http://apis.data.go.kr/1360000/MidFcstInfoService/getMidFcst"
#define ULTRA_SRT_NCST_URL "http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getUltraSrtNcst"
#define ULTRA_SRT_FCST_URL "http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getUltraSrtFcst"
#define VILAGE_URL "http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst"
#define ENCODING_KEY "5M%2BZsPId%2F6xhqKg56OeNuumzBSYrPzmwkjaaDigEu%2BCjShkBalYk0d8Abnay1jjEgrA0lAx%2BUlEwDnxa6X0F1g%3D%3D"
#define NX FString("62")
#define NY FString("122")
#define MID_ID FString("109") // 서울, 인천, 경기도 지역코드

DEFINE_LOG_CATEGORY(weather_api);

UHttp_weather* UHttp_weather::Instance = nullptr;

UHttp_weather::UHttp_weather()
{
	Http = &FHttpModule::Get();

	lowest_tmp = FString();
	highest_tmp = FString();
	now_tmp = FString();
	nearfuture_tmp_01 = FString();
	nearfuture_tmp_02 = FString();
	nearfuture_tmp_03 = FString();

	now_weather = hyundai_weather::none;
	nearfuture_weather_01 = hyundai_weather::none;
	nearfuture_weather_02 = hyundai_weather::none;
	nearfuture_weather_03 = hyundai_weather::none;
	lowest_weather = hyundai_weather::none;
	highest_weather = hyundai_weather::none;

	Widget_Weather = nullptr;
}

void UHttp_weather::MidFutureCast(FString base_date, FString base_time)
{
	if(!Instance)
	{
		Instance = NewObject<UHttp_weather>();
		if (!Instance)
		{
			UE_LOG(weather_api, Error, TEXT("Could not create an instance for UHttp_weather"));
			return;
		}

		Instance->AddToRoot();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Instance->Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(Instance, &UHttp_weather::OnResponseReceived_MidFutureCast);

	FString URL = MID_URL;
	URL += "?ServiceKey=" + FString(ENCODING_KEY);
	URL += "&pageNo=1";
	URL += "&numOfRows=10";
	URL += "&dataType=JSON";
	URL += "&stnId=" + MID_ID;
	URL += "&tmFc=" + base_date + base_time;

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHttp_weather::VilageFutureCast(FString base_date, FString base_time)
{
	if (!Instance)
	{
		Instance = NewObject<UHttp_weather>();
		if (!Instance)
		{
			UE_LOG(weather_api, Error, TEXT("Could not create an instance for UHttp_weather"));
			return;
		}

		Instance->AddToRoot();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Instance->Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(Instance, &UHttp_weather::OnResponseReceived_VilageFutureCast);

	FString URL = VILAGE_URL;
	URL += "?ServiceKey=" + FString(ENCODING_KEY);
	URL += "&pageNo=1";
	URL += "&numOfRows=1000";
	URL += "&dataType=JSON";
	URL += "&base_date=" + base_date;
	URL += "&base_time=" + base_time;
	URL += "&nx=" + NX;
	URL += "&ny=" + NY;

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHttp_weather::UltraSrtFutureCast(FString base_date, FString base_time)
{
	if (!Instance)
	{
		Instance = NewObject<UHttp_weather>();
		if (!Instance)
		{
			UE_LOG(weather_api, Error, TEXT("Could not create an instance for UHttp_weather"));
			return;
		}

		Instance->AddToRoot();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Instance->Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(Instance, &UHttp_weather::OnResponseReceived_UltraSrtFutureCast);

	FString URL = ULTRA_SRT_FCST_URL;
	URL += "?ServiceKey=" + FString(ENCODING_KEY);
	URL += "&pageNo=1";
	URL += "&numOfRows=1000";
	URL += "&dataType=JSON";
	URL += "&base_date=" + base_date;
	URL += "&base_time=" + base_time;
	URL += "&nx=" + NX;
	URL += "&ny=" + NY;

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHttp_weather::UltraSrtNowCast(FString base_date, FString base_time)
{
	if (!Instance)
	{
		Instance = NewObject<UHttp_weather>();
		if (!Instance)
		{
			UE_LOG(weather_api, Error, TEXT("Could not create an instance for UHttp_weather"));
			return;
		}

		Instance->AddToRoot();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Instance->Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(Instance, &UHttp_weather::OnResponseReceived_UltraSrtNowCast);

	FString URL = ULTRA_SRT_NCST_URL;
	URL += "?ServiceKey=" + FString(ENCODING_KEY);
	URL += "&pageNo=1";
	URL += "&numOfRows=100";
	URL += "&dataType=JSON";
	URL += "&base_date=" + base_date;
	URL += "&base_time=" + base_time;
	URL += "&nx=" + NX;
	URL += "&ny=" + NY;

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHttp_weather::UltraSrtFutureOneminus(FString base_date, FString base_time)
{
	if (!Instance)
	{
		Instance = NewObject<UHttp_weather>();
		if (!Instance)
		{
			UE_LOG(weather_api, Error, TEXT("Could not create an instance for UHttp_weather"));
			return;
		}

		Instance->AddToRoot();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Instance->Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(Instance, &UHttp_weather::OnResponseReceived_UltraSrtFutureOneminus);

	FString URL = ULTRA_SRT_FCST_URL;
	URL += "?ServiceKey=" + FString(ENCODING_KEY);
	URL += "&pageNo=1";
	URL += "&numOfRows=1000";
	URL += "&dataType=JSON";
	URL += "&base_date=" + base_date;
	URL += "&base_time=" + base_time;
	URL += "&nx=" + NX;
	URL += "&ny=" + NY;

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHttp_weather::OnResponseReceived_MidFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(weather_api, Log, TEXT("%s"), *Response->GetContentAsString());
}

void UHttp_weather::OnResponseReceived_VilageFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "OnResponseReceived_VilageFutureCast");
	//UE_LOG(weather_api, Log, TEXT("%s"), *Response->GetContentAsString());

	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(*Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	TSharedPtr<FJsonObject> JsonObj = JsonObject->GetObjectField(TEXT("response"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("body"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("items"));
	if (!JsonObj.IsValid())
		return;

	TArray<TSharedPtr<FJsonValue>> Array = JsonObj->GetArrayField(TEXT("item"));

	bool find_TMN = false;
	bool find_TMX = false;
	bool find_SKY = false;
	bool find_PTY = false;
	bool find_WSD = false;

	weather_bitfield weather_lowest;
	weather_bitfield weather_highest;

	//파싱
	for (int i = 0; i < Array.Num(); i++)
	{
		JsonObj = (*(Array[i])).AsObject();
		FString category = JsonObj->GetStringField(TEXT("category"));
		
		FString fcstDate = JsonObj->GetStringField(TEXT("fcstDate"));
		FString baseDate = JsonObj->GetStringField(TEXT("baseDate"));

		int32 fcst = UKismetStringLibrary::Conv_StringToInt(fcstDate);
		int32 base = UKismetStringLibrary::Conv_StringToInt(baseDate);

		if (fcst == base)//오늘 날짜면 넘어감.
			continue;

		//최고 최저 기온은 코드로 제공 되기 때문에 category로 찾는게 가장 빠름.
		if (!find_TMN && category == FString("TMN"))//일 최저 기온
		{
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "TMN : " + fcstValue);
			lowest_tmp = fcstValue;
			find_TMN = true;
		}
		else if (!find_TMX && category == FString("TMX"))//일 최고 기온
		{
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "TMX : " + fcstValue);
			highest_tmp = fcstValue;
			find_TMX = true;
		}
		else if (!find_SKY && category == FString("SKY"))//하늘 상태, 맑음(1), 구름많음(3), 흐림(4)
		{
			FString fcstTime = JsonObj->GetStringField(TEXT("fcstTime"));
			int32 fcst_t = UKismetStringLibrary::Conv_StringToInt(fcstTime);

			if (fcst_t >= 300 && fcst_t <= 900) //3~9시 일 경우
			{
				FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
				if (fcstValue == FString("1"))
					weather_lowest.sunny = 1;
				else if (fcstValue == FString("3"))
					weather_lowest.mostly_cloudy = 1;
				else if (fcstValue == FString("4"))
					weather_lowest.cloudy = 1;
			}
			else if (fcst_t >= 900 && fcst_t <= 1700) //9~17시 일 경우
			{
				FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
				if (fcstValue == FString("1"))
					weather_highest.sunny = 1;
				else if (fcstValue == FString("3"))
					weather_highest.mostly_cloudy = 1;
				else if (fcstValue == FString("4"))
					weather_highest.cloudy = 1;
			}
			else if (fcst_t > 1700) // 한번이라도 17시가 지났다는건 '내일'의 데이터 파싱이 끝났다는 뜻. flag on.
			{
				find_SKY = true;
				continue;
			}
		}
		else if (!find_PTY && category == FString("PTY"))//강수 형태, (단기) 없음(0), 비(1), 비/눈(2), 눈(3), 소나기(4)
		{
			FString fcstTime = JsonObj->GetStringField(TEXT("fcstTime"));
			int32 fcst_t = UKismetStringLibrary::Conv_StringToInt(fcstTime);

			if (fcst_t >= 300 && fcst_t <= 900) //3~9시 일 경우
			{ 
				FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
				if (fcstValue == FString("1"))
					weather_lowest.rainy = 1;
				else if (fcstValue == FString("2"))
					weather_lowest.snow_rainy = 1;
				else if (fcstValue == FString("3"))
					weather_lowest.snow = 1;
				else if (fcstValue == FString("4"))
					weather_lowest.rainy = 1;
			}
			else if (fcst_t >= 900 && fcst_t <= 1700) //9~17시 일 경우
			{
				FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
				if (fcstValue == FString("1"))
					weather_highest.rainy = 1;
				else if (fcstValue == FString("2"))
					weather_highest.snow_rainy = 1;
				else if (fcstValue == FString("3"))
					weather_highest.snow = 1;
				else if (fcstValue == FString("4"))
					weather_highest.rainy = 1;
			}
			else if (fcst_t > 1700) // 한번이라도 17시가 지났다는건 '내일'의 데이터 파싱이 끝났다는 뜻. flag on.
			{
				find_PTY = true;
				continue;
			}
		}
		else if (!find_WSD && category == FString("WSD"))//풍속
		{
			FString fcstTime = JsonObj->GetStringField(TEXT("fcstTime"));
			int32 fcst_t = UKismetStringLibrary::Conv_StringToInt(fcstTime);

			if (fcst_t >= 300 && fcst_t <= 900) //3~9시 일 경우
			{
				FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
				float fcstValue_float = UKismetStringLibrary::Conv_StringToFloat(fcstValue);
				if (fcstValue_float >= 14.0f)
					weather_lowest.windy = 1;
			}
			else if (fcst_t >= 900 && fcst_t <= 1700) //9~17시 일 경우
			{
				FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
				float fcstValue_float = UKismetStringLibrary::Conv_StringToFloat(fcstValue);
				if (fcstValue_float >= 14.0f)
					weather_highest.windy = 1;
			}
			else if (fcst_t > 1700) // 한번이라도 17시가 지났다는건 '내일'의 데이터 파싱이 끝났다는 뜻. flag on.
			{
				find_WSD = true;
				continue;
			}
		}
	}

	//함수 호출
	lowest_weather = BitfieldToStruct(weather_lowest);
	highest_weather = BitfieldToStruct(weather_highest);

	if (Instance->Widget_Weather != nullptr)
	{
		UWidget_Weather* temp = Cast<UWidget_Weather>(Instance->Widget_Weather);
		if (!(temp->bImage_low_high))
			temp->PlayAnimation(temp->Dissolve_low_high);
		else
			temp->PlayAnimation(temp->Dissolve_low_high_sec);
	}
}

void UHttp_weather::OnResponseReceived_UltraSrtFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//UE_LOG(weather_api, Warning, TEXT("%s"), *Response->GetContentAsString());

	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(*Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	TSharedPtr<FJsonObject> JsonObj = JsonObject->GetObjectField(TEXT("response"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("body"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("items"));
	if (!JsonObj.IsValid())
		return;

	TArray<TSharedPtr<FJsonValue>> Array = JsonObj->GetArrayField(TEXT("item"));

	weather_bitfield weather_01;
	weather_bitfield weather_02;
	weather_bitfield weather_03;

	uint8 flag1 = 0; // 연속적인 세개의 데이터를 파싱하면 해당 코드 무시. flag == 3.
	uint8 flag2 = 0;
	uint8 flag3 = 0;
	uint8 flag4 = 0;
	bool flag30_1 = false;
	bool flag30_2 = false;
	bool flag30_3 = false;
	bool flag30_4 = false;

	//파싱
	for (int i = 0; i < Array.Num(); i++)
	{
		JsonObj = (*(Array[i])).AsObject();
		FString category = JsonObj->GetStringField(TEXT("category"));

		if (flag1 < 3 && category == FString("T1H"))// 기온 코드 찾기
		{
			//30분 이전이면 전 시간대의 테이블을 가져왔을 테니 한번 무시하고 다음부터 파싱해야 현재+1시의 데이터를 얻음.
			if (!flag30_1 && UKismetMathLibrary::Now().GetMinute() < 30)
			{
				flag30_1 = true;
				continue;
			}
			flag1++;
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue")); // 처음 3번이 연속적인 시간의 데이터
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Near Future T1H : " + fcstValue);

			switch (flag1)
			{
			case 1:
				nearfuture_tmp_01 = fcstValue;
				break;
			case 2:
				nearfuture_tmp_02 = fcstValue;
				break;
			case 3:
				nearfuture_tmp_03 = fcstValue;
				break;
			default:
				break;
			}
		}
		else if (flag2 < 3 && category == FString("SKY"))//하늘 상태, 맑음(1), 구름많음(3), 흐림(4)
		{
			if (!flag30_2 && UKismetMathLibrary::Now().GetMinute() < 30)
			{
				flag30_2 = true;
				continue;
			}
			flag2++;
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue")); // 처음 3번이 연속적인 시간의 데이터

			switch (flag2)
			{
			case 1:
				if (fcstValue == FString("1"))
					weather_01.sunny = 1;
				else if (fcstValue == FString("3"))
					weather_01.mostly_cloudy = 1;
				else if (fcstValue == FString("4"))
					weather_01.cloudy = 1;
				break;
			case 2:
				if (fcstValue == FString("1"))
					weather_02.sunny = 1;
				else if (fcstValue == FString("3"))
					weather_02.mostly_cloudy = 1;
				else if (fcstValue == FString("4"))
					weather_02.cloudy = 1;
				break;
			case 3:
				if (fcstValue == FString("1"))
					weather_03.sunny = 1;
				else if (fcstValue == FString("3"))
					weather_03.mostly_cloudy = 1;
				else if (fcstValue == FString("4"))
					weather_03.cloudy = 1;
				break;
			default:
				break;
			}
		}
		else if (flag3 < 3 && category == FString("PTY"))//강수 형태, (초단기) 없음(0), 비(1), 비/눈(2), 눈(3), 빗방울(5), 빗방울눈날림(6), 눈날림(7)
		{
			if (!flag30_3 && UKismetMathLibrary::Now().GetMinute() < 30)
			{
				flag30_3 = true;
				continue;
			}
			flag3++;
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue")); // 처음 3번이 연속적인 시간의 데이터

			switch (flag3)
			{
			case 1:
				if (fcstValue == FString("1"))
					weather_01.rainy = 1;
				else if (fcstValue == FString("2"))
					weather_01.snow_rainy = 1;
				else if (fcstValue == FString("3"))
					weather_01.snow = 1;
				else if (fcstValue == FString("5"))
					weather_01.rainy = 1;
				else if (fcstValue == FString("6"))
					weather_01.snow_rainy = 1;
				else if (fcstValue == FString("7"))
					weather_01.snow = 1;
				break;
			case 2:
				if (fcstValue == FString("1"))
					weather_02.rainy = 1;
				else if (fcstValue == FString("2"))
					weather_02.snow_rainy = 1;
				else if (fcstValue == FString("3"))
					weather_02.snow = 1;
				else if (fcstValue == FString("5"))
					weather_02.rainy = 1;
				else if (fcstValue == FString("6"))
					weather_02.snow_rainy = 1;
				else if (fcstValue == FString("7"))
					weather_02.snow = 1;
				break;
			case 3:
				if (fcstValue == FString("1"))
					weather_03.rainy = 1;
				else if (fcstValue == FString("2"))
					weather_03.snow_rainy = 1;
				else if (fcstValue == FString("3"))
					weather_03.snow = 1;
				else if (fcstValue == FString("5"))
					weather_03.rainy = 1;
				else if (fcstValue == FString("6"))
					weather_03.snow_rainy = 1;
				else if (fcstValue == FString("7"))
					weather_03.snow = 1;
				break;
			default:
				break;
			}
		}
		else if (flag4 < 3 && category == FString("WSD"))//풍속
		{
			if (!flag30_4 && UKismetMathLibrary::Now().GetMinute() < 30)
			{
				flag30_4 = true;
				continue;
			}
			flag4++;
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue")); // 처음 3번이 연속적인 시간의 데이터
			float fcstValue_float = UKismetStringLibrary::Conv_StringToFloat(fcstValue);

			switch (flag4)
			{
			case 1:
				if (fcstValue_float >= 14.0f)
					weather_01.windy = 1;
				break;
			case 2:
				if (fcstValue_float >= 14.0f)
					weather_02.windy = 1;
				break;
			case 3:
				if (fcstValue_float >= 14.0f)
					weather_03.windy = 1;
				break;
			default:
				break;
			}
		}
	}

	nearfuture_weather_01 = BitfieldToStruct(weather_01, 1);
	nearfuture_weather_02 = BitfieldToStruct(weather_02, 2);
	nearfuture_weather_03 = BitfieldToStruct(weather_03, 3);

	if (Instance->Widget_Weather != nullptr)
	{
		UWidget_Weather* temp = Cast<UWidget_Weather>(Instance->Widget_Weather);
		if (!(temp->bImage_near))
			temp->PlayAnimation(temp->Dissolve_near);
		else
			temp->PlayAnimation(temp->Dissolve_near_sec);
	}
}

void UHttp_weather::OnResponseReceived_UltraSrtNowCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//UE_LOG(weather_api, Log, TEXT("%s"), *Response->GetContentAsString());

	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(*Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	TSharedPtr<FJsonObject> JsonObj = JsonObject->GetObjectField(TEXT("response"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("body"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("items"));
	if (!JsonObj.IsValid())
		return;

	TArray<TSharedPtr<FJsonValue>> Array = JsonObj->GetArrayField(TEXT("item"));

	for (int i = 0; i < Array.Num(); i++)
	{

		JsonObj = (*(Array[i])).AsObject();
		FString category = JsonObj->GetStringField(TEXT("category"));

		if (category == FString("T1H")) // 초단기 실황은 조회한 날짜와 시간만 리스폰 되기 때문에 코드 조회 후 바로 사용.
		{
			FString obsrValue = JsonObj->GetStringField(TEXT("obsrValue"));
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Now T1H : " + obsrValue);
			now_tmp = obsrValue;
			break;
		}
	}
}

void UHttp_weather::OnResponseReceived_UltraSrtFutureOneminus(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(*Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	TSharedPtr<FJsonObject> JsonObj = JsonObject->GetObjectField(TEXT("response"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("body"));
	if (!JsonObj.IsValid())
		return;

	JsonObj = JsonObj->GetObjectField(TEXT("items"));
	if (!JsonObj.IsValid())
		return;

	TArray<TSharedPtr<FJsonValue>> Array = JsonObj->GetArrayField(TEXT("item"));

	weather_bitfield weather_01;

	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;

	//파싱
	for (int i = 0; i < Array.Num(); i++)
	{
		JsonObj = (*(Array[i])).AsObject();
		FString category = JsonObj->GetStringField(TEXT("category"));

		if (!flag1 && category == FString("SKY"))//하늘 상태, 맑음(1), 구름많음(3), 흐림(4)
		{
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));

			if (fcstValue == FString("1"))
				weather_01.sunny = 1;
			else if (fcstValue == FString("3"))
				weather_01.mostly_cloudy = 1;
			else if (fcstValue == FString("4"))
				weather_01.cloudy = 1;

			flag1 = true;
		}
		else if (!flag2 && category == FString("PTY"))//강수 형태, (초단기) 없음(0), 비(1), 비/눈(2), 눈(3), 빗방울(5), 빗방울눈날림(6), 눈날림(7)
		{
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));

			if (fcstValue == FString("1"))
				weather_01.rainy = 1;
			else if (fcstValue == FString("2"))
				weather_01.snow_rainy = 1;
			else if (fcstValue == FString("3"))
				weather_01.snow = 1;
			else if (fcstValue == FString("5"))
				weather_01.rainy = 1;
			else if (fcstValue == FString("6"))
				weather_01.snow_rainy = 1;
			else if (fcstValue == FString("7"))
				weather_01.snow = 1;

			flag2 = true;
		}
		else if (!flag3 && category == FString("WSD"))//풍속
		{
			FString fcstValue = JsonObj->GetStringField(TEXT("fcstValue"));
			float fcstValue_float = UKismetStringLibrary::Conv_StringToFloat(fcstValue);

			if (fcstValue_float >= 14.0f)
				weather_01.windy = 1;

			flag3 = true;
		}

		if (flag1 && flag2 && flag3)
			break;
	}
	auto pre_now_weather = now_weather;
	now_weather = BitfieldToStruct(weather_01);

	UWidget_Weather* temp = Cast<UWidget_Weather>(Instance->Widget_Weather);

	if (Instance->Widget_Weather != nullptr && (temp->bFirstActivate || pre_now_weather != now_weather))
	{
		temp->bFirstActivate = false;

		if (!(temp->bImage_bg))
			temp->PlayAnimation(temp->Dissolve_bg);
		else
			temp->PlayAnimation(temp->Dissolve_bg_sec);

		if (!(temp->bImage_now))
			temp->PlayAnimation(temp->Dissolve_now);
		else
			temp->PlayAnimation(temp->Dissolve_now_sec);
	}
}

void UHttp_weather::SetWeatherPointer(UUserWidget* widget)
{
	if (!Instance)
	{
		Instance = NewObject<UHttp_weather>();
		if (!Instance)
		{
			UE_LOG(weather_api, Error, TEXT("Could not create an instance for UHttp_weather"));
			return;
		}

		Instance->AddToRoot();
	}

	Instance->Widget_Weather = widget;
}

hyundai_weather UHttp_weather::BitfieldToStruct(weather_bitfield bitfield, int32 additional_time)
{
	hyundai_weather temp = hyundai_weather::snow;

	int32 hour = UKismetMathLibrary::Now().GetHour() + additional_time;
	bool isnight = false;
	if (hour >= 18)//오후 6시 이후라면. 밤이라면.
		isnight = true;

	//우선 순위에 따라 날씨 설정.
	if (bitfield.snow_rainy == 1)
		temp = hyundai_weather::snow_rainy;
	else if (bitfield.rainy == 1)
		temp = hyundai_weather::rainy;
	else if (bitfield.snow == 1)
		temp = hyundai_weather::snow;
	else if (bitfield.windy == 1)
		temp = hyundai_weather::windy;
	else if (bitfield.cloudy == 1)
		temp = hyundai_weather::cloudy;
	else if (bitfield.mostly_cloudy == 1 && !isnight)
		temp = hyundai_weather::mostly_cloudy01;
	else if (bitfield.mostly_cloudy == 1 && isnight)
		temp = hyundai_weather::mostly_cloudy02;
	else if (bitfield.sunny == 1 && !isnight)
		temp = hyundai_weather::sunny01;
	else if (bitfield.sunny == 1 && isnight)
		temp = hyundai_weather::sunny02;

	return temp;
}
