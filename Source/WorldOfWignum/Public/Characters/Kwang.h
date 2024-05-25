// World of Wignum by Rany Touffaha

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "Kwang.generated.h"

// Forward declarations for Kwang class
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class UWignumOverlay;

UCLASS()
class WORLDOFWIGNUM_API AKwang : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AKwang();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** <IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */
	
	/** <IPickupInterface>*/
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	/** </IPickupInterface> */
	
protected:
	virtual void BeginPlay() override;

	/** Callback functions for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	/** Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual bool CanAttack() const override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	bool CanDisarm() const;
	bool CanArm() const;
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName) const;
	virtual void Die_Implementation() override;
	bool IsOccupied() const;
	bool HasEnoughStamina() const;
		
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack() const;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand() const;

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	
	/** Input Actions */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* KwangContext;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

private:
	/** UI functions */
	void InitialiseWignumOverlay(const APlayerController* PlayerController);
	void InitialiseInputMappingContext(const APlayerController* PlayerController) const;
	void SetHUDHealth() const;
	
	bool IsUnoccupied() const;
	
	/** Character Components */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/** UI Components */
	UPROPERTY()
	UWignumOverlay* WignumOverlay;
	
public:
	/** Getters and Setters */
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	
};
 