// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetToolFunctionLibrary.generated.h"


UCLASS()
class UEASSETTOOLS_API UAssetToolFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// �p�b�P�[�W�����w�肵�ăA�Z�b�g���擾����
	// @param[in]  PackageName              �擾����A�Z�b�g�̃p�b�P�[�W��
	// @param[out] OutAssetData             ���������A�Z�b�g�̊e����
	// @param[in]  bIncludeOnlyOnDiskAssets True�̏ꍇ�f�B�X�N��Ɋi�[����Ă���f�[�^�݂̂����W����
	// @return �擾�ɐ��������True�A�����łȂ����False��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static bool GetAssetsByPackageName(const FName& PackageName, TArray<FAssetData> &OutAssetData, bool bIncludeOnlyOnDiskAssets = false);

	// �w�肵���p�X�ȉ��̃p�b�P�[�W���擾����
	// @param[in]  PackagePaths             �p�b�P�[�W����������N�_�p�X
	// @param[out] OutAssetData             ���������A�Z�b�g�̊e����
	// @param[in]  bRecursivePaths          PackagePaths�Ŏw�肵���p�X�ȉ����ċA�I�Ɍ������邩�H
	// @param[in]  bIncludeOnlyOnDiskAssets True�̏ꍇ�f�B�X�N��Ɋi�[����Ă���f�[�^�݂̂����W����
	// @return �擾�ɐ��������True�A�����łȂ����False��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static bool GetAssetsByPaths(const TArray<FName> &PackagePaths, TArray<FAssetData> &OutAssetData, bool bRecursivePaths = false, bool bIncludeOnlyOnDiskAssets = false);
	
	// �w�肵���N���X�����S�ẴA�Z�b�g���擾����
	// @param[in]  ClassPathName     �w�肷��N���X�p�X��
	// @param[out] OutAssetData      ���������A�Z�b�g�̊e����
	// @param[in]  bSearchSubClasses ClassPathName�Ŏw�肵���N���X�̔h���N���X���������邩�H
	// @return �擾�ɐ��������True�A�����łȂ����False��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static bool GetAssetsByClass(FTopLevelAssetPath ClassPathName, TArray<FAssetData> &OutAssetData, bool bSearchSubClasses = false);

	// �w�肵���p�b�P�[�W���ˑ�����A�Z�b�g���擾
	// @param[in]  PackageName              �ˑ����A�Z�b�g�̃p�b�P�[�W��
	// @param[out] OutAssetData             ���������ˑ���A�Z�b�g�̊e����
	// @param[in]  bIncludeOnlyOnDiskAssets True�̏ꍇ�f�B�X�N��Ɋi�[����Ă���f�[�^�݂̂����W����
	// @return �擾�ɐ��������True�A�����łȂ����False��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static bool GetDependencies(const FName &PackageName, TArray<FAssetData>& OutDependencyAssetData, bool bIncludeOnlyOnDiskAssets = false);

	// �w�肵���p�b�P�[�W���Q�Ƃ���A�Z�b�g���擾
	// @param[in]  PackageName              �Q�ƌ��A�Z�b�g�̃p�b�P�[�W��
	// @param[out] OutAssetData             ���������Q�Ɛ�A�Z�b�g�̊e����
	// @param[in]  bIncludeOnlyOnDiskAssets True�̏ꍇ�f�B�X�N��Ɋi�[����Ă���f�[�^�݂̂����W����
	// @return �擾�ɐ��������True�A�����łȂ����False��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static bool GetReferencers(const FName &PackageName, TArray<FAssetData> &OutDependencyAssetData, bool bIncludeOnlyOnDiskAssets = false);

	// ���[�h�ς݃A�Z�b�g�𕡐�����
	// @param[in] SrcAsset       �������A�Z�b�g�̃|�C���^
	// @param[in] DstPackageName ������A�Z�b�g�̃p�b�P�[�W��
	// @return ���������A�Z�b�g�̃C���X�^���X��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static UObject* DuplicateLoadedAsset(UObject* SrcAsset, const FName& DstPackageName);

	// �A�Z�b�g�𕡐�����
	// @param[in] SrcPackageName �������A�Z�b�g�̃p�b�P�[�W��
	// @param[in] DstPackageName ������A�Z�b�g�̃p�b�P�[�W��
	// @return ���������A�Z�b�g�̃C���X�^���X��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting|Asset")
	static UObject* DuplicateAsset(const FName& SrcPackageName, const FName& DstPackageName);
};
