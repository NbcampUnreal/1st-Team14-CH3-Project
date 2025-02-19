#include "CCharacter.h"

ACCharacter::ACCharacter()
{
    Health = 100;
}

int ACCharacter::GetHP() const
{
    return Health;
}

void ACCharacter::SetHP(int HP)
{
    Health = FMath::Clamp(HP, 0, 100);
}

void ACCharacter::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("Character Attacked!"));
}

void ACCharacter::TakeDamaged(int Amount)
{
    Health -= Amount;
    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character Died!"));
    }
}