#pragma once

#include "UObject/Object.h"

#include "InkChoice.generated.h"

namespace ink::runtime { class choice; }

UCLASS(BlueprintType)
class UInkChoice : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	FString GetText() const;

	UFUNCTION(BlueprintPure)
	int GetIndex() const;

protected:
	friend class UInkThread;
	void Initialize(const ink::runtime::choice*);

private:
	const ink::runtime::choice* data;
};