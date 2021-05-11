// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ObjectItems/XYXItemArrow.h"

UXYXItemArrow::UXYXItemArrow()
{

}

TSubclassOf<AXYXArrowProjectileBase> UXYXItemArrow::GetProjectile_Implementation()
{
	return Projectile;
}
