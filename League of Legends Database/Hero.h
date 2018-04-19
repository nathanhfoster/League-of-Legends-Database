//  Dalton Rick, Nathan Foster, Kush Patel, Desmond Lee, Kurtis Hoang
//
//  CIS 22C
//
//  Group Project - Leaugue of Legends Character engine
//
//  Hero.h


#ifndef ____Hero__
#define ____Hero__

#include <string>
#include <iostream>
#include <iomanip>
#include "Equipment.h"
using namespace std;


class Hero {

private:
	string name;
	int health;
	int mana; //
	int attackRange; // i.e. Ranged 550
	int attackDamage;
	double attackSpeed;
	double armor;
	double magicResistance;
	int movementSpeed;
	int equippedCount;
	Equipment Inventory[6];


public:

	//Default Constructor
	Hero() : name(""), health(0), mana(0), attackRange(0), attackDamage(0), attackSpeed(0.0), armor(0.0), magicResistance(0.0), movementSpeed(0), equippedCount(0) {}

	//Copy Constructor
	Hero(const Hero &data);

	//Default Constuctor
	~Hero();

	// ** Setters (manipulators) **
	void setName(string championName);
	void setHealth(int life);
	void setMana(int);
	void setAttackRange(int);
	void setAttackDamage(int);
	void setAttackSpeed(double);
	void setarmor(double);
	void setMagicResistance(double);
	void setMovementSpeed(int);
	void setEquipment(const Equipment e);
	void zeroEquippedCount() { equippedCount = 0; };

	// ** Accessors **
	string getName() { return name; };
	int getHealth() { return health; };
	int getMana() { return mana; };
	int getAttackRange() { return attackRange; };
	int getAttackDamage() { return attackDamage; };
	double getAttackSpeed() { return attackSpeed; };
	double getarmor() { return armor; };
	double getMagicResistance() { return magicResistance; };
	int getMovementSpeed() { return movementSpeed; };
	void removeEquipment(int count);
	int getEquippedCount() { return equippedCount; };
	Equipment sendInventory(int count);
	string getEquipment(int index);
	string getEquipmentName(int index);
	//bool operator==(const Equipment &data);

	// ** Other Member Functions **
	void print();
	void printEquipment();

};

Hero::Hero(const Hero &data) {

	name = data.name;
	health = data.health;
	mana = data.mana;
	attackRange = data.attackRange;
	attackDamage = data.attackDamage;
	attackSpeed = data.attackSpeed;
	armor = data.armor;
	magicResistance = data.magicResistance;
	movementSpeed = data.movementSpeed;
	equippedCount = data.equippedCount;

	for (int i = 0; i < data.equippedCount; i++)
		Inventory[i] = data.Inventory[i];
	

}
Hero::~Hero() {}

void Hero::setName(string championName)
{
	name = championName;
}
void Hero::setHealth(int life) {
	health = life;
}
void Hero::setMana(int MA) {
	mana = MA;
}
void Hero::setAttackRange(int A_R) {
	attackRange = A_R;
}
void Hero::setAttackDamage(int A_D) {
	attackDamage = A_D;
}

void Hero::setAttackSpeed(double A_S) {
	attackSpeed = A_S;
}
void Hero::setarmor(double AMR) {
	armor = AMR;
}
void Hero::setMagicResistance(double magRes) {
	magicResistance = magRes;
}
void Hero::setMovementSpeed(int speed) {
	movementSpeed = speed;
}

void Hero::setEquipment(const Equipment e) {

	Equipment temp; Armor tempArmor; Weapon weapon;

	if (equippedCount < 6) {

		Inventory[equippedCount] = e;

		if (Inventory[equippedCount].getType() == "Weapon") {
			weapon = Inventory[equippedCount].getWeapon();
			attackDamage += weapon.getAttackDamage();
			attackSpeed += weapon.getAttackSpeed();


		}
		else if (Inventory[equippedCount].getType() == "Armor") {
			tempArmor = Inventory[equippedCount].getArmor();
			armor += tempArmor.getArmorBoost();
			health += tempArmor.getHealthBoost();
			mana += tempArmor.getManaBoost();
			magicResistance += tempArmor.getMagicResistanceBoost();

		}
		equippedCount++;


	}
	else
	{
		cout << "\nEquipment Slots Full!\n";
	}

}

void Hero::removeEquipment(int count) {

	Weapon tempWeapon; Armor tempArmor; Equipment removed;

	removed = Inventory[count - 1];

	if (removed.getType() == "Weapon") {
		tempWeapon = removed.getWeapon();
		attackDamage -= tempWeapon.getAttackDamage();
		attackSpeed -= tempWeapon.getAttackSpeed();

	}
	else if (removed.getType() == "Armor") {
		tempArmor = removed.getArmor();
		armor -= tempArmor.getArmorBoost();
		health -= tempArmor.getHealthBoost();
		mana -= tempArmor.getManaBoost();
		magicResistance -= tempArmor.getMagicResistanceBoost();

	}
	else {
		cout << "\nWeapon Not Found!" << endl;
		return;
	}
	equippedCount--;

	for (int i = count - 1; i < equippedCount; i++) {
		Inventory[i] = Inventory[i + 1];
	}



}

void Hero::print() {
	cout << setprecision(3);

	cout << left << "Champion:\t" << name << endl;
	cout << left << setw(20) << "Health: " << right << setw(6) << health << endl;
	cout << left << setw(20) << "Mana: " << right << setw(6) << mana << endl;
	cout << left << setw(20) << "Attack Range: " << right << setw(6) << attackRange << endl;
	cout << left << setw(20) << "Attack Damage: " << right << setw(6) << attackDamage << endl;
	cout << left << setw(20) << "Attack Speed: " << right << setw(6) << attackSpeed << endl;
	cout << left << setw(20) << "Armor: " << right << setw(6) << armor << endl;
	cout << left << setw(20) << "Magic Resistance: " << right << setw(6) << magicResistance << endl;
	cout << left << setw(20) << "Movement Speed: " << right << setw(6) << movementSpeed << endl;
	cout << left << setw(20) << "Equipped Items: " << right << setw(6) << equippedCount << endl;
}
void Hero::printEquipment() {

	for (int i = 0; i < equippedCount; i++) {
		cout << endl << i + 1 << "." << endl;
		Inventory[i].print();
		cout << endl;
	}

}
Equipment Hero::sendInventory(int count) {

	return Inventory[count];
}
string Hero::getEquipment(int index) {

	return Inventory[index].getDataString();
}

string Hero::getEquipmentName(int index)
{
	return Inventory[index].getName();
}






#endif /* defined(____Hero__) */
