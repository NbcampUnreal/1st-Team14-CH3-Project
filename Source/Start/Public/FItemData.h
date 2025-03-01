#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemData.generated.h"

// 🔹 데이터 테이블에서 사용할 구조체
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
    // ✅ 아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FText Name;

    // ✅ 아이템 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FText Description;

};
