#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>


class Creature {
public:
    Creature(int attack, int defense, int health, std::pair<int, int> damage_range)
        : attack(attack), defense(defense) {
        setHealth(health);
        setDamageRange(damage_range);
    }

    bool is_dead() const {
        return health <= 0;
    }

    int calculate_attack_modifier(const Creature& target) const {
        return std::max(attack - target.defense + 1, 1);
    }

    bool successful_attack(const Creature& target) const {
        int modifier = calculate_attack_modifier(target);
        std::vector<int> dice_rolls(modifier);
        for (int i = 0; i < modifier; ++i) {
            dice_rolls[i] = rand() % 6 + 1;  // Бросок кубика от 1 до 6
        }
        return std::any_of(dice_rolls.begin(), dice_rolls.end(), [](int roll) { return roll == 5 || roll == 6; });
    }

    int getHealth() const {
        return health;
    }

    void setHealth(int new_health) {
        health = new_health;
    }

    void take_damage(int damage) {
        health -= damage;
    }

    void heal() {
        const int max_health = 30;  // Максимальное здоровье, можно изменить
        if (health < max_health) {
            health = std::min(static_cast<int>(health + max_health * 0.3), max_health);
        }
    }

    std::pair<int, int> getDamageRange() const {
        return damage_range;
    }

    void setDamageRange(std::pair<int, int> new_damage_range) {
        damage_range = new_damage_range;
    }

protected:
    int attack;
    int defense;

private:
    int health;
    std::pair<int, int> damage_range;
};

class Player : public Creature {
public:
    Player(int attack, int defense, int health, std::pair<int, int> damage_range)
        : Creature(attack, defense, health, damage_range) {}

    void heal() {
        const int max_health = 30;  // Максимальное здоровье, можно изменить
        if (getHealth() < max_health) {
            setHealth(std::min(static_cast<int>(getHealth() + max_health * 0.3), max_health));
        }
    }
};

class Monster : public Creature {
public:
    Monster(int attack, int defense, int health, std::pair<int, int> damage_range)
        : Creature(attack, defense, health, damage_range) {}
};

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));  // Инициализация генератора случайных чисел

    Player player(25, 20, 30, std::make_pair(1, 6));
    Monster monster(20, 15, 25, std::make_pair(2, 8));

    while (!player.is_dead() && !monster.is_dead()) {
        if (player.successful_attack(monster)) {
            auto damage_range = player.getDamageRange();
            int damage = rand() % (damage_range.second - damage_range.first + 1) + damage_range.first;
            monster.take_damage(damage);
            std::cout << "Игрок нанес монстру " << damage << " урона. Здоровье монстра: " << monster.getHealth() << std::endl;
        }
        else {
            std::cout << "Игрок не попал по монстру." << std::endl;
        }

        if (monster.successful_attack(player)) {
            auto damage_range = monster.getDamageRange();
            int damage = rand() % (damage_range.second - damage_range.first + 1) + damage_range.first;
            player.take_damage(damage);
            std::cout << "Монстр нанес игроку " << damage << " урона. Здоровье игрока: " << player.getHealth() << std::endl;
        }
        else {
            std::cout << "Монстр не попал по игроку." << std::endl;
        }

        if (rand() / static_cast<float>(RAND_MAX) < 0.3) {  // Вероятность исцеления игрока
            player.heal();
        }
    }

    if (player.is_dead()) {
        std::cout << "Игрок проиграл." << std::endl;
    }
    else if (monster.is_dead()) {
        std::cout << "Игрок победил." << std::endl;
    }

    return 0;
}
