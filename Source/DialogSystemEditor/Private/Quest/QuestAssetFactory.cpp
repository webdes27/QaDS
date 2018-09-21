#include "DialogSystemEditor.h"
#include "QuestAssetFactory.h"
#include "QuestAsset.h"
#include "QuestScript.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

UQuestAssetFactory::UQuestAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestAsset::StaticClass();
}

UObject* UQuestAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UQuestAsset* asset = NewObject<UQuestAsset>(InParent, Class, Name, Flags | RF_Transactional);
	asset->Name = Name;

	return asset;
}

bool UQuestAssetFactory::CanCreateNew() const
{
	return true;
}