#include <iostream>
#include <string>

using namespace std;

class Character {
   public:
    string name;
    int health, damage;

    Character(string n, int h, int d) : name(n), health(h), damage(d) {};
    virtual ~Character() {};

   public:
    virtual void Attack(Character* target) { target->TakeDamage(damage); };
    virtual void TakeDamage(int amount) {
        health -= amount;
        cout << name << " Current Health: " << health << endl;
    };
};

class Player : public Character {
   public:
    Player(string n, int h, int d) : Character(n, h, d) {};
    void Attack(Character* target) override {
        cout << "Hero slash!" << endl;
        Character::Attack(target);
    }
};

class Enemy : public Character {
   public:
    Enemy(string n, int h, int d) : Character(n, h, d) {};
    void Attack(Character* target) override {
        cout << "Monster bite!" << endl;
        Character::Attack(target);
    }
};

class EnemyFactory {
   public:
    static Enemy* CreateEnemy(int level) {
        if (level == 1) return new Enemy("Slime", 10, 1);
        if (level == 2) return new Enemy("Goblin", 50, 5);
        if (level == 3) return new Enemy("Sucipto", 1000, 100);
        return nullptr;
    }
};

int main() {
    Player player = Player("Ujang", 100, 10);

    Enemy* enemy = EnemyFactory::CreateEnemy(1);

    player.Attack(enemy);
    enemy->Attack(&player);

    delete enemy;
}