#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

class Egg {
private:
    const std::string size;
    const std::string flavor;

public:
    Egg(std::string size, std::string flavor) : size(std::move(size)), flavor(std::move(flavor)) {}

    virtual ~Egg() = default;

    const std::string& getSize() const { return size; }
    const std::string& getFlavor() const { return flavor; }

    virtual void eat_egg() const = 0;
};

class bad_egg : public std::runtime_error {
public:
    bad_egg() : std::runtime_error("This egg is rotten.") {}
};

class ScrambledEgg : public Egg {
public:
    ScrambledEgg() : Egg("Medium", "Buttery") {}

    void eat_egg() const override {
        std::cout << "You eat a buttery scrambled egg.!\n";
    }
};

class BoiledEgg : public Egg {
public:
    BoiledEgg() : Egg("Large", "Savory") {}

    void eat_egg() const override {
        std::cout << "You eat a savory boiled egg.\n";
    }
};

class RottenEgg : public Egg {
public:
    RottenEgg() : Egg("Small", "Rotten") {}

    void eat_egg() const override {
        throw bad_egg();
    }
};

class Carton {
private:
    std::vector<std::unique_ptr<Egg>> eggs;

public:
    void addEgg(std::unique_ptr<Egg> egg) {
        if (eggs.size() >= 12) {
            std::cout << "The carton is full.\n";
            return;
        }

        eggs.push_back(std::move(egg));
    }

    void eatEgg() {
        if (eggs.empty()) {
            std::cout << "The carton is empty.\n";
            return;
        }

        std::unique_ptr<Egg> egg = std::move(eggs.back());
        eggs.pop_back();

        try {
            egg->eat_egg();
        }
        catch (const bad_egg& e) {
            std::cout << "Warning: Bad egg! " << e.what() << '\n';
        }
    }
};

int main() {
    Carton carton;

    carton.addEgg(std::make_unique<ScrambledEgg>());
    carton.addEgg(std::make_unique<RottenEgg>());
    carton.addEgg(std::make_unique<BoiledEgg>());

    carton.eatEgg();
    carton.eatEgg();
    carton.eatEgg();
    carton.eatEgg();

    return 0;
}
