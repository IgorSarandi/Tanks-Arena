#include "Bullet.h"


std::vector<Bullet*> Bullet::storage;

Bullet::Bullet(Tanks* parent) : 
	cur_dir(parent->GetDirection()),
	parent_coord(parent->GetPosition()),
	activate(false)
{
	Bullet::storage.push_back(this);

	this->SetPosition(parent->GetPosition().first, parent->GetPosition().second);

	this->SetParent(parent);
}

Bullet::~Bullet()
{
}

void Bullet::SetParent(Tanks* _parent)
{
	parent = _parent;
}

Tanks* Bullet::GetParent()
{
	return parent;
}

std::pair<char, int> Bullet::Figure()
{
	return std::make_pair('*', 12);
}