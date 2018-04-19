//  Dalton Rick, Nathan Foster, Kush Patel, Desmond Lee, Kurtis Hoang
//
//  CIS 22C
//
//  Group Project - Leaugue of Legends Character engine
//
//  Equipment.h

#ifndef ____Equipment__
#define ____Equipment__
#include <sstream>
using namespace std;

//***************************************************************************************************************************************WEAPON
class Weapon {
	friend class Equipment;
private:
	//string weaponName;
	int attackDamageBoost;
	double attackSpeedBoost;

public:
	//Default Constructor
	Weapon() : attackDamageBoost(0), attackSpeedBoost(0.0) {}
	//Weapon(string name, int damage, double speed){ weaponName = name, attackDamageBoost = damage, attackSpeedBoost = speed; }

	//Copy Constructor
	Weapon(const Weapon &data);
	// ** Setters (manipulators) **
	void setAttackDamageBoost(int);
	void setattackSpeedBoost(double);

	// ** Accessors **
	int getAttackDamage() { return attackDamageBoost; }
	double getAttackSpeed() { return attackSpeedBoost; }

	// ** Other Member Functions **
	void print();
	bool operator==(const Weapon &data);
};
Weapon::Weapon(const Weapon &data) {
	attackDamageBoost = data.attackDamageBoost;
	attackSpeedBoost = data.attackSpeedBoost;
}

void Weapon::setAttackDamageBoost(int damage) {
	attackDamageBoost = damage;
}

void Weapon::setattackSpeedBoost(double speed) {
	attackSpeedBoost = speed;
}
void Weapon::print() {
	cout << "Attack Damage Boost: " << attackDamageBoost << endl;
	cout << "Attack Speed Boost: " << attackSpeedBoost << endl;
}
bool Weapon::operator==(const Weapon &data) {

	if (attackDamageBoost != data.attackDamageBoost) {
		return false;
	}
	else if (attackSpeedBoost != data.attackSpeedBoost) {
		return false;
	}
	else
		return true;
}

//***************************************************************************************************************************************ARMOR
class Armor {
	friend class Equipment;
private:
	//string armorName;
	double armorBoost;
	int healthBoost;
	int manaBoost;
	double magicResistanceBoost;



public:
	//Default Constructor
	Armor() : armorBoost(0), healthBoost(0), manaBoost(0), magicResistanceBoost(0) {}

	//Copy Constructor
	Armor(const Armor &data);
	// ** Setters (manipulators) **
	void setArmorBoost(double);
	void setHealthBoost(int);
	void setManaBoost(int);
	void setMagicResistanceBoost(double);

	// ** Accessors **
	double getArmorBoost() { return armorBoost; }
	int getHealthBoost() { return healthBoost; }
	int getManaBoost() { return manaBoost; }
	double getMagicResistanceBoost() { return magicResistanceBoost; }

	// ** Other Member Functions **
	bool operator==(const Armor &data);

};
Armor::Armor(const Armor &data) {
	armorBoost = data.armorBoost;
	healthBoost = data.healthBoost;
	manaBoost = data.manaBoost;
	magicResistanceBoost = data.magicResistanceBoost;

}
bool Armor::operator==(const Armor &data) {
	if (armorBoost != data.armorBoost) {
		return false;
	}
	else if (healthBoost != data.healthBoost) {
		return false;
	}
	else if (manaBoost != data.manaBoost) {
		return false;
	}
	else if (magicResistanceBoost != data.magicResistanceBoost) {
		return false;
	}
	else
		return true;
}

void Armor::setArmorBoost(double ab) {
	armorBoost = ab;
}
void Armor::setHealthBoost(int hb) {
	healthBoost = hb;
}
void Armor::setManaBoost(int mb) {
	manaBoost = mb;
}
void Armor::setMagicResistanceBoost(double mrb) {
	magicResistanceBoost = mrb;
}

//********************************************************************************** EQUIPMENT

class Equipment {
private:
	string name;
	string type;
	Weapon weapon;
	Armor armor;

public:

	Equipment() : name(""), type("") {}
	Equipment(const Equipment &data);

	// ** Setters **
	void setName(string n);
	void setType(string typespecifier);
	void setWeapon(Weapon e);
	void setArmor(Armor a);


	// ** Getters **
	string getName() { return name; };
	string getType() { return type; };
	Weapon getWeapon() { return weapon; };
	Armor getArmor() { return armor; };
	string getDataString();

	// ** Other Functions **
	void print();
	bool operator==(const Equipment &data);
};
Equipment::Equipment(const Equipment &data) {
	name = data.name;
	type = data.type;
	armor = data.armor;
	weapon = data.weapon;

}
void Equipment::setName(string n) {
	name = n;
}
void Equipment::setType(string typespecifier) {
	type = typespecifier;
}


bool Equipment::operator==(const Equipment &data) {

	if (name != data.name) {
		return false;
	}
	else if (type != data.type) {
		return false;
	}
	else if (weapon == data.weapon && armor == data.armor) {
		return true;
	}
	else
		return false;

}
void Equipment::setWeapon(Weapon e) {
	type = "Weapon";
	weapon.setAttackDamageBoost(e.getAttackDamage());
	weapon.setattackSpeedBoost(e.getAttackSpeed());

}
void Equipment::setArmor(Armor a) {
	type = "Armor";
	armor.setArmorBoost(a.getArmorBoost());
	armor.setHealthBoost(a.getHealthBoost());
	armor.setMagicResistanceBoost(a.getMagicResistanceBoost());
	armor.setManaBoost(a.getManaBoost());

}
void Equipment::print() {



	if (type == "Weapon") {
		cout << "Name: " << name << endl;
		cout << "Attack Damage: " << weapon.getAttackDamage() << endl;
		cout << "Attack Speed: " << weapon.getAttackSpeed() << endl;
	}
	else if (type == "Armor") {
		cout << "Name: " << name << endl;
		cout << "Armor Boost: " << armor.getArmorBoost() << endl;
		cout << "Health Boost: " << armor.getHealthBoost() << endl;
		cout << "Magic Resistance: " << armor.getMagicResistanceBoost() << endl;
		cout << "Mana Boost: " << armor.getManaBoost() << endl;

	}
	else {
		cout << "\nEmpty Weapon Slot!" << endl;
	}
}
string Equipment::getDataString() {
	string data; string number;

	if (type == "Weapon") {
		data = name + "\n\n";
		number = to_string(weapon.getAttackSpeed());
		number.erase(number.find_last_not_of('0') + 1, string::npos);
		data += "+ " + number;
		data += " Attack speed\n+ ";
		data += to_string(weapon.getAttackDamage());
		data += " Attack damage\n";

	}
	else if (type == "Armor") {
		data = name + "\n\n";
		number = to_string(armor.getArmorBoost());
		number.erase(number.find_last_not_of('0') + 1, string::npos);
		data += "+ " + number;
		data += " Armor\n+ ";
		data += to_string(armor.getHealthBoost());
		data += " Health\n+ ";
		number = to_string(armor.getMagicResistanceBoost());
		number.erase(number.find_last_not_of('0') + 1, string::npos);
		data += number;
		data += " Magic Resistance\n+ ";
		data += to_string(armor.getManaBoost());
		data += " Mana\n";
	}
	else {
		data = "\nEmpty Weapon Slot\n";
	}

	return data;
}
#endif /* defined(____Equipment__) */