#include "Attack.h"
#include "Character.h"

C_Attack::C_Attack(Character* characterRef)
{
	characterReference = characterRef;
}

void C_Attack::SetCharacterReference(Character* characterRef)
{
	characterReference = characterRef;
}

float C_Attack::AttackSpeed(Character* character)
{
	possibleRefresh = refreshTime - ((character->speed / 100) * character->weapon->speed);
	return possibleRefresh;
}

float C_Attack::damageCalc(Character& a, Character& b) //A is attacker, B is victim
{
	int weaponPower = 1;
	if (a.characterName == "Arnould")weaponPower = a.weapon->attackPower;

	int damage = (a.strength + weaponPower + (rand() % (maxDamage-minDamage) + minDamage)); //Damage is comprised of both statistic strength and the power of the weapon

	damage = (damage - b.defence);
	if (damage < 1)
		damage = 1;
	cout << attackText << " deals " << damage << " points of damage to " << b.characterName << "! " << endl << endl;
	possibleDamage = damage;
	return damage;
}


void C_Attack::Attack(Character& attacker, Character &opponent, int block)
{
	int damage = (damageCalc(attacker, opponent)/block);
	opponent.Damage(damage);
	cout << damage << " damage done to " << attacker.characterName << " by : " << attackText <<" current health of character: " << attacker.health << endl;
	attackCompleted = true;

	PlayAttackSound(SOUND_ATTACK);
	PlayAttackSound(SOUND_HURT);
}

void C_Attack::FailedBlockedAttack(Character& attacker, Character &opponent)
{
	cout << attacker.characterName << " FAILED BLOCK ATTACK!" << endl;
	blockingTime = 0; 
	blockingStatus = 2;
	Attack(attacker, opponent, 1);
}

void C_Attack::BlockedAttack(Character& attacker, Character &opponent)
{
	cout << attacker.characterName << " BLOCKED ATTACK SUCCESSFULLY!" << endl;
	blockingTime = 0;
	blockingStatus = 1;
	Attack(attacker, opponent, 2);
	PlayAttackSound(SOUND_BLOCK);
}

void C_Attack::PlayAttackSound(int soundFile)
{
	FileLoader* fileLoader = new FileLoader;
	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		std::cout << "Can't initialize device";
	HSAMPLE sound;
	switch (soundFile)
	{
	case SOUND_ATTACK:
		sound = fileLoader->loadSample("Sound/Combat/Combat_Attack.wav");
		break;
	case SOUND_BLOCK:
		sound = fileLoader->loadSample("Sound/Combat/Combat_Block.wav");
		break;
	case SOUND_HURT:
		sound = fileLoader->loadSample("Sound/Combat/Combat_Hurt.wav");
		break;
	default:
		return;
	}
	delete fileLoader;

	HCHANNEL ch = BASS_SampleGetChannel(sound, FALSE);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, 1);
	if (!BASS_ChannelPlay(ch, FALSE))
		cout << "Can't play sample" << endl;
}