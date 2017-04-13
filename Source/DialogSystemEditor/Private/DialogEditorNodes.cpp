#include "DialogSystemEditor.h"
#include "EdGraph/EdGraphPin.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "StoryInformationManager.h"
#include "EdGraph/EdGraph.h"
#include "DialogEditorNodes.h"
#include "DialogAsset.h"

const FString FPinDataTypes::PinType_Root = "root";

//UDialogNodeEditorBase...........................................................................................

TArray<UDialogNodeEditorBase*> UDialogNodeEditorBase::GetChildNodes()
{
	TArray<UDialogNodeEditorBase*> ChildNodes;

	for (auto Pin : Pins)
	{
		if (Pin->Direction != EGPD_Output)
			continue;

		for (int i = 0; i < Pin->LinkedTo.Num(); i++)
		{
			if (Pin->LinkedTo[i])
				ChildNodes.Add((UDialogNodeEditorBase*)Pin->LinkedTo[i]->GetOwningNode());
		}
	}

	return ChildNodes;
}

void UDialogNodeEditorBase::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	if (PropertyObserver.IsValid())
	{
		FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
		PropertyObserver->OnPropertyChanged(this, PropertyName);
	}

	Super::PostEditChangeProperty(e);
}

//PhraseNode..........................................................................................................

UPhraseNode::UPhraseNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
}

void UPhraseNode::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, FPinDataTypes::PinType_Root, TEXT(""), NULL, false, false, TEXT(""));
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, FPinDataTypes::PinType_Root, TEXT(""), NULL, false, false, TEXT(""));
}

FLinearColor UPhraseNode::GetNodeTitleColor() const
{
	//return IsPlayer ? FLinearColor(1.0f, 0.4f, 0.0f) : FLinearColor(0.0f, 0.4f, 1.0f);
	return IsPlayer ? FLinearColor(1.0f, 0.4f, 0.0f) : FLinearColor(0.08f, 0.08f, 0.08f);
}

FText UPhraseNode::GetTooltipText() const
{
	return FText::FromString("Dialog Phrase");
}

FText UPhraseNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Text.IsEmpty())
		return FText::FromString("Phrase");

	return Text;
}

void UPhraseNode::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	UDialogNodeEditorBase::PostEditChangeProperty(e);
}

//URootNode...........................................................................................

URootNode::URootNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URootNode::AllocateDefaultPins()
{
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, FPinDataTypes::PinType_Root, TEXT(""), NULL, false, false, TEXT("Start"));
}

FLinearColor URootNode::GetNodeTitleColor() const
{
	return FLinearColor(0.08f, 0.08f, 0.08f);
}

FText URootNode::GetTooltipText() const
{
	return FText::FromString("Dialog Start Node");
}

FText URootNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Start ") + Cast<UDialogAsset>(GetGraph()->GetOuter())->Name.ToString());
}